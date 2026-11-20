/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "KillReward.h"
#include "KillRewardRules.h"

#include "Creature.h"
#include "DatabaseEnv.h"
#include "DBCEnums.h"
#include "DBCStores.h"
#include "Chat.h"
#include "Containers.h"
#include "Group.h"
#include "Item.h"
#include "Log.h"
#include "Mail.h"
#include "Map.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "ThreatManager.h"
#include "Timer.h"
#include "World.h"

#include <algorithm>
#include <cstring>
#include <limits>
#include <unordered_map>

static_assert(Skyfire::KillReward::DIFFICULTY_ID_RAID_FINDER == DIFFICULTY_25MAN_LFR, "raid finder DifficultyID changed");
static_assert(Skyfire::KillReward::DIFFICULTY_ID_FLEX == DIFFICULTY_FLEX, "flex DifficultyID changed");
static_assert(Skyfire::KillReward::DIFFICULTY_ID_NONE == DIFFICULTY_NONE, "open world DifficultyID changed");

namespace
{
    std::unordered_map<uint32, CreatureLootCurrencyList> CreatureLootCurrencyStore;
    std::unordered_map<uint32, PersonalLootTemplate> PersonalLootStore;

    void RewardCurrency(Player* player, uint8 difficulty, CreatureLootCurrencyList const* currencies)
    {
        if (currencies && Skyfire::KillReward::DifficultyPaysKillCurrency(difficulty))
            for (CreatureLootCurrency const& row : *currencies)
                if (Skyfire::KillReward::CurrencyRowMatchesDifficulty(row.LootMode, difficulty))
                    player->ModifyCurrency(row.CurrencyId, int32(row.Count)); // weekly and total caps apply here
    }

    // ItemNameDescription.dbc is keyed by item id in 5.4.8; flex items carry "Flexible".
    bool IsFlexibleItem(uint32 itemId)
    {
        ItemNameDescriptionEntry const* entry = sItemNameDescriptionStore.LookupEntry(itemId);
        return entry && entry->Description && std::strcmp(entry->Description, "Flexible") == 0;
    }

    void GivePersonalLootItem(Player* player, Creature* boss, uint32 itemId)
    {
        ItemPosCountVec dest;
        if (player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itemId, 1) == EQUIP_ERR_OK)
        {
            if (Item* item = player->StoreNewItem(dest, itemId, true, Item::GenerateItemRandomPropertyId(itemId)))
            {
                player->SendNewItem(item, 1, true, false, true); // "You receive loot" in chat, shown to the group too
                return;
            }
        }

        // bags full: the item comes by mail from the boss
        if (Item* item = Item::CreateItem(itemId, 1, player))
        {
            SQLTransaction trans = CharacterDatabase.BeginTransaction();
            item->SaveToDB(trans);
            MailDraft("Personal loot", "Your bags were full when you earned this.").AddItem(item)
                .SendMailTo(trans, player, MailSender(boss), MAIL_CHECK_MASK_COPIED);
            CharacterDatabase.CommitTransaction(trans);
            ChatHandler(player->GetSession()).PSendSysMessage("Your bags are full: your personal loot [%s] was sent by mail.", sObjectMgr->GetItemTemplate(itemId)->Name1.c_str()); // the mail owns the item now, do not touch it
        }
        else
            SF_LOG_ERROR("loot", "Personal loot: item %u could not be created for player %s (boss %u)", itemId, player->GetName().c_str(), boss->GetEntry());
    }

    // One roll per player, boss and difficulty a week: an item for the player's loot specialization
    // at the configured chance, else the boss's gold bag; then the weekly tracking quest.
    void RewardPersonalLoot(Player* player, Creature* boss, PersonalLootTemplate const& loot, uint8 difficulty)
    {
        if (!player->AddLootLockout(LootLockoutType::PersonalLoot, boss->GetEntry(), difficulty))
            return;

        bool flex = difficulty == Skyfire::KillReward::DIFFICULTY_ID_FLEX;
        float chance = Skyfire::KillReward::PersonalLootChance(sWorld->GetFloatConfig(WorldFloatConfigs::CONFIG_PERSONAL_LOOT_CHANCE),
            difficulty, player->GetMap()->GetPlayersCountExceptGMs());

        uint32 itemId = 0;
        if (roll_chance_f(chance))
        {
            std::vector<uint32> fitting;
            for (uint32 item : loot.Items)
                if (IsFlexibleItem(item) == flex && player->IsItemFitToSpecialization(sObjectMgr->GetItemTemplate(item)))
                    fitting.push_back(item);

            if (!fitting.empty())
                itemId = Skyfire::Containers::SelectRandomContainerElement(fitting);
        }

        if (!itemId)
            itemId = flex && loot.MoneyBagFlex ? loot.MoneyBagFlex : loot.MoneyBag;

        GivePersonalLootItem(player, boss, itemId);

        // a weekly tracker is repeatable, so GetQuestStatus never reads REWARDED for it: ask the weekly list
        if (loot.QuestTracker && player->GetQuestStatus(loot.QuestTracker) != QUEST_STATUS_REWARDED)
            if (Quest const* quest = sObjectMgr->GetQuestTemplate(loot.QuestTracker))
                if (player->SatisfyQuestWeek(quest, false))
                    player->RewardQuest(quest, 0, player, false);
    }

    // The killer's group members in reward range, or the killer alone. A world boss with personal
    // loot can be fought by anyone, so every player in reward range on its threat list counts too.
    std::vector<Player*> GetRewardedPlayers(Player* killer, Creature* victim, bool everyoneWhoFought)
    {
        std::vector<Player*> players;
        auto add = [&players](Player* player)
        {
            if (player && std::find(players.begin(), players.end(), player) == players.end())
                players.push_back(player);
        };

        if (Group* group = killer->GetGroup())
        {
            for (GroupReference* itr = group->GetFirstMember(); itr != nullptr; itr = itr->next())
                if (Player* member = itr->GetSource())
                    if (member->IsAtGroupRewardDistance(victim))
                        add(member);
        }
        else
            add(killer);

        if (everyoneWhoFought)
            for (HostileReference* ref : victim->getThreatManager().getThreatList())
                if (Unit* target = ref->getTarget())
                    if (Player* player = target->GetCharmerOrOwnerPlayerOrPlayerItself())
                        if (player->IsAtGroupRewardDistance(victim))
                            add(player);

        return players;
    }
}

