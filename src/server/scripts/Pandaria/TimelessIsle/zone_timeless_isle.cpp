/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Zone_Timeless_Isle
SD%Complete: 80
SDComment: Timeless Isle rare chests and zone spells. Each chest hands out once per character (weekly for
           the six chests whose tracking quest is weekly) through a tracking quest: a repeat attempt is
           refused (before the chest opens when clicked, by
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
#include "Creature.h"
#include "Guild.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"

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
enum TimelessSpells
{
    SPELL_BLOODY_COIN_IMMUNE    = 149624,
    SPELL_POUNCE_STUN           = 147651,
    SPELL_INSTAKILL_TOXIN       = 147656,
    GULP_FROG_TOXIN_MAX_STACKS  = 9,
    NPC_ZARHYM_SPIRIT           = 71869
};

// 148385, 148429 - Bloody Coin (Censer of Eternal Agony, Fire-Watcher's Oath): only procs on players
// that do not carry the Bloody Coin immunity.
class spell_bloody_coin_proc : public AuraScript
{
    PrepareAuraScript(spell_bloody_coin_proc);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Unit* target = eventInfo.GetProcTarget();
        if (!target || target->GetTypeId() != TypeID::TYPEID_PLAYER)
            return false;

        Unit* actionTarget = eventInfo.GetActionTarget();
        return !actionTarget || !actionTarget->HasAura(SPELL_BLOODY_COIN_IMMUNE);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_bloody_coin_proc::CheckProc);
    }
};

// 148385 - Censer of Eternal Agony: the wearer is flagged for PvP and gains no reputation while it lasts.
class spell_censer_of_eternal_agony : public AuraScript
{
    PrepareAuraScript(spell_censer_of_eternal_agony);

    bool Load() override
    {
        return GetOwner()->GetTypeId() == TypeID::TYPEID_PLAYER;
    }

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Player* player = GetOwner()->ToPlayer();
        player->SetFlag(UNIT_FIELD_FLAGS2, UNIT_FLAG2_IGNORE_REPUTATION);
        player->UpdatePvP(true, true);
    }

    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Player* player = GetOwner()->ToPlayer();
        player->RemoveFlag(UNIT_FIELD_FLAGS2, UNIT_FLAG2_IGNORE_REPUTATION);
        player->UpdatePvP(false);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_censer_of_eternal_agony::HandleApply, EFFECT_0, SPELL_AURA_MOD_FACTION, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_censer_of_eternal_agony::HandleRemove, EFFECT_0, SPELL_AURA_MOD_FACTION, AURA_EFFECT_HANDLE_REAL);
    }
};

// Casts the toxin's kill after the aura handler has returned, so the owner's auras are not removed mid-handler.
class GulpFrogToxinKillEvent : public BasicEvent
{
public:
    explicit GulpFrogToxinKillEvent(Unit* owner) : _owner(owner) { }

    bool Execute(uint64 /*time*/, uint32 /*diff*/) override
    {
        if (_owner->IsAlive())
            _owner->CastSpell(_owner, SPELL_INSTAKILL_TOXIN, true);
        return true;
    }

private:
    Unit* _owner;
};

// 147655 - Gulp Frog Toxin: the tenth stack kills the target.
class spell_gulp_frog_toxin : public AuraScript
{
    PrepareAuraScript(spell_gulp_frog_toxin);

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (GetStackAmount() > GULP_FROG_TOXIN_MAX_STACKS)
            if (Unit* owner = GetUnitOwner())
                owner->m_Events.AddEvent(new GulpFrogToxinKillEvent(owner), owner->m_Events.CalculateTime(0));
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_gulp_frog_toxin::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAPPLY);
    }
};

// 144145 - Spirit World: the owner shifts to phase 2, back to phase 1 on removal, and Zarhym's spirits vanish.
class spell_spirit_world : public AuraScript
{
    PrepareAuraScript(spell_spirit_world);

    void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        GetOwner()->SetPhaseMask(2, true);
    }

    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        WorldObject* owner = GetOwner();
        owner->SetPhaseMask(1, true);

        if (Player* player = owner->ToPlayer())
        {
            std::list<TempSummon*> summons = player->GetSummons();
            for (TempSummon* summon : summons)
                if (summon->GetEntry() == NPC_ZARHYM_SPIRIT)
                    summon->DespawnOrUnsummon();
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_spirit_world::HandleApply, EFFECT_0, SPELL_AURA_SCREEN_EFFECT, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_spirit_world::HandleRemove, EFFECT_0, SPELL_AURA_SCREEN_EFFECT, AURA_EFFECT_HANDLE_REAL);
    }
};

