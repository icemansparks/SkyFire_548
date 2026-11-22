/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_Yu_Lon
SD%Complete: 70
SDComment: Timeless Isle Celestial Court. Gaps: the Jadefire Walls move across the court but
           deal no damage (SkyFire has no area trigger auras), no dynamic health scaling by
           group size, no personal loot or currency, no guild achievement credit. Jadefire Bolt
           prefers ranged-class players (SkyFire has no talent role lookup).
SDCategory: Timeless Isle
EndScriptData */

#include "celestial_court.h"
#include "SpellScript.h"
#include "Containers.h"
#include <cmath>

enum YuLonSpells
{
    SPELL_JADEFIRE_BREATH       = 144530,
    SPELL_JADEFIRE_BUFFET       = 144630,
    SPELL_JADEFIRE_BOLT_AREA    = 144545,
    SPELL_JADEFIRE_BLAZE        = 144537
};

enum YuLonEvents
{
    EVENT_JADEFIRE_BREATH = 1,
    EVENT_JADEFIRE_WALL,
    EVENT_JADEFIRE_BOLT,
    EVENT_JADEFIRE_BUFFET,
    EVENT_WALL_GO,
    EVENT_WALL_DESPAWN
};

// creature_text groups of Yu'lon (71955).
enum YuLonTexts
{
    SAY_AGGRO   = 0,
    SAY_INTRO   = 1,
    SAY_DEATH   = 2,
    SAY_KILL    = 3,
    SAY_WALL_1  = 4,
    SAY_WALL_3  = 6
};

enum YuLonData
{
    NPC_JADEFIRE_WALL   = 72020,
    NPC_JADEFIRE_BLAZE  = 72016,

    WALL_COUNT          = 6,
    WALL_SPACING        = 36,
    WALL_RUN_DISTANCE   = 146,
    MAX_BOLT_TARGETS    = 8
};

Position const WallStartPosition = { -562.14f, -5086.51f, -6.278f, COURT_FACING };

class boss_yu_lon : public CreatureScript
{
public:
    boss_yu_lon() : CreatureScript("boss_yu_lon") { }

    struct boss_yu_lonAI : public CelestialAI
    {
        boss_yu_lonAI(Creature* creature) : CelestialAI(creature, SHAOHAO_OUTRO_YU_LON, SAY_AGGRO, SAY_INTRO, SAY_DEATH, SAY_KILL) { }

        void JustSummoned(Creature* summon) OVERRIDE
        {
            CelestialAI::JustSummoned(summon);

            if (summon->GetEntry() == NPC_JADEFIRE_BLAZE)
                summon->CastSpell(summon, SPELL_JADEFIRE_BLAZE, true);
        }

    protected:
        void ScheduleCombatEvents() OVERRIDE
        {
            events.ScheduleEvent(EVENT_JADEFIRE_BREATH, 6000);
            events.ScheduleEvent(EVENT_JADEFIRE_WALL, 40000);
            events.ScheduleEvent(EVENT_JADEFIRE_BOLT, 15000);
            events.ScheduleEvent(EVENT_JADEFIRE_BUFFET, 30000);
        }

        void ExecuteCombatEvent(uint32 eventId) OVERRIDE
        {
            switch (eventId)
            {
                case EVENT_JADEFIRE_BREATH:
                    if (Unit* victim = me->GetVictim())
                        DoCast(victim, SPELL_JADEFIRE_BREATH);
                    events.ScheduleEvent(EVENT_JADEFIRE_BREATH, 10000);
                    break;
                case EVENT_JADEFIRE_BUFFET:
                    DoCast(me, SPELL_JADEFIRE_BUFFET);
                    events.ScheduleEvent(EVENT_JADEFIRE_BUFFET, 30000);
                    break;
                case EVENT_JADEFIRE_BOLT:
                    DoCast(me, SPELL_JADEFIRE_BOLT_AREA);
                    events.ScheduleEvent(EVENT_JADEFIRE_BOLT, 15000);
                    break;
                case EVENT_JADEFIRE_WALL:
                    SummonWalls();
                    events.ScheduleEvent(EVENT_JADEFIRE_WALL, 60000);
                    break;
                default:
                    break;
            }
        }

