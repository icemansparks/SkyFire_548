/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "BonusLoot.h"
#include "BonusLootRules.h"
#include "KillReward.h"
#include "KillRewardRules.h"

#include "Containers.h"
#include "DatabaseEnv.h"
#include "DBCStores.h"
#include "GameObject.h"
#include "Item.h"
#include "Log.h"
#include "LootMgr.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "SpellAuras.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "Timer.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"

#include <algorithm>
#include <unordered_map>
#include <vector>

namespace
{
    std::unordered_map<uint32, BonusLootTemplate> BonusLootStore;

    // SMSG_DISPLAY_TOAST types used by the bonus roll (5.4.8).
    enum class ToastType : uint8
    {
        Item = 0,
        Money = 3,
    };

    // The roll window waits for this toast, flagged as a bonus roll, to show what the roll gave.
    void SendBonusRollToast(Player* player, ToastType type, uint32 itemId, Item const* item, uint32 count)
    {
        WorldPacket data(SMSG_DISPLAY_TOAST, 1 + 6 * 4 + 4 + 1);
        data.WriteBit(true);                            // bonus roll
        data.WriteBits(uint8(type), 2);
        data.WriteBit(false);                           // display type follows
        if (type == ToastType::Item)
        {
            data.WriteBit(false);
            data.FlushBits();

            ItemTemplate const* proto = sObjectMgr->GetItemTemplate(itemId);
            data << uint32(0);                          // upgrade id
            data << uint32(item ? item->GetItemSuffixFactor() : 0);
            data << int32(item ? item->GetItemRandomPropertyId() : 0);
            data << uint32(proto ? proto->DisplayInfoID : 0);
            data << uint32(itemId);
            data << uint32(player->GetLootSpecialization());
        }
        else
            data.FlushBits();

        data << uint32(count);
        data << uint8(1);                               // display type: one frame
        player->GetSession()->SendPacket(&data);
    }

    // Items the roll can give at this difficulty, before the specialization filter.
    std::vector<uint32> GetRollItems(BonusLootTemplate const& loot, uint8 difficulty)
    {
        std::vector<uint32> items;
        if (Skyfire::BonusLoot::RollsOnPersonalLoot(difficulty))
        {
            bool flex = difficulty == Skyfire::KillReward::DIFFICULTY_ID_FLEX;
            if (PersonalLootTemplate const* personal = Skyfire::KillReward::GetPersonalLoot(loot.Entry))
                for (uint32 item : personal->Items)
                    if (Skyfire::KillReward::IsFlexibleItem(item) == flex)
                        items.push_back(item);
            return items;
        }

        uint32 lootId = Skyfire::BonusLoot::LootIdForDifficulty(loot.LootIdNormal, loot.LootIdHeroic, difficulty);
        LootStore const& store = loot.Source == Skyfire::BonusLoot::SOURCE_GAMEOBJECT ? LootTemplates_Gameobject : LootTemplates_Creature;
        if (LootTemplate const* lootTemplate = lootId ? store.GetLootFor(lootId) : nullptr)
            lootTemplate->CollectItemIds(items);

        return items;
    }
}

void Skyfire::BonusLoot::Load()
{
    uint32 oldMSTime = getMSTime();

    BonusLootStore.clear();

    //                                               0      1      2         3               4               5
    QueryResult result = WorldDatabase.Query("SELECT entry, spell, currency, loot_id_normal, loot_id_heroic, loot_source+0 FROM bonus_loot_template");
    if (!result)
    {
        SF_LOG_INFO("server.loading", ">> Loaded 0 bonus loot templates. DB table `bonus_loot_template` is empty.");
        return;
    }

    do
    {
        Field* fields = result->Fetch();
        BonusLootTemplate loot;
        loot.Entry = fields[0].GetUInt32();
        loot.Spell = fields[1].GetUInt32();
        loot.Currency = fields[2].GetUInt16();
        loot.LootIdNormal = fields[3].GetUInt32();
        loot.LootIdHeroic = fields[4].GetUInt32();
        loot.Source = uint8(fields[5].GetUInt32());

        if (!sObjectMgr->GetCreatureTemplate(loot.Entry))
        {
            SF_LOG_ERROR("sql.sql", "Table `bonus_loot_template` has a row for creature %u, which does not exist. Skipped.", loot.Entry);
            continue;
        }

        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(loot.Spell);
        if (!spellInfo || !spellInfo->HasAura(SPELL_AURA_BONUS_ROLL_TRIGGER))
        {
            SF_LOG_ERROR("sql.sql", "Table `bonus_loot_template` has spell %u for creature %u, which is no bonus roll trigger. Skipped.", loot.Spell, loot.Entry);
            continue;
        }

        if (!sCurrencyTypesStore.LookupEntry(loot.Currency))
        {
            SF_LOG_ERROR("sql.sql", "Table `bonus_loot_template` has currency %u for creature %u, which does not exist. Skipped.", loot.Currency, loot.Entry);
            continue;
        }

        if (loot.Source != SOURCE_CREATURE && loot.Source != SOURCE_GAMEOBJECT)
        {
            SF_LOG_ERROR("sql.sql", "Table `bonus_loot_template` has loot_source %u for creature %u, which is unknown. Skipped.", loot.Source, loot.Entry);
            continue;
        }

        BonusLootStore[loot.Entry] = loot;
    }
    while (result->NextRow());

    SF_LOG_INFO("server.loading", ">> Loaded %u bonus loot templates in %u ms", uint32(BonusLootStore.size()), GetMSTimeDiffToNow(oldMSTime));
}

