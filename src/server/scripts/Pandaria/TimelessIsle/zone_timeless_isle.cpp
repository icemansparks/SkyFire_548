/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Zone_Timeless_Isle
SD%Complete: 80
SDComment: Timeless Isle rare chests only. Each chest hands out once per character (weekly for the six
           chests whose tracking quest is weekly) through a tracking quest: a repeat attempt is refused (before the chest opens when clicked, by
           emptying it when opened through the loot opcode), the first one rewards the quest and
           counts for the Timeless Treasures credit. A second player joining an already open chest
           gets neither the lockout nor the credit.
SDCategory: Timeless Isle
EndScriptData */

#include "ScriptMgr.h"
#include "GameObject.h"
#include "ObjectMgr.h"
#include "Player.h"
#include <mutex>
#include <map>

enum TimelessChest
{
    GO_MIST_COVERED_TREASURE_CHEST  = 220908,
    SPELL_MIST_COVERED_KEY          = 144385,
    NPC_TIMELESS_TREASURES_CREDIT   = 73982
};

// Chest entry -> tracking quest that limits the chest to one pickup per character; the quest's own
// weekly flag decides whether the limit resets (six chests) or holds for good (the other 43).
static std::map<uint32, uint32> const timelessRareChests =
{
    { 220832, 32957 },   // GO_SUNKEN_TREASURE
    { 220901, 32969 },   // GO_GLEAMING_TREASURE_CHEST
    { 220902, 32968 },   // GO_ROPE_BOUND_TREASURE_CHEST
    { 220908, 32971 },   // GO_MIST_COVERED_TREASURE_CHEST
    { 220986, 32956 },   // GO_BLACKGUARDS_JETSAM
    { 221036, 32970 },   // GO_GLEAMING_TREASURE_SATCHEL
    { 221617, 33203 },   // GO_SKULL_COVERED_CHEST
    { 221671, 33204 },   // GO_STURDY_CHEST_1
    { 223116, 33205 },   // GO_STURDY_CHEST_2
    { 223117, 33206 },   // GO_STURDY_CHEST_3
    { 223118, 33207 },   // GO_STURDY_CHEST_4
    { 221672, 33208 },   // GO_SMOLDERING_CHEST_1
    { 223119, 33209 },   // GO_SMOLDERING_CHEST_2
    { 221673, 33210 },   // GO_BLAZING_CHEST
    { 221670, 33170 },   // GO_MOSS_COVERED_CHEST_1
    { 223084, 33171 },   // GO_MOSS_COVERED_CHEST_2
    { 223085, 33172 },   // GO_MOSS_COVERED_CHEST_3
    { 223086, 33173 },   // GO_MOSS_COVERED_CHEST_4
    { 223087, 33174 },   // GO_MOSS_COVERED_CHEST_5
    { 223088, 33175 },   // GO_MOSS_COVERED_CHEST_6
    { 223089, 33176 },   // GO_MOSS_COVERED_CHEST_7
    { 223090, 33177 },   // GO_MOSS_COVERED_CHEST_8
    { 223091, 33178 },   // GO_MOSS_COVERED_CHEST_9
    { 223092, 33179 },   // GO_MOSS_COVERED_CHEST_10
    { 223093, 33180 },   // GO_MOSS_COVERED_CHEST_11
    { 223094, 33181 },   // GO_MOSS_COVERED_CHEST_12
    { 223095, 33182 },   // GO_MOSS_COVERED_CHEST_13
    { 223096, 33183 },   // GO_MOSS_COVERED_CHEST_14
    { 223097, 33184 },   // GO_MOSS_COVERED_CHEST_15
    { 223098, 33185 },   // GO_MOSS_COVERED_CHEST_16
    { 223099, 33186 },   // GO_MOSS_COVERED_CHEST_17
    { 223100, 33187 },   // GO_MOSS_COVERED_CHEST_18
    { 223101, 33188 },   // GO_MOSS_COVERED_CHEST_19
    { 223102, 33189 },   // GO_MOSS_COVERED_CHEST_20
    { 223103, 33190 },   // GO_MOSS_COVERED_CHEST_21
    { 223104, 33191 },   // GO_MOSS_COVERED_CHEST_22
    { 223105, 33192 },   // GO_MOSS_COVERED_CHEST_23
    { 223106, 33193 },   // GO_MOSS_COVERED_CHEST_24
    { 223107, 33194 },   // GO_MOSS_COVERED_CHEST_25
    { 223108, 33195 },   // GO_MOSS_COVERED_CHEST_26
    { 223109, 33196 },   // GO_MOSS_COVERED_CHEST_27
    { 223110, 33197 },   // GO_MOSS_COVERED_CHEST_28
    { 223111, 33198 },   // GO_MOSS_COVERED_CHEST_29
    { 223112, 33199 },   // GO_MOSS_COVERED_CHEST_30
    { 223113, 33200 },   // GO_MOSS_COVERED_CHEST_31
    { 223114, 33201 },   // GO_MOSS_COVERED_CHEST_32
    { 223115, 33202 },   // GO_MOSS_COVERED_CHEST_33
    { 223204, 33227 },   // GO_MOSS_COVERED_CHEST_34
    { 220807, 32961 },   // GO_NEVERENDING_SPRITEWOOD
};