    private:
        // A row of walls across the court with one gap, at one of the four inner places.
        void SummonWalls()
        {
            uint8 gap = urand(1, WALL_COUNT - 2);
            float perpendicular = Position::NormalizeOrientation(COURT_FACING + M_PI / 2);

            me->SummonCreature(NPC_JADEFIRE_WALL, WallStartPosition, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
            for (uint8 i = 1; i < WALL_COUNT; ++i)
            {
                if (i == gap)
                    continue;

                me->SummonCreature(NPC_JADEFIRE_WALL,
                    WallStartPosition.GetPositionX() + WALL_SPACING * i * std::cos(perpendicular),
                    WallStartPosition.GetPositionY() + WALL_SPACING * i * std::sin(perpendicular),
                    WallStartPosition.GetPositionZ(), COURT_FACING, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
            }

            Talk(urand(SAY_WALL_1, SAY_WALL_3));
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_yu_lonAI(creature);
    }
};

// Jadefire Wall (72020): waits 2 s, runs across the court and despawns. The damage aura of the
// original (an area trigger) is a gap.
class npc_jadefire_wall : public CreatureScript
{
public:
    npc_jadefire_wall() : CreatureScript("npc_jadefire_wall") { }

    struct npc_jadefire_wallAI : public ScriptedAI
    {
        npc_jadefire_wallAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() OVERRIDE
        {
            me->setActive(true);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
            me->SetFacingTo(COURT_FACING);
            events.Reset();
            events.ScheduleEvent(EVENT_WALL_GO, 2000);
        }

        void EnterEvadeMode() OVERRIDE { }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_WALL_GO:
                    {
                        float angle = me->GetOrientation();
                        me->GetMotionMaster()->MovePoint(0, me->GetPositionX() + WALL_RUN_DISTANCE * std::cos(angle),
                            me->GetPositionY() + WALL_RUN_DISTANCE * std::sin(angle), me->GetPositionZ());
                        events.ScheduleEvent(EVENT_WALL_DESPAWN, uint32(WALL_RUN_DISTANCE / me->GetSpeed(MOVE_RUN) * 1000) + 500);
                        break;
                    }
                    case EVENT_WALL_DESPAWN:
                        me->DespawnOrUnsummon();
                        break;
                    default:
                        break;
                }
            }
        }

    private:
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_jadefire_wallAI(creature);
    }
};

// 144545 - Jadefire Bolt: hits up to 8 players around Yu'lon (ranged players first) and casts the
// spell id held in effect 0's base points (144532) on each. Only players are targeted.
class spell_yulon_jadefire_bolt_selector : public SpellScriptLoader
{
public:
    spell_yulon_jadefire_bolt_selector() : SpellScriptLoader("spell_yulon_jadefire_bolt_selector") { }

    class spell_yulon_jadefire_bolt_selector_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_yulon_jadefire_bolt_selector_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            std::list<WorldObject*> ranged;
            std::list<WorldObject*> others;
            for (std::list<WorldObject*>::const_iterator itr = targets.begin(); itr != targets.end(); ++itr)
            {
                Unit* unit = (*itr)->ToUnit();
                if (!unit || unit->GetTypeId() != TypeID::TYPEID_PLAYER)
                    continue;

                if (IsRangedClass(unit))
                    ranged.push_back(*itr);
                else
                    others.push_back(*itr);
            }

            // Ranged players first; the free slots go to random other players.
            if (ranged.size() > MAX_BOLT_TARGETS)
            {
                Skyfire::Containers::RandomResizeList(ranged, MAX_BOLT_TARGETS);
                others.clear();
            }
            else if (ranged.size() + others.size() > MAX_BOLT_TARGETS)
                Skyfire::Containers::RandomResizeList(others, MAX_BOLT_TARGETS - ranged.size());

            ranged.splice(ranged.end(), others);
            targets = ranged;
        }

        void HandleScriptEffect(SpellEffIndex effIndex)
        {
            if (Unit* target = GetHitUnit())
                GetCaster()->CastSpell(target, GetSpellInfo()->Effects[effIndex].BasePoints, true);
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_yulon_jadefire_bolt_selector_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnEffectHitTarget += SpellEffectFn(spell_yulon_jadefire_bolt_selector_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_yulon_jadefire_bolt_selector_SpellScript();
    }
};

void AddSC_boss_yu_lon()
{
    new boss_yu_lon();
    new npc_jadefire_wall();
    new spell_yulon_jadefire_bolt_selector();
}