// 147650 - Defensive Leap: the cat's knock back hands its AI action 0.
class spell_cat_defensive_leap : public SpellScript
{
    PrepareSpellScript(spell_cat_defensive_leap);

    void HandleHit(SpellEffIndex /*effIndex*/)
    {
        if (Creature* caster = GetCaster()->ToCreature())
            if (caster->IsAIEnabled)
                caster->AI()->DoAction(0);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_cat_defensive_leap::HandleHit, EFFECT_0, SPELL_EFFECT_KNOCK_BACK);
    }
};

// 147649 - Pounce: the jump stuns the target it lands on.
class spell_timelesscat_pounce : public SpellScript
{
    PrepareSpellScript(spell_timelesscat_pounce);

    void HandleHit(SpellEffIndex /*effIndex*/)
    {
        if (Unit* caster = GetCaster())
            if (Unit* target = GetHitUnit())
                caster->CastSpell(target, SPELL_POUNCE_STUN, true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_timelesscat_pounce::HandleHit, EFFECT_0, SPELL_EFFECT_JUMP);
    }
};

// 147812 Time Rift, 147841 Time Lock: the destination lies in the water 10 to 30 yards behind the caster.
class spell_evermaw_time_portals : public SpellScript
{
    PrepareSpellScript(spell_evermaw_time_portals);

    void SelectDest(SpellDestination& dest)
    {
        Unit* caster = GetCaster();
        float dist = frand(10.0f, 30.0f);
        float angle = Position::NormalizeOrientation(caster->GetOrientation() - M_PI + frand(-M_PI / 2, M_PI / 2));
        Position pos;
        pos.Relocate(caster->GetPositionX() + dist * std::cos(angle), caster->GetPositionY() + dist * std::sin(angle),
            caster->GetPositionZ() - 10.0f + frand(-4.5f, 4.5f), 0.0f);
        dest.Relocate(pos);
    }

    void Register() override
    {
        OnDestinationTargetSelect += SpellDestinationTargetSelectFn(spell_evermaw_time_portals::SelectDest, EFFECT_0, TARGET_DEST_CASTER);
    }
};

enum TimelessLootSpells
{
    SPELL_TIMELESS_AGILITY      = 147355,
    SPELL_TIMELESS_INTELLECT    = 147357,
    SPELL_TIMELESS_STRENGTH     = 147359,
    SPELL_TIMELESS_STAMINA      = 147361
};

static bool IsTankSpec(uint32 spec)
{
    return spec == SPEC_WARRIOR_PROTECTION || spec == SPEC_PALADIN_PROTECTION || spec == SPEC_DEATH_KNIGHT_BLOOD
        || spec == SPEC_DRUID_GUARDIAN || spec == SPEC_MONK_BREWMASTER;
}

// Primary stat of a specialization, as the item stat type it is gear-matched on. 0 for an unknown spec.
static uint32 GetSpecPrimaryStat(uint32 spec)
{
    switch (spec)
    {
        case SPEC_WARRIOR_ARMS: case SPEC_WARRIOR_FURY: case SPEC_WARRIOR_PROTECTION:
        case SPEC_PALADIN_PROTECTION: case SPEC_PALADIN_RETRIBUTION:
        case SPEC_DEATH_KNIGHT_BLOOD: case SPEC_DEATH_KNIGHT_FROST: case SPEC_DEATH_KNIGHT_UNHOLY:
            return ITEM_MOD_STRENGTH;
        case SPEC_PALADIN_HOLY:
        case SPEC_PRIEST_DISCIPLINE: case SPEC_PRIEST_HOLY: case SPEC_PRIEST_SHADOW:
        case SPEC_SHAMAN_ELEMENTAL: case SPEC_SHAMAN_RESTORATION:
        case SPEC_MAGE_ARCANE: case SPEC_MAGE_FIRE: case SPEC_MAGE_FROST:
        case SPEC_WARLOCK_AFFLICTION: case SPEC_WARLOCK_DEMONOLOGY: case SPEC_WARLOCK_DESTRUCTION:
        case SPEC_DRUID_BALANCE: case SPEC_DRUID_RESTORATION:
        case SPEC_MONK_MISTWEAVER:
            return ITEM_MOD_INTELLECT;
        case SPEC_HUNTER_BEAST_MASTERY: case SPEC_HUNTER_MARKSMANSHIP: case SPEC_HUNTER_SURVIVAL:
        case SPEC_ROGUE_ASSASSINATION: case SPEC_ROGUE_COMBAT: case SPEC_ROGUE_SUBTLETY:
        case SPEC_SHAMAN_ENHANCEMENT:
        case SPEC_DRUID_FERAL: case SPEC_DRUID_GUARDIAN:
        case SPEC_MONK_BREWMASTER: case SPEC_MONK_WINDWALKER:
            return ITEM_MOD_AGILITY;
        default:
            return 0;
    }
}

