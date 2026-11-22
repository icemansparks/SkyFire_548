/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef CELESTIAL_COURT_H
#define CELESTIAL_COURT_H

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "GameObject.h"
#include "Player.h"
#include "ThreatManager.h"

// Timeless Isle Celestial Court: Emperor Shaohao calls one celestial at a time to the middle of
// the court, the celestial turns hostile after an intro, and on defeat it turns friendly again,
// Shaohao speaks the outro and the next celestial is called.
enum CelestialCourt
{
    NPC_CHI_JI                  = 71952,
    NPC_XUEN                    = 71953,
    NPC_NIUZAO                  = 71954,
    NPC_YU_LON                  = 71955,
    NPC_EMPEROR_SHAOHAO         = 73303,

    GO_CELESTIAL_DOOR           = 223501,

    FACTION_CELESTIAL_FRIENDLY  = 35,
    FACTION_CELESTIAL_HOSTILE   = 31,

    ACTION_SELECT_CELESTIAL     = 0,
    ACTION_CELESTIAL_DONE       = 1,
    POINT_COURT_MIDDLE          = 1,

    CELESTIAL_INTRO_DELAY       = 1000,
    CELESTIAL_ATTACKABLE_DELAY  = 15000,
    CELESTIAL_OUTRO_DELAY       = 10000,
    CELESTIAL_DESPAWN_DELAY     = 13000
};

// creature_text groups of Emperor Shaohao (73303).
enum ShaohaoTexts
{
    SHAOHAO_INTRO_YU_LON        = 0,
    SHAOHAO_INTRO_XUEN          = 1,
    SHAOHAO_INTRO_CHI_JI        = 2,
    SHAOHAO_INTRO_NIUZAO        = 3,
    SHAOHAO_OUTRO_YU_LON        = 4,
    SHAOHAO_OUTRO_XUEN          = 5,
    SHAOHAO_OUTRO_CHI_JI        = 6,
    SHAOHAO_OUTRO_NIUZAO        = 7
};

Position const CourtMiddle = { -650.04f, -5016.84f, -4.27f, 1.573f };
float const COURT_FACING = 1.573f;

// Celestials that have a script, in the order Shaohao may call them. Each celestial PR adds its
// entry here: a celestial without a script would walk to the middle and stand there.
uint32 const CelestialsInRotation[] = { NPC_XUEN, NPC_CHI_JI };

// Players whose class fights from range; stand-in for alexkulya's caster-spec lookup (SkyFire
// has no talent role lookup).
inline bool IsRangedClass(Unit const* unit)
{
    switch (unit->getClass())
    {
        case CLASS_MAGE:
        case CLASS_WARLOCK:
        case CLASS_PRIEST:
            return true;
        default:
            return false;
    }
}

struct RangedPlayerSelector
{
    bool operator()(Unit const* target) const
    {
        return target->GetTypeId() == TypeID::TYPEID_PLAYER && IsRangedClass(target);
    }
};

inline void HandleCelestialDoor(Creature* me, bool open)
{
    std::list<GameObject*> doors;
    me->GetGameObjectListWithEntryInGrid(doors, GO_CELESTIAL_DOOR, 300.0f);
    for (std::list<GameObject*>::iterator itr = doors.begin(); itr != doors.end(); ++itr)
        (*itr)->SetGoState(open ? GOState::GO_STATE_ACTIVE : GOState::GO_STATE_READY);
}

