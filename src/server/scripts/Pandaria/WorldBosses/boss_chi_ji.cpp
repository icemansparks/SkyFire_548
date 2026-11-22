/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_Chi_Ji
SD%Complete: 85
SDComment: Timeless Isle Celestial Court. Gaps: no dynamic health scaling by group size, no
           personal loot or currency, no guild achievement credit. Beacon of Hope goes to a
           ranged-class player (SkyFire has no talent role lookup).
SDCategory: Timeless Isle
EndScriptData */

#include "celestial_court.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"

enum ChiJiSpells
{
    SPELL_INSPIRING_SONG        = 144468,
    SPELL_BEACON_OF_HOPE_SUMMON = 144473,
    SPELL_BEACON_OF_HOPE        = 144474,
    SPELL_BLAZING_SONG          = 144471,
    SPELL_CRANE_RUSH            = 144470,
    SPELL_FIRESTORM             = 144463,
    SPELL_FIRESTORM_SUMMON      = 144461,
    SPELL_BLAZING_NOVA          = 144493
};

enum ChiJiEvents
{
    EVENT_FIRESTORM = 1,
    EVENT_INSPIRING_SONG,
    EVENT_BEACON_OF_HOPE,
    EVENT_BLAZING_SONG,
    EVENT_CRANE_RUSH,
    EVENT_CRANE_RUSH_CHECK,
    EVENT_CHILD_NOVA,
    EVENT_CHILD_DESPAWN
};

// creature_text groups of Chi-Ji (71952).
enum ChiJiTexts
{
    SAY_AGGRO       = 0,
    SAY_INTRO       = 1,
    SAY_DEATH       = 2,
    SAY_KILL        = 3,
    SAY_BEACON      = 4,
    SAY_CRANE_RUSH_1 = 5,
    SAY_CRANE_RUSH_2 = 6
};

enum ChiJiData
{
    NPC_FIRESTORM           = 71971,
    NPC_BEACON_OF_HOPE      = 71978,

    ACTION_CRANE_RUSH_ENDED = 2,

    CHILD_RUN_DISTANCE      = 100
};

class boss_chi_ji : public CreatureScript
{
public:
    boss_chi_ji() : CreatureScript("boss_chi_ji") { }

    struct boss_chi_jiAI : public CelestialAI
    {
        boss_chi_jiAI(Creature* creature) : CelestialAI(creature, SHAOHAO_OUTRO_CHI_JI, SAY_AGGRO, SAY_INTRO, SAY_DEATH, SAY_KILL),
            craneRushActive(false), nextChildrenPct(66.0f) { }

        void Reset() OVERRIDE
        {
            CelestialAI::Reset();
            craneRushActive = false;
            nextChildrenPct = 66.0f;
        }

        // Crane Rush at 66% and 33% health.
        void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
        {
            CelestialAI::DamageTaken(attacker, damage);

            // Defeated by this hit: no Crane Rush.
            if (me->getFaction() == FACTION_CELESTIAL_FRIENDLY)
                return;

            if (nextChildrenPct > 0.0f && me->HealthBelowPctDamaged(uint32(nextChildrenPct), damage))
            {
                nextChildrenPct -= 33.0f;
                events.ScheduleEvent(EVENT_CRANE_RUSH, 500);
            }
        }

        void JustSummoned(Creature* summon) OVERRIDE
        {
            CelestialAI::JustSummoned(summon);

            switch (summon->GetEntry())
            {
                case NPC_FIRESTORM:
                    summon->CastSpell(summon, SPELL_FIRESTORM, false);
                    break;
                case NPC_BEACON_OF_HOPE:
                    summon->CastSpell(summon, SPELL_BEACON_OF_HOPE, false);
                    break;
                default:
                    break;
            }
        }

        // Called by the Crane Rush aura when it ends: the normal rotation starts again.
        void DoAction(int32 action) OVERRIDE
        {
            // The aura's end and the check event can both report the end: only the first counts.
            if (action != ACTION_CRANE_RUSH_ENDED || !craneRushActive || !me->IsInCombat())
                return;

            craneRushActive = false;

            events.ScheduleEvent(EVENT_FIRESTORM, 1000);
            events.ScheduleEvent(EVENT_INSPIRING_SONG, 5000);
            // 30 s, not 50 s: as in alexkulya's script, the first beacon after Crane Rush comes sooner.
            events.ScheduleEvent(EVENT_BEACON_OF_HOPE, 30000);
        }

    protected:
        void ScheduleCombatEvents() OVERRIDE
        {
            events.ScheduleEvent(EVENT_FIRESTORM, 10000);
            events.ScheduleEvent(EVENT_INSPIRING_SONG, 19500);
            events.ScheduleEvent(EVENT_BEACON_OF_HOPE, 50000);
        }