static uint32 GetCurrentSpec(Player* player)
{
    return player->GetLootSpecialization() ? player->GetLootSpecialization() : player->GetTalentSpecialization(player->GetActiveSpec());
}

// This core has no item specialization table, so the spec match goes by the primary stat of each item
// (first stat of the item template) against the primary stat of the player's loot or active spec. Falls
// back to any item the player can use, then to any item of the list.
static uint32 PickTimelessItem(Player* player, std::vector<uint32> const& items)
{
    uint32 stat = GetSpecPrimaryStat(GetCurrentSpec(player));
    std::vector<uint32> matching, usable;
    for (uint32 itemId : items)
    {
        ItemTemplate const* proto = sObjectMgr->GetItemTemplate(itemId);
        if (!proto || player->CanUseItem(proto) != EQUIP_ERR_OK)
            continue;

        usable.push_back(itemId);
        if (stat && proto->ItemStat[0].ItemStatType == stat)
            matching.push_back(itemId);
    }

    std::vector<uint32> const& pool = !matching.empty() ? matching : (!usable.empty() ? usable : items);
    return Skyfire::Containers::SelectRandomContainerElement(pool);
}

static bool GiveTimelessItem(Player* player, std::vector<uint32> const& items)
{
    uint32 itemId = PickTimelessItem(player, items);
    if (!player->AddItem(itemId, 1))
        return false;

    if (Guild* guild = player->GetGuild())
        guild->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_CRAFT_ITEMS_GUILD, itemId, 1, 0, nullptr, player);
    return true;
}

// The create-item effect is replaced by the script, so the core does not check the bag space: a full bag
// would spend the item or cache and give nothing back.
static SpellCastResult CheckTimelessBagSpace(Unit* caster, std::vector<uint32> const& items)
{
    Player* player = caster->ToPlayer();
    if (!player || items.empty())
        return SpellCastResult::SPELL_CAST_OK;

    ItemPosCountVec dest;
    InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, items.front(), 1);
    if (msg == EQUIP_ERR_OK)
        return SpellCastResult::SPELL_CAST_OK;

    player->SendEquipError(msg, nullptr, nullptr, items.front());
    return SpellCastResult::SPELL_FAILED_DONT_REPORT;
}

static std::map<uint32, std::vector<uint32>> const timelessLoot =
{
    { 146259, { 101838, 101830 } },
    { 146265, { 101843, 101835 } },
    { 146243, { 101799, 101861, 101826 } },
    { 146267, { 101839, 101831 } },
    { 146268, { 101869, 101788 } },
    { 146277, { 101807, 101816 } },
    { 146263, { 101840, 101832 } },
    { 146270, { 101867, 101786 } },
    { 146239, { 101791, 101853, 101820 } },
    { 146283, { 101810, 101818 } },
    { 146271, { 101865, 101784 } },
    { 146264, { 101842, 101834 } },
    { 146272, { 101866, 101785 } },
    { 146237, { 101797, 101859, 101825 } },
    { 146275, { 101862, 101781 } },
    { 146279, { 101803, 101813 } },
    { 146261, { 101845, 101837 } },
    { 146236, { 101792, 101854, 101821 } },
    { 148740, { 101805, 101795, 101857, 101849, 101827 } },
    { 148746, { 103689, 103688, 103686, 103690, 103687 } },
    { 146246, { 101800, 101790, 101852, 101848, 101828 } },
    { 146274, { 101868, 101787 } },
    { 146269, { 101863, 101782 } },
    { 146260, { 101841, 101833 } },
    { 146280, { 101802, 101812 } },
    { 146266, { 101844, 101836 } },
    { 146273, { 101864, 101783 } },
    { 146278, { 101806, 101815 } },
    { 146282, { 101808, 101817 } },
    { 146281, { 101804, 101814 } },
    { 146276, { 101801, 101811 } },
    { 146240, { 101793, 101855, 101822 } },
    { 146241, { 101794, 101856, 101823 } },
    { 146238, { 101789, 101851, 101819 } },
    { 146242, { 101796, 101858, 101824 } },
    { 146244, { 101809, 101798, 101860, 101850, 101829 } },
};