class go_timeless_chest : public GameObjectScript
{
    public:
        go_timeless_chest() : GameObjectScript("go_timeless_chest") { }

        // First line of defence for a click on a closed chest: refused before it opens, so the chest
        // stays closed and lootable for everyone else. The player who opened a chest passes on later
        // clicks, so they can finish the leftovers; anyone else clicking an open chest meets the same
        // checks as on a closed one.
        bool OnGossipHello(Player* player, GameObject* go) override
        {
            if (go->getLootState() != LootState::GO_READY && IsOpener(player, go))
                return false;

            if (CanOpen(player, go))
                return false;

            player->SendGameError(GameError::ERR_LOOT_CANT_LOOT_THAT);
            return true;
        }

        void OnLootStateChanged(GameObject* go, uint32 state, Unit* unit) override
        {
            // Any other state ends an opening: forget it, so the next opening counts as the first one.
            if (state != uint32(LootState::GO_ACTIVATED))
            {
                std::lock_guard<std::mutex> lock(openedChestsLock);
                openedChests.erase(go->GetGUID());
                return;
            }

            if (!unit || unit->GetTypeId() != TypeID::TYPEID_PLAYER)
                return;

            Player* player = unit->ToPlayer();

            // The core sets this state again when a player closes the loot window with items left in
            // the chest, and a second player may open a chest that is already open: only the first
            // opening is counted, a later call never rewards, locks out or empties anything.
            {
                std::lock_guard<std::mutex> lock(openedChestsLock);
                if (openedChests.find(go->GetGUID()) != openedChests.end())
                    return;
            }

            // The client may open a chest with the loot opcode, which never reaches OnGossipHello.
            // The loot window is sent after this hook returns, so the loot is emptied first, and the
            // chest closes the way the core closes an emptied chest. The close is visible to nearby
            // players for the respawn time; accepted by the owner (2026-11-13) to keep the
            // limit strict on every opening path.
            if (!CanOpen(player, go))
            {
                go->loot.clear();
                player->SendGameError(GameError::ERR_LOOT_CANT_LOOT_THAT);
                go->ForcedDespawn();
                return;
            }

            {
                std::lock_guard<std::mutex> lock(openedChestsLock);
                openedChests[go->GetGUID()] = player->GetGUID();
            }

            if (Quest const* quest = GetTrackingQuest(go->GetEntry()))
                player->RewardQuest(quest, 0, player, false);

            player->KilledMonsterCredit(NPC_TIMELESS_TREASURES_CREDIT);

            // Close a chest nobody finished looting so it does not stay open.
            go->m_Events.Schedule(MINUTE * IN_MILLISECONDS, [go]
            {
                if (go->getLootState() == LootState::GO_ACTIVATED)
                    go->SetLootState(LootState::GO_JUST_DEACTIVATED);
            });
        }

    private:
        // Chests (by guid) whose current opening already counted, with the player who opened them.
        static std::map<uint64, uint64> openedChests;
        static std::mutex openedChestsLock;

        static Quest const* GetTrackingQuest(uint32 chestEntry)
        {
            std::map<uint32, uint32>::const_iterator itr = timelessRareChests.find(chestEntry);
            return itr != timelessRareChests.end() ? sObjectMgr->GetQuestTemplate(itr->second) : NULL;
        }

        static bool CanOpen(Player* player, GameObject* go)
        {
            // The mist covered chest needs the aura from the crane platform route.
            if (go->GetEntry() == GO_MIST_COVERED_TREASURE_CHEST && !player->HasAura(SPELL_MIST_COVERED_KEY))
                return false;

            Quest const* quest = GetTrackingQuest(go->GetEntry());
            // A weekly quest stays in the rewarded list across the weekly reset, so SatisfyQuestWeek decides
            // for those. The one-time chests go by the rewarded list itself: GetQuestStatus would read
            // none for a quest template flagged repeatable.
            if (!quest)
                return true;

            return quest->IsWeekly() ? player->SatisfyQuestWeek(quest, false) : !player->IsQuestRewarded(quest->GetQuestId());
        }

        static bool IsOpener(Player* player, GameObject* go)
        {
            std::lock_guard<std::mutex> lock(openedChestsLock);
            std::map<uint64, uint64>::const_iterator itr = openedChests.find(go->GetGUID());
            return itr != openedChests.end() && itr->second == player->GetGUID();
        }
};

std::map<uint64, uint64> go_timeless_chest::openedChests;
std::mutex go_timeless_chest::openedChestsLock;

void AddSC_zone_timeless_isle()
{
    new go_timeless_chest();
}
