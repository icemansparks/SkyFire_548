/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_Galleon
SD%Complete: 80
SDComment: Valley of the Four Winds world boss. The crew stands on the galleon as
           static spawns instead of riding its vehicle seats; the cannons stay cosmetic.
SDCategory: Valley of the Four Winds
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "Player.h"

enum GalleonSpells
{
    SPELL_STOMP             = 121787,
    SPELL_CANNON_BARRAGE    = 121577,
    SPELL_FIRE_SHOT         = 121673,
    SPELL_IMPALING_PULL     = 121747,
    SPELL_BERSERK           = 47008
};

enum GalleonEvents
{
    EVENT_STOMP             = 1,
    EVENT_CANNON_BARRAGE,
    EVENT_WARMONGERS,
    EVENT_BERSERK,
    EVENT_FIRE_SHOT,
    EVENT_IMPALING_PULL,
    EVENT_CHECK_AREA
};

enum GalleonActions
{
    ACTION_CREW_START       = 1,
    ACTION_CREW_STOP        = 2
};

enum GalleonCreatures
{
    NPC_SALYIN_SKIRMISHER   = 62350,
    NPC_SALYIN_WARMONGER    = 62351
};

enum GalleonMisc
{
    WARMONGERS_PER_WAVE     = 6
};

float const CREW_SEARCH_RANGE   = 60.0f;
float const COMBAT_AREA_RANGE   = 125.0f;
float const PLAYER_SEARCH_RANGE = 100.0f;

class boss_galion : public CreatureScript
{
public:
    boss_galion() : CreatureScript("boss_galion") { }

    struct boss_galionAI : public ScriptedAI
    {
        boss_galionAI(Creature* creature) : ScriptedAI(creature), summons(creature) { }

        void Reset() OVERRIDE
        {
            events.Reset();
            summons.DespawnAll();
            SignalCrew(ACTION_CREW_STOP);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            events.ScheduleEvent(EVENT_CANNON_BARRAGE, 25 * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_STOMP, 50 * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_WARMONGERS, MINUTE * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_BERSERK, 15 * MINUTE * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_CHECK_AREA, 2 * IN_MILLISECONDS);
            SignalCrew(ACTION_CREW_START);
        }

        void JustSummoned(Creature* summon) OVERRIDE
        {
            summons.Summon(summon);
        }

        void SummonedCreatureDespawn(Creature* summon) OVERRIDE
        {
            summons.Despawn(summon);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            events.Reset();
            summons.DespawnAll();
            SignalCrew(ACTION_CREW_STOP);
        }

        // The skirmishers on deck are static spawns, so they are found by range, not by seat.
        void SignalCrew(int32 action)
        {
            std::list<Creature*> crew;
            GetCreatureListWithEntryInGrid(crew, me, NPC_SALYIN_SKIRMISHER, CREW_SEARCH_RANGE);
            for (std::list<Creature*>::const_iterator itr = crew.begin(); itr != crew.end(); ++itr)
                if ((*itr)->IsAlive() && (*itr)->AI())
                    (*itr)->AI()->DoAction(action);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_STOMP:
                        DoCast(me, SPELL_STOMP, true);
                        events.ScheduleEvent(EVENT_STOMP, MINUTE * IN_MILLISECONDS);
                        break;
                    case EVENT_CANNON_BARRAGE:
                        DoCast(me, SPELL_CANNON_BARRAGE);
                        events.ScheduleEvent(EVENT_CANNON_BARRAGE, MINUTE * IN_MILLISECONDS);
                        break;
                    case EVENT_WARMONGERS:
                        for (uint8 i = 0; i < WARMONGERS_PER_WAVE; ++i)
                            me->SummonCreature(NPC_SALYIN_WARMONGER, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(),
                                TempSummonType::TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 30 * IN_MILLISECONDS);
                        events.ScheduleEvent(EVENT_WARMONGERS, MINUTE * IN_MILLISECONDS);
                        break;
                    case EVENT_BERSERK:
                        DoCast(me, SPELL_BERSERK, true);
                        break;
                    case EVENT_CHECK_AREA:
                        // Galleon never moves, so the evade check looks at its attackers instead.
                        if (!AnyAttackerInRange())
                        {
                            EnterEvadeMode();
                            return;
                        }
                        events.ScheduleEvent(EVENT_CHECK_AREA, 2 * IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }

        bool AnyAttackerInRange()
        {
            ThreatContainer::StorageType const& threatList = me->getThreatManager().getThreatList();
            for (ThreatContainer::StorageType::const_iterator itr = threatList.begin(); itr != threatList.end(); ++itr)
                if (Unit* unit = Unit::GetUnit(*me, (*itr)->getUnitGuid()))
                    if (me->IsWithinDist(unit, COMBAT_AREA_RANGE))
                        return true;
            return false;
        }

    private:
        EventMap events;
        SummonList summons;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_galionAI(creature);
    }
};

// Static deck spawns: they fire Fire Shot at the players around the galleon while it fights.
class npc_salyin_skrimisher : public CreatureScript
{
public:
    npc_salyin_skrimisher() : CreatureScript("npc_salyin_skrimisher") { }