static std::map<uint32, std::vector<uint32>> const timelessCacheLoot =
{
    { 147597, { 101789, 101791, 101792, 101793, 101794, 101796, 101797, 101799, 101819, 101820, 101821, 101822, 101823, 101824, 101825, 101826, 101851, 101853, 101854, 101855, 101856, 101858, 101859, 101861 } },
    { 148099, { 101830, 101831, 101832, 101833, 101834, 101835, 101836, 101837, 101838, 101839, 101840, 101841, 101842, 101843, 101844, 101845 } },
    { 148103, { 101781, 101782, 101783, 101784, 101785, 101786, 101787, 101788, 101862, 101863, 101864, 101865, 101866, 101867, 101868, 101869 } },
    { 148104, { 101801, 101802, 101803, 101804, 101806, 101807, 101808, 101810, 101811, 101812, 101813, 101814, 101815, 101816, 101817, 101818 } },
};

static std::map<uint32, std::vector<uint32>> const embracedTimelessLoot =
{
    { 146259, { 101927, 101919 } },
    { 146265, { 101932, 101924 } },
    { 146243, { 101915, 101948, 101888 } },
    { 146267, { 101928, 101920 } },
    { 146268, { 101877, 101956 } },
    { 146277, { 101896, 101905 } },
    { 146263, { 101929, 101921 } },
    { 146270, { 101875, 101954 } },
    { 146239, { 101909, 101940, 101880 } },
    { 146283, { 101899, 101907 } },
    { 146271, { 101952, 101873 } },
    { 146264, { 101923, 101931 } },
    { 146272, { 101874, 101953 } },
    { 146237, { 101914, 101946, 101886 } },
    { 146275, { 101949, 101870 } },
    { 146279, { 101892, 101902 } },
    { 146261, { 101926, 101934 } },
    { 146236, { 101910, 101941, 101881 } },
    { 148740, { 101936, 101894, 101916, 101944, 101884 } },
    { 146246, { 101917, 101889, 101939, 101879, 101935 } },
    { 146274, { 101955, 101876 } },
    { 146269, { 101950, 101871 } },
    { 146260, { 101922, 101930 } },
    { 146280, { 101901, 101891 } },
    { 146266, { 101925, 101933 } },
    { 146273, { 101951, 101872 } },
    { 146278, { 101904, 101895 } },
    { 146282, { 101897, 101906 } },
    { 146281, { 101893, 101903 } },
    { 146276, { 101890, 101900 } },
    { 146240, { 101882, 101911, 101942 } },
    { 146241, { 101883, 101912, 101943 } },
    { 146238, { 101938, 101878, 101908 } },
    { 146242, { 101913, 101945, 101885 } },
    { 146244, { 101887, 101898, 101937, 101918, 101947 } },
};

// Timeless Isle gear spells: the create-item effect hands out the item that suits the caster's spec.
class spell_create_timeless_item : public SpellScript
{
    PrepareSpellScript(spell_create_timeless_item);

    SpellCastResult CheckCast()
    {
        auto itr = timelessLoot.find(GetSpellInfo()->Id);
        return itr != timelessLoot.end() ? CheckTimelessBagSpace(GetCaster(), itr->second) : SpellCastResult::SPELL_CAST_OK;
    }

    void HandleCreateItem(SpellEffIndex effIndex)
    {
        PreventHitEffect(effIndex);

        if (Player* player = GetCaster()->ToPlayer())
        {
            auto itr = timelessLoot.find(GetSpellInfo()->Id);
            if (itr != timelessLoot.end())
                GiveTimelessItem(player, itr->second);
        }
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_create_timeless_item::CheckCast);
        OnEffectHitTarget += SpellEffectFn(spell_create_timeless_item::HandleCreateItem, EFFECT_0, SPELL_EFFECT_CREATE_ITEM_2);
    }
};

// Timeless cache spells: a random item of the cache's armor type and slot group, spec-matched.
class spell_create_timeless_item_from_cache : public SpellScript
{
    PrepareSpellScript(spell_create_timeless_item_from_cache);