void Skyfire::KillReward::LoadCreatureLootCurrency()
{
    uint32 oldMSTime = getMSTime();

    CreatureLootCurrencyStore.clear();

    //                                               0      1         2         3
    QueryResult result = WorldDatabase.Query("SELECT entry, lootmode, currency, count FROM creature_loot_currency");
    if (!result)
    {
        SF_LOG_INFO("server.loading", ">> Loaded 0 creature kill currency rows. DB table `creature_loot_currency` is empty.");
        return;
    }

    uint32 count = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();
        CreatureLootCurrency row;
        row.LootMode = fields[1].GetUInt32();
        row.CurrencyId = fields[2].GetUInt16();
        row.Count = fields[3].GetUInt32();

        if (!sObjectMgr->GetCreatureTemplate(entry))
        {
            SF_LOG_ERROR("sql.sql", "Table `creature_loot_currency` has a row for creature %u, which does not exist. Skipped.", entry);
            continue;
        }

        if (!sCurrencyTypesStore.LookupEntry(row.CurrencyId))
        {
            SF_LOG_ERROR("sql.sql", "Table `creature_loot_currency` has currency %u for creature %u, which does not exist. Skipped.", row.CurrencyId, entry);
            continue;
        }

        if (!row.Count || row.Count > uint32(std::numeric_limits<int32>::max()))
        {
            SF_LOG_ERROR("sql.sql", "Table `creature_loot_currency` has count %u for currency %u of creature %u, out of range. Skipped.", row.Count, row.CurrencyId, entry);
            continue;
        }

        CreatureLootCurrencyStore[entry].push_back(row);
        ++count;
    }
    while (result->NextRow());

    SF_LOG_INFO("server.loading", ">> Loaded %u creature kill currency rows for %u creatures in %u ms", count, uint32(CreatureLootCurrencyStore.size()), GetMSTimeDiffToNow(oldMSTime));
}

CreatureLootCurrencyList const* Skyfire::KillReward::GetCreatureLootCurrency(uint32 entry)
{
    auto itr = CreatureLootCurrencyStore.find(entry);
    return itr != CreatureLootCurrencyStore.end() ? &itr->second : nullptr;
}

