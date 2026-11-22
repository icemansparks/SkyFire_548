/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_Sha_of_Anger
SD%Complete: 80
SDComment: Kun-Lai Summit world boss. Overcome by Anger is applied by the boss itself
           to players within 30 yards, not by a bunny.
SDCategory: Kun-Lai Summit
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Player.h"

enum ShaOfAngerSpells
{
    SPELL_SEETHE                = 119487,
    SPELL_UNLEASHED_WRATH       = 119488,
    SPELL_ENDLESS_RAGE          = 119446,
    SPELL_BITTER_THOUGHTS       = 119601,
    SPELL_GROWING_ANGER         = 119622,
    SPELL_AGGRESSIVE_BEHAVIOR   = 119626,
    SPELL_OVERCOME_BY_ANGER     = 129356,
    SPELL_BERSERK               = 47008
};

enum ShaOfAngerTexts
{
    TALK_ANGER                  = 1,
    TALK_SPAWN                  = 2,
    TALK_RESET                  = 3,
    TALK_SLAY                   = 4,
    TALK_AGGRO                  = 5
};

enum ShaOfAngerEvents
{
    EVENT_GROWING_ANGER         = 1,
    EVENT_AGGRESSIVE_BEHAVIOR,
    EVENT_ENDLESS_RAGE,
    EVENT_UPDATE_RAGE,
    EVENT_SEETHE,
    EVENT_BERSERK
};

enum ShaOfAngerMisc
{
    NPC_SHA_OF_ANGER_CLOUD      = 61523,
    GROWING_ANGER_TARGETS       = 2,
    ENDLESS_RAGE_CASTS          = 3,
    RAGE_MAX                    = 1000,
    RAGE_STEP                   = 20
};

float const SHA_REACH               = 37.0f;
float const OVERCOME_RANGE          = 30.0f;
uint32 const OVERCOME_CHECK_TIMER   = 2 * IN_MILLISECONDS;

class boss_sha_of_anger : public CreatureScript
{
public:
    boss_sha_of_anger() : CreatureScript("boss_sha_of_anger") { }

    struct boss_sha_of_angerAI : public ScriptedAI
    {
        boss_sha_of_angerAI(Creature* creature) : ScriptedAI(creature), summons(creature) { }

        void Reset() OVERRIDE
        {
            SetCombatMovement(false);
            me->setPowerType(POWER_RAGE);
            me->SetMaxPower(POWER_RAGE, RAGE_MAX);
            me->SetPower(POWER_RAGE, 0);
            me->SetFloatValue(UNIT_FIELD_BOUNDING_RADIUS, SHA_REACH);
            me->SetFloatValue(UNIT_FIELD_COMBAT_REACH, SHA_REACH);

            events.Reset();
            summons.DespawnAll();
            angerTargets.clear();
            rage = 0;
            rising = true;
            seethePending = false;
            overcomeTimer = OVERCOME_CHECK_TIMER;
            Talk(TALK_RESET);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            Talk(TALK_AGGRO);
            events.ScheduleEvent(EVENT_ENDLESS_RAGE, 8 * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_GROWING_ANGER, 19 * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_UPDATE_RAGE, 4 * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_BERSERK, 15 * MINUTE * IN_MILLISECONDS);
        }

        void KilledUnit(Unit* victim) OVERRIDE
        {
            if (victim->GetTypeId() == TypeID::TYPEID_PLAYER)
                Talk(TALK_SLAY);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            summons.DespawnAll();
        }

        // Endless Rage summons the Bitter Thoughts clouds and the Ire adds.
        void JustSummoned(Creature* summon) OVERRIDE
        {
            summons.Summon(summon);
            if (summon->GetEntry() == NPC_SHA_OF_ANGER_CLOUD)
            {
                summon->CastSpell(summon, SPELL_BITTER_THOUGHTS, true);
                summon->DespawnOrUnsummon(MINUTE * IN_MILLISECONDS);
            }
        }

        void SummonedCreatureDespawn(Creature* summon) OVERRIDE
        {
            summons.Despawn(summon);
        }

        // Players near the Sha get the aura, players who walked out of range lose it.
        void ApplyOvercomeByAnger()
        {
            std::list<Player*> players;
            GetPlayerListInGrid(players, me, OVERCOME_RANGE * 2);
            for (std::list<Player*>::const_iterator itr = players.begin(); itr != players.end(); ++itr)
            {
                Player* player = *itr;
                bool inRange = player->IsAlive() && !player->IsGameMaster() && me->IsWithinDist(player, OVERCOME_RANGE);
                if (inRange && !player->HasAura(SPELL_OVERCOME_BY_ANGER))
                    me->AddAura(SPELL_OVERCOME_BY_ANGER, player);
                else if (!inRange)
                    player->RemoveAura(SPELL_OVERCOME_BY_ANGER, me->GetGUID());
            }
        }