    struct npc_salyin_skrimisherAI : public ScriptedAI
    {
        npc_salyin_skrimisherAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() OVERRIDE
        {
            events.Reset();
        }

        void DoAction(int32 action) OVERRIDE
        {
            if (action == ACTION_CREW_START)
                events.ScheduleEvent(EVENT_FIRE_SHOT, urand(3, 16) * IN_MILLISECONDS);
            else if (action == ACTION_CREW_STOP)
                events.Reset();
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            events.Update(diff);

            if (events.ExecuteEvent() == EVENT_FIRE_SHOT)
            {
                DoCast(me, SPELL_FIRE_SHOT);
                events.ScheduleEvent(EVENT_FIRE_SHOT, urand(3, 16) * IN_MILLISECONDS);
            }
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_salyin_skrimisherAI(creature);
    }
};

// Summoned in waves by the galleon. The template is the unselectable deck crew,
// so summoned ones are made selectable before they attack a random player nearby.
class npc_salyin_warmonger : public CreatureScript
{
public:
    npc_salyin_warmonger() : CreatureScript("npc_salyin_warmonger") { }

    struct npc_salyin_warmongerAI : public ScriptedAI
    {
        npc_salyin_warmongerAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() OVERRIDE
        {
            events.Reset();
        }

        void IsSummonedBy(Unit* /*summoner*/) OVERRIDE
        {
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

            std::list<Player*> players;
            GetPlayerListInGrid(players, me, PLAYER_SEARCH_RANGE);
            for (std::list<Player*>::iterator itr = players.begin(); itr != players.end();)
            {
                if (!(*itr)->IsAlive() || (*itr)->IsGameMaster() || !me->IsValidAttackTarget(*itr))
                    itr = players.erase(itr);
                else
                    ++itr;
            }

            if (players.empty())
                return;

            Skyfire::Containers::RandomResizeList(players, 1);
            AttackStart(players.front());
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            events.ScheduleEvent(EVENT_IMPALING_PULL, 50 * IN_MILLISECONDS);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (events.ExecuteEvent() == EVENT_IMPALING_PULL)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                    DoCast(target, SPELL_IMPALING_PULL, true);
                events.ScheduleEvent(EVENT_IMPALING_PULL, MINUTE * IN_MILLISECONDS);
            }

            DoMeleeAttackIfReady();
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_salyin_warmongerAI(creature);
    }
};

// 121673 - Fire Shot: with more than three players in range, only half of them are hit.
class spell_galleon_fire_shot : public SpellScriptLoader
{
public:
    spell_galleon_fire_shot() : SpellScriptLoader("spell_galleon_fire_shot") { }

    class spell_galleon_fire_shot_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_galleon_fire_shot_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            if (targets.size() > 3)
                Skyfire::Containers::RandomResizeList(targets, uint32(targets.size() / 2));
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_galleon_fire_shot_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_galleon_fire_shot_SpellScript();
    }
};

void AddSC_boss_galleon()
{
    new boss_galion();
    new npc_salyin_skrimisher();
    new npc_salyin_warmonger();
    new spell_galleon_fire_shot();
}