void Skyfire::KillReward::LoadPersonalLoot()
{
    uint32 oldMSTime = getMSTime();

    PersonalLootStore.clear();

    //                                               0      1          2               3
    QueryResult result = WorldDatabase.Query("SELECT entry, money_bag, money_bag_flex, quest FROM personal_loot_template");
    if (!result)
    {
        SF_LOG_INFO("server.loading", ">> Loaded 0 personal loot templates. DB table `personal_loot_template` is empty.");
        return;
    }

    do
    {
        Field* fields = result->Fetch();
        uint32 entry = fields[0].GetUInt32();
        PersonalLootTemplate loot;
        loot.MoneyBag = fields[1].GetUInt32();
        loot.MoneyBagFlex = fields[2].GetUInt32();
        loot.QuestTracker = fields[3].GetUInt32();

        if (!sObjectMgr->GetCreatureTemplate(entry))
        {
            SF_LOG_ERROR("sql.sql", "Table `personal_loot_template` has a row for creature %u, which does not exist. Skipped.", entry);
            continue;
        }

        if (!sObjectMgr->GetItemTemplate(loot.MoneyBag))
        {
            SF_LOG_ERROR("sql.sql", "Table `personal_loot_template` has money_bag %u for creature %u, which does not exist. Skipped.", loot.MoneyBag, entry);
            continue;
        }

        if (loot.MoneyBagFlex && !sObjectMgr->GetItemTemplate(loot.MoneyBagFlex))
        {
            SF_LOG_ERROR("sql.sql", "Table `personal_loot_template` has money_bag_flex %u for creature %u, which does not exist. Set to 0.", loot.MoneyBagFlex, entry);
            loot.MoneyBagFlex = 0;
        }

        if (loot.QuestTracker && !sObjectMgr->GetQuestTemplate(loot.QuestTracker))
        {
            SF_LOG_ERROR("sql.sql", "Table `personal_loot_template` has quest %u for creature %u, which does not exist. Set to 0.", loot.QuestTracker, entry);
            loot.QuestTracker = 0;
        }

        PersonalLootStore[entry] = loot;
    }
    while (result->NextRow());

    uint32 items = 0;
    //                                   0      1
    result = WorldDatabase.Query("SELECT entry, item FROM personal_loot_item");
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            uint32 entry = fields[0].GetUInt32();
            uint32 item = fields[1].GetUInt32();

            auto itr = PersonalLootStore.find(entry);
            if (itr == PersonalLootStore.end())
            {
                SF_LOG_ERROR("sql.sql", "Table `personal_loot_item` has item %u for creature %u, which has no personal_loot_template row. Skipped.", item, entry);
                continue;
            }

            if (!sObjectMgr->GetItemTemplate(item))
            {
                SF_LOG_ERROR("sql.sql", "Table `personal_loot_item` has item %u for creature %u, which does not exist. Skipped.", item, entry);
                continue;
            }

            itr->second.Items.push_back(item);
            ++items;
        }
        while (result->NextRow());
    }

    SF_LOG_INFO("server.loading", ">> Loaded %u personal loot templates with %u items in %u ms", uint32(PersonalLootStore.size()), items, GetMSTimeDiffToNow(oldMSTime));
}

PersonalLootTemplate const* Skyfire::KillReward::GetPersonalLoot(uint32 entry)
{
    auto itr = PersonalLootStore.find(entry);
    return itr != PersonalLootStore.end() ? &itr->second : nullptr;
}

bool Skyfire::KillReward::UsesPersonalLoot(Creature const* creature)
{
    Map const* map = creature->GetMap();
    return GetPersonalLoot(creature->GetEntry()) && DifficultyUsesPersonalLoot(uint8(map->GetDifficulty()), map->Instanceable());
}

void Skyfire::KillReward::Reward(Player* killer, Creature* victim)
{
    if (!killer || !victim)
        return;

    // SkyFire keeps the base entry at every difficulty, so rows key the base entry.
    uint32 entry = victim->GetEntry();
    CreatureLootCurrencyList const* currencies = GetCreatureLootCurrency(entry);
    PersonalLootTemplate const* personalLoot = UsesPersonalLoot(victim) ? GetPersonalLoot(entry) : nullptr;
    if (!currencies && !personalLoot)
        return;

    uint8 difficulty = uint8(victim->GetMap()->GetDifficulty());
    bool worldBoss = personalLoot && !victim->GetMap()->Instanceable();

    for (Player* player : GetRewardedPlayers(killer, victim, worldBoss))
    {
        bool lockedOut = personalLoot && player->HasLootLockout(LootLockoutType::PersonalLoot, entry, difficulty);

        // flex pays its currency once a week per boss, with the personal roll (alexkulya)
        if (!(lockedOut && difficulty == DIFFICULTY_ID_FLEX))
            RewardCurrency(player, difficulty, currencies);

        if (personalLoot && !lockedOut)
            RewardPersonalLoot(player, victim, *personalLoot, difficulty);
    }
}