    SpellCastResult CheckCast()
    {
        auto itr = timelessCacheLoot.find(GetSpellInfo()->Id);
        return itr != timelessCacheLoot.end() ? CheckTimelessBagSpace(GetCaster(), itr->second) : SpellCastResult::SPELL_CAST_OK;
    }

    void HandleCreateItem(SpellEffIndex effIndex)
    {
        PreventHitEffect(effIndex);

        if (Player* player = GetCaster()->ToPlayer())
        {
            auto itr = timelessCacheLoot.find(GetSpellInfo()->Id);
            if (itr != timelessCacheLoot.end())
                GiveTimelessItem(player, itr->second);
        }
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_create_timeless_item_from_cache::CheckCast);
        OnEffectHitTarget += SpellEffectFn(spell_create_timeless_item_from_cache::HandleCreateItem, EFFECT_0, SPELL_EFFECT_CREATE_ITEM_2);
    }
};

// 147678 - Eternity's Embrace: trades a Timeless item for the matching embraced version.
class spell_eternitys_embrace : public SpellScript
{
    PrepareSpellScript(spell_eternitys_embrace);

    static std::vector<uint32> const* FindLoot(Item* item)
    {
        if (!item || !item->GetTemplate()->Spells[0].SpellId)
            return NULL;

        auto itr = embracedTimelessLoot.find(item->GetTemplate()->Spells[0].SpellId);
        return itr != embracedTimelessLoot.end() ? &itr->second : NULL;
    }

    SpellCastResult CheckCast()
    {
        return FindLoot(GetExplTargetItem()) ? SpellCastResult::SPELL_CAST_OK : SpellCastResult::SPELL_FAILED_BAD_TARGETS;
    }

    void HandleAfterCast()
    {
        Player* player = GetCaster()->ToPlayer();
        Item* item = GetExplTargetItem();
        std::vector<uint32> const* loot = FindLoot(item);
        if (!player || !loot)
            return;

        // Give first: when the bags are full or the item is refused, the Timeless item stays. The targeted
        // item goes by its position, so a second copy of the same entry is not taken instead.
        uint8 bag = item->GetBagSlot();
        uint8 slot = item->GetSlot();
        if (GiveTimelessItem(player, *loot))
            player->DestroyItem(bag, slot, true);
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_eternitys_embrace::CheckCast);
        AfterCast += SpellCastFn(spell_eternitys_embrace::HandleAfterCast);
    }
};

// 147226 - Book of the Ages: procs the stat buff of the owner's role.
class spell_book_of_the_ages : public AuraScript
{
    PrepareAuraScript(spell_book_of_the_ages);

    void HandleProc(ProcEventInfo& /*eventInfo*/)
    {
        Player* player = GetOwner()->ToPlayer();
        if (!player)
            return;

        uint32 spec = player->GetTalentSpecialization(player->GetActiveSpec());
        uint32 spell = SPELL_TIMELESS_STAMINA;
        if (!IsTankSpec(spec))
        {
            switch (GetSpecPrimaryStat(spec))
            {
                case ITEM_MOD_STRENGTH:     spell = SPELL_TIMELESS_STRENGTH; break;
                case ITEM_MOD_INTELLECT:    spell = SPELL_TIMELESS_INTELLECT; break;
                case ITEM_MOD_AGILITY:      spell = SPELL_TIMELESS_AGILITY; break;
                default: break;
            }
        }

        player->CastSpell(player, spell, true);
    }

    void Register() override
    {
        OnProc += AuraProcFn(spell_book_of_the_ages::HandleProc);
    }
};

void AddSC_zone_timeless_isle()
{
    new go_timeless_chest();

    RegisterAuraScript(spell_bloody_coin_proc);
    RegisterAuraScript(spell_censer_of_eternal_agony);
    RegisterAuraScript(spell_gulp_frog_toxin);
    RegisterAuraScript(spell_spirit_world);
    RegisterSpellScript(spell_cat_defensive_leap);
    RegisterSpellScript(spell_timelesscat_pounce);
    RegisterSpellScript(spell_evermaw_time_portals);
    RegisterSpellScript(spell_create_timeless_item);
    RegisterSpellScript(spell_create_timeless_item_from_cache);
    RegisterSpellScript(spell_eternitys_embrace);
    RegisterAuraScript(spell_book_of_the_ages);
}