BonusLootTemplate const* Skyfire::BonusLoot::Get(uint32 entry)
{
    auto itr = BonusLootStore.find(entry);
    return itr != BonusLootStore.end() ? &itr->second : nullptr;
}

BonusLootTemplate const* Skyfire::BonusLoot::GetForChestLoot(uint32 lootId)
{
    if (!lootId)
        return nullptr;

    for (auto const& itr : BonusLootStore)
        if (itr.second.Source == SOURCE_GAMEOBJECT && (itr.second.LootIdNormal == lootId || itr.second.LootIdHeroic == lootId))
            return &itr.second;

    return nullptr;
}

void Skyfire::BonusLoot::GrantPrompt(Player* player, BonusLootTemplate const& loot, uint8 difficulty, WorldObject* source)
{
    if (!player->HasCurrency(loot.Currency, 1) || player->HasLootLockout(LootLockoutType::BonusLoot, loot.Entry, difficulty) || player->HasAura(loot.Spell, source->GetGUID()))
        return;

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(loot.Spell);
    if (!spellInfo)
        return;

    int32 amounts[MAX_SPELL_EFFECTS] = { };
    for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        if (spellInfo->Effects[i].IsAura(SPELL_AURA_BONUS_ROLL_TRIGGER))
            amounts[i] = difficulty;

    // a new aura, never a refresh (checked above); the refresh path needs a unit caster, and a chest is none
    if (Aura* aura = Aura::TryCreate(spellInfo, MAX_EFFECT_MASK, player, source->ToUnit(), amounts, nullptr, source->GetGUID()))
        aura->ApplyForTargets();
}

BonusLootTemplate const* Skyfire::BonusLoot::GetForPrompt(uint64 casterGuid)
{
    if (IS_CRE_OR_VEH_GUID(casterGuid))
        return Get(GUID_ENPART(casterGuid));

    if (IS_GAMEOBJECT_GUID(casterGuid))
        if (GameObjectTemplate const* chest = sObjectMgr->GetGameObjectTemplate(GUID_ENPART(casterGuid)))
            return GetForChestLoot(chest->GetLootId());

    return nullptr;
}

bool Skyfire::BonusLoot::Roll(Player* player, BonusLootTemplate const& loot, uint8 difficulty)
{
    if (!player->HasCurrency(loot.Currency, 1))
        return false;

    // the lockout row is written at once, so a crash or relog cannot pay the roll twice
    if (!player->AddLootLockout(LootLockoutType::BonusLoot, loot.Entry, difficulty))
        return false;

    player->ModifyCurrency(loot.Currency, -1, true, true); // exactly one, no currency gain auras

    uint32 itemId = 0;
    if (roll_chance_f(sWorld->GetFloatConfig(WorldFloatConfigs::CONFIG_BONUS_ROLL_CHANCE)))
    {
        std::vector<uint32> fitting;
        for (uint32 item : GetRollItems(loot, difficulty))
            if (!IsExcludedItem(item) && player->IsItemFitToSpecialization(sObjectMgr->GetItemTemplate(item)))
                fitting.push_back(item);

        if (!fitting.empty())
            itemId = Skyfire::Containers::SelectRandomContainerElement(fitting);
    }

    Item* item = nullptr;
    if (itemId && Skyfire::KillReward::GiveLootItem(player, loot.Entry, itemId, "Bonus roll", &item))
    {
        SendBonusRollToast(player, ToastType::Item, itemId, item, 1);
        return true;
    }

    // a failed roll, no item for the player's specialization, or an item that could not be created pays gold
    uint32 moneyMin = sWorld->getIntConfig(WorldIntConfigs::CONFIG_BONUS_ROLL_MONEY_MIN);
    uint32 moneyMax = sWorld->getIntConfig(WorldIntConfigs::CONFIG_BONUS_ROLL_MONEY_MAX);
    uint32 money = urand(moneyMin, std::max(moneyMin, moneyMax));
    player->ModifyMoney(int64(money));
    SendBonusRollToast(player, ToastType::Money, 0, nullptr, money);
    return true;
}