        // Rage fills by 2 per second; when full the Sha casts Unleashed Wrath and it drains again.
        void UpdateRage()
        {
            if (rising)
            {
                rage = std::min<int32>(rage + RAGE_STEP, RAGE_MAX);
                if (rage == RAGE_MAX)
                {
                    rising = false;
                    DoCast(me, SPELL_UNLEASHED_WRATH);
                }
            }
            else
            {
                rage = std::max<int32>(rage - RAGE_STEP, 0);
                if (rage == 0)
                    rising = true;
            }
            me->SetPower(POWER_RAGE, rage);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (overcomeTimer <= diff)
            {
                ApplyOvercomeByAnger();
                overcomeTimer = OVERCOME_CHECK_TIMER;
            }
            else
                overcomeTimer -= diff;

            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_GROWING_ANGER:
                        Talk(TALK_ANGER);
                        angerTargets.clear();
                        for (uint8 i = 0; i < GROWING_ANGER_TARGETS; ++i)
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                            {
                                angerTargets.push_back(target->GetGUID());
                                DoCast(target, SPELL_GROWING_ANGER, true);
                            }
                        events.ScheduleEvent(EVENT_AGGRESSIVE_BEHAVIOR, 6 * IN_MILLISECONDS);
                        break;
                    case EVENT_AGGRESSIVE_BEHAVIOR:
                        // The tank is never turned against the raid.
                        for (std::list<uint64>::const_iterator itr = angerTargets.begin(); itr != angerTargets.end(); ++itr)
                            if (Player* target = ObjectAccessor::GetPlayer(*me, *itr))
                                if (target->IsAlive() && target != me->GetVictim())
                                    DoCast(target, SPELL_AGGRESSIVE_BEHAVIOR);
                        angerTargets.clear();
                        events.ScheduleEvent(EVENT_GROWING_ANGER, 19 * IN_MILLISECONDS);
                        break;
                    case EVENT_ENDLESS_RAGE:
                        Talk(TALK_SPAWN);
                        for (uint8 i = 0; i < ENDLESS_RAGE_CASTS; ++i)
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                DoCast(target, SPELL_ENDLESS_RAGE, true);
                        events.ScheduleEvent(EVENT_ENDLESS_RAGE, 15 * IN_MILLISECONDS);
                        break;
                    case EVENT_UPDATE_RAGE:
                        UpdateRage();
                        events.ScheduleEvent(EVENT_UPDATE_RAGE, IN_MILLISECONDS);
                        break;
                    case EVENT_SEETHE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            DoCast(target, SPELL_SEETHE);
                        seethePending = false;
                        break;
                    case EVENT_BERSERK:
                        DoCast(me, SPELL_BERSERK, true);
                        break;
                    default:
                        break;
                }
            }

            // The Sha does not move: a tank out of its reach gets Seethe instead of melee.
            if (!seethePending && !me->IsWithinMeleeRange(me->GetVictim()))
            {
                seethePending = true;
                events.ScheduleEvent(EVENT_SEETHE, 2 * IN_MILLISECONDS);
            }

            DoMeleeAttackIfReady();
        }

    private:
        EventMap events;
        SummonList summons;
        std::list<uint64> angerTargets;
        int32 rage;
        bool rising;
        bool seethePending;
        uint32 overcomeTimer;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_sha_of_angerAI(creature);
    }
};

// 119626 - Aggressive Behavior: the core's charm aura turns the player; this frees them
// once they drop below half health.
class spell_sha_of_anger_aggressive_behaviour : public SpellScriptLoader
{
public:
    spell_sha_of_anger_aggressive_behaviour() : SpellScriptLoader("spell_sha_of_anger_aggressive_behaviour") { }

    class spell_sha_of_anger_aggressive_behaviour_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_of_anger_aggressive_behaviour_AuraScript);

        void HandlePeriodicTick(AuraEffect const* /*aurEff*/)
        {
            PreventDefaultAction();
            if (GetTarget()->GetHealthPct() < 50.0f)
                Remove(AURA_REMOVE_BY_DEFAULT);
        }

        void Register() OVERRIDE
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_of_anger_aggressive_behaviour_AuraScript::HandlePeriodicTick, EFFECT_5, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_of_anger_aggressive_behaviour_AuraScript();
    }
};

// 129356 - Overcome by Anger: drops off once the player is more than 30 yards from the Sha
// (for players the boss's own range sweep no longer sees, e.g. after a teleport).
class spell_sha_of_anger_overcome_by_anger : public SpellScriptLoader
{
public:
    spell_sha_of_anger_overcome_by_anger() : SpellScriptLoader("spell_sha_of_anger_overcome_by_anger") { }

    class spell_sha_of_anger_overcome_by_anger_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_of_anger_overcome_by_anger_AuraScript);

        void OnUpdate(uint32 /*diff*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetUnitOwner();
            if (!caster || target->GetMapId() != caster->GetMapId() || !target->IsWithinDist(caster, OVERCOME_RANGE))
                Remove(AURA_REMOVE_BY_DEFAULT);
        }

        void Register() OVERRIDE
        {
            OnAuraUpdate += AuraUpdateFn(spell_sha_of_anger_overcome_by_anger_AuraScript::OnUpdate);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_sha_of_anger_overcome_by_anger_AuraScript();
    }
};

void AddSC_boss_sha_of_anger()
{
    new boss_sha_of_anger();
    new spell_sha_of_anger_aggressive_behaviour();
    new spell_sha_of_anger_overcome_by_anger();
}