        void ExecuteCombatEvent(uint32 eventId) OVERRIDE
        {
            switch (eventId)
            {
                case EVENT_BEACON_OF_HOPE:
                {
                    Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, RangedPlayerSelector());
                    if (!target)
                        target = SelectTarget(SELECT_TARGET_RANDOM, 0, 40.0f, true);
                    if (target)
                    {
                        Talk(SAY_BEACON);
                        DoCast(target, SPELL_BEACON_OF_HOPE_SUMMON);
                        events.ScheduleEvent(EVENT_BLAZING_SONG, 2500);
                    }
                    events.ScheduleEvent(EVENT_BEACON_OF_HOPE, 50000);
                    break;
                }
                case EVENT_INSPIRING_SONG:
                    DoCast(me, SPELL_INSPIRING_SONG);
                    events.ScheduleEvent(EVENT_INSPIRING_SONG, 30000);
                    break;
                case EVENT_FIRESTORM:
                    DoCast(me, SPELL_FIRESTORM_SUMMON);
                    events.ScheduleEvent(EVENT_FIRESTORM, 14500);
                    break;
                case EVENT_CRANE_RUSH:
                    // The aura's end (DoAction) brings the normal events back; the check event covers a cast
                    // that did not land.
                    events.Reset();
                    craneRushActive = true;
                    Talk(urand(SAY_CRANE_RUSH_1, SAY_CRANE_RUSH_2));
                    DoCast(me, SPELL_CRANE_RUSH);
                    events.ScheduleEvent(EVENT_CRANE_RUSH_CHECK, 3000);
                    break;
                case EVENT_CRANE_RUSH_CHECK:
                    // The cast did not land (stunned, interrupted): without the aura nothing
                    // would ever restart the rotation.
                    if (!me->HasAura(SPELL_CRANE_RUSH))
                        DoAction(ACTION_CRANE_RUSH_ENDED);
                    break;
                case EVENT_BLAZING_SONG:
                    DoCast(me, SPELL_BLAZING_SONG);
                    break;
                default:
                    break;
            }
        }

    private:
        bool craneRushActive;
        float nextChildrenPct;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_chi_jiAI(creature);
    }
};

// Child of Chi-Ji (71990), summoned by Crane Rush: casts Blazing Nova, then runs straight away
// from Chi-Ji and despawns.
class npc_child_of_chi_ji : public CreatureScript
{
public:
    npc_child_of_chi_ji() : CreatureScript("npc_child_of_chi_ji") { }

    struct npc_child_of_chi_jiAI : public ScriptedAI
    {
        npc_child_of_chi_jiAI(Creature* creature) : ScriptedAI(creature), summonerGuid(0) { }

        void IsSummonedBy(Unit* summoner) OVERRIDE
        {
            summonerGuid = summoner->GetGUID();
        }

        void Reset() OVERRIDE
        {
            me->setActive(true);
            events.Reset();
            events.ScheduleEvent(EVENT_CHILD_NOVA, 500);
        }

        void EnterEvadeMode() OVERRIDE { }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_CHILD_NOVA:
                    {
                        DoCast(me, SPELL_BLAZING_NOVA);
                        Creature* chiJi = ObjectAccessor::GetCreature(*me, summonerGuid);
                        if (!chiJi)
                        {
                            me->DespawnOrUnsummon();
                            break;
                        }

                        float angle = chiJi->GetAngle(me);
                        me->GetMotionMaster()->MovePoint(0, me->GetPositionX() + CHILD_RUN_DISTANCE * std::cos(angle),
                            me->GetPositionY() + CHILD_RUN_DISTANCE * std::sin(angle), me->GetPositionZ());
                        events.ScheduleEvent(EVENT_CHILD_DESPAWN, uint32(CHILD_RUN_DISTANCE / me->GetSpeed(MOVE_RUN) * 1000) + 500);
                        break;
                    }
                    case EVENT_CHILD_DESPAWN:
                        me->DespawnOrUnsummon();
                        break;
                    default:
                        break;
                }
            }
        }

    private:
        EventMap events;
        uint64 summonerGuid;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_child_of_chi_jiAI(creature);
    }
};

// 144475 - Beacon of Hope: only players in the area get the damage reduction.
class spell_chi_ji_beacon_of_hope : public SpellScriptLoader
{
public:
    spell_chi_ji_beacon_of_hope() : SpellScriptLoader("spell_chi_ji_beacon_of_hope") { }

    class spell_chi_ji_beacon_of_hope_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_chi_ji_beacon_of_hope_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if(NonPlayerTargetCheck());
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_chi_ji_beacon_of_hope_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
        }

        struct NonPlayerTargetCheck
        {
            bool operator()(WorldObject* target) const
            {
                return target->GetTypeId() != TypeID::TYPEID_PLAYER;
            }
        };
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_chi_ji_beacon_of_hope_SpellScript();
    }
};

// 144470 - Crane Rush: when the aura ends, Chi-Ji goes back to its normal rotation.
class spell_chi_ji_crane_rush : public SpellScriptLoader
{
public:
    spell_chi_ji_crane_rush() : SpellScriptLoader("spell_chi_ji_crane_rush") { }

    class spell_chi_ji_crane_rush_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_chi_ji_crane_rush_AuraScript);

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Creature* owner = GetTarget()->ToCreature())
                if (owner->AI())
                    owner->AI()->DoAction(ACTION_CRANE_RUSH_ENDED);
        }

        void Register() OVERRIDE
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_chi_ji_crane_rush_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_chi_ji_crane_rush_AuraScript();
    }
};

void AddSC_boss_chi_ji()
{
    new boss_chi_ji();
    new npc_child_of_chi_ji();
    new spell_chi_ji_beacon_of_hope();
    new spell_chi_ji_crane_rush();
}