// The fight skeleton all four celestials share. A subclass schedules and runs its abilities.
struct CelestialAI : public ScriptedAI
{
    CelestialAI(Creature* creature, uint8 outroText, uint8 aggroText, uint8 introText, uint8 deathText, uint8 killText)
        : ScriptedAI(creature), summons(creature), outroText(outroText), aggroText(aggroText),
          introText(introText), deathText(deathText), killText(killText), defeated(false)
    {
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void Reset() OVERRIDE
    {
        events.Reset();
        summons.DespawnAll();
        me->SetWalk(true);
        me->setActive(true);
        me->ResetPlayerDamageReq();
        me->SetLootRecipient(NULL);
        if (me->getFaction() == FACTION_CELESTIAL_HOSTILE)
            me->SetFacingTo(COURT_FACING);
        HandleCelestialDoor(me, true);
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        me->SetWalk(false);
        defeated = false;
        Talk(aggroText);
        HandleCelestialDoor(me, false);
        ScheduleCombatEvents();
    }

    // Shaohao's call: the celestial walks to the middle, turns hostile, speaks, and a moment
    // later can be attacked.
    void MovementInform(uint32 type, uint32 pointId) OVERRIDE
    {
        if (type != POINT_MOTION_TYPE || pointId != POINT_COURT_MIDDLE)
            return;

        nonCombatEvents.ScheduleEvent(EVENT_CELESTIAL_INTRO, CELESTIAL_INTRO_DELAY);
        me->SetFacingTo(COURT_FACING);
        me->setFaction(FACTION_CELESTIAL_HOSTILE);
        me->SetHomePosition(CourtMiddle);
    }

    void KilledUnit(Unit* victim) OVERRIDE
    {
        if (victim->GetTypeId() == TypeID::TYPEID_PLAYER)
            Talk(killText);
    }

    void JustSummoned(Creature* summon) OVERRIDE
    {
        summons.Summon(summon);
    }

    void SummonedCreatureDespawn(Creature* summon) OVERRIDE
    {
        summons.Despawn(summon);
    }

    void EnterEvadeMode() OVERRIDE
    {
        ScriptedAI::EnterEvadeMode();
        summons.DespawnAll();
        events.Reset();
        me->SetLootRecipient(NULL);
        HandleCelestialDoor(me, true);

        // A wipe ends the fight like a defeat without credit: the celestial turns friendly and
        // unattackable again and respawns, so Shaohao can call the next one.
        me->setFaction(FACTION_CELESTIAL_FRIENDLY);
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        nonCombatEvents.ScheduleEvent(EVENT_CELESTIAL_DESPAWN, 1000);
    }

    // A celestial is never killed: the lethal hit starts the defeat sequence instead.
    void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
    {
        if (defeated)
        {
            damage = 0;
            return;
        }

        if (!me->hasLootRecipient())
            me->SetLootRecipient(attacker);

        if (damage < me->GetHealth())
            return;

        defeated = true;
        damage = 0;
        Talk(deathText);

        // Kill credit for everyone on the threat list.
        ThreatContainer::StorageType const& threatList = me->getThreatManager().getThreatList();
        for (ThreatContainer::StorageType::const_iterator itr = threatList.begin(); itr != threatList.end(); ++itr)
            if (Player* player = ObjectAccessor::GetPlayer(*me, (*itr)->getUnitGuid()))
                player->KilledMonsterCredit(me->GetEntry());

        me->setFaction(FACTION_CELESTIAL_FRIENDLY);
        me->StopMoving();
        me->RemoveAllAuras();
        me->GetMotionMaster()->Clear();
        me->CombatStop(true);
        me->SetFullHealth();
        me->SetFacingTo(COURT_FACING);
        me->DeleteThreatList();
        summons.DespawnAll();
        events.Reset();

        nonCombatEvents.ScheduleEvent(EVENT_CELESTIAL_OUTRO, CELESTIAL_OUTRO_DELAY);
        nonCombatEvents.ScheduleEvent(EVENT_CELESTIAL_DESPAWN, CELESTIAL_DESPAWN_DELAY);
        HandleCelestialDoor(me, true);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        nonCombatEvents.Update(diff);

        while (uint32 eventId = nonCombatEvents.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_CELESTIAL_INTRO:
                    Talk(introText);
                    nonCombatEvents.ScheduleEvent(EVENT_CELESTIAL_ATTACKABLE, CELESTIAL_ATTACKABLE_DELAY);
                    break;
                case EVENT_CELESTIAL_ATTACKABLE:
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    me->SetFullHealth();
                    break;
                case EVENT_CELESTIAL_OUTRO:
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    if (Creature* shaohao = me->FindNearestCreature(NPC_EMPEROR_SHAOHAO, 300.0f, true))
                        shaohao->AI()->Talk(outroText);
                    break;
                case EVENT_CELESTIAL_DESPAWN:
                    DespawnForRespawn();
                    break;
                default:
                    break;
            }
        }

        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        // One event per tick, so a second cast is not dropped while the first is casting.
        if (uint32 eventId = events.ExecuteEvent())
            ExecuteCombatEvent(eventId);

        DoMeleeAttackIfReady();
    }

protected:
    virtual void ScheduleCombatEvents() { }
    virtual void ExecuteCombatEvent(uint32 /*eventId*/) { }

    EventMap events;
    SummonList summons;

private:
    enum CelestialEvents
    {
        EVENT_CELESTIAL_INTRO = 1,
        EVENT_CELESTIAL_ATTACKABLE,
        EVENT_CELESTIAL_OUTRO,
        EVENT_CELESTIAL_DESPAWN
    };

    // Tells Shaohao this round is over, then despawns with a short respawn so the
    // celestial is back at its own spawn point for the next round.
    void DespawnForRespawn()
    {
        // Shaohao calls the next celestial only after this one has respawned, or he would find
        // this one while it is still alive.
        if (Creature* shaohao = me->FindNearestCreature(NPC_EMPEROR_SHAOHAO, 500.0f, true))
            shaohao->AI()->DoAction(ACTION_CELESTIAL_DONE);

        uint32 corpseDelay = me->GetCorpseDelay();
        uint32 respawnDelay = me->GetRespawnDelay();

        me->SetCorpseDelay(1);
        me->SetRespawnDelay(4);
        defeated = false;
        me->DespawnOrUnsummon();
        me->SetCorpseDelay(corpseDelay);
        me->SetRespawnDelay(respawnDelay);
    }

    EventMap nonCombatEvents;
    uint8 outroText;
    uint8 aggroText;
    uint8 introText;
    uint8 deathText;
    uint8 killText;
    bool defeated;
};

#endif
