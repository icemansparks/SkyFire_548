/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_Niuzao
SD%Complete: 85
SDComment: Timeless Isle Celestial Court. Gaps: no dynamic health scaling by group size, no
           personal loot or currency, no guild achievement credit. Headbutt's threat redirect
           is ported as alexkulya has it; its effect in play is unverified.
SDCategory: Timeless Isle
EndScriptData */

#include "celestial_court.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"

enum NiuzaoSpells
{
    SPELL_CHARGE            = 144608,
    SPELL_HEADBUTT          = 144610,
    SPELL_MASSIVE_QUAKE     = 144611,
    SPELL_OXEN_FORTITUDE    = 144606
};

enum NiuzaoEvents
{
    EVENT_OXEN_FORTITUDE = 1,
    EVENT_HEADBUTT,
    EVENT_MASSIVE_QUAKE,
    EVENT_CHARGE_PREPARE,
    EVENT_CHARGE_NEXT
};

// creature_text groups of Niuzao (71954).
enum NiuzaoTexts
{
    SAY_AGGRO   = 0,
    SAY_INTRO   = 1,
    SAY_DEATH   = 2,
    SAY_KILL    = 3,
    SAY_CHARGE  = 4,
    SAY_QUAKE   = 6
};

enum NiuzaoData
{
    ACTION_CHARGE           = 2,
    POINT_CHARGE            = 2,
    CHARGE_LAST_POSITION    = 4
};

float const ChargePositions[CHARGE_LAST_POSITION + 1][3] =
{
    { -740.79f, -5019.06f, -6.277f },
    { -740.41f, -4962.37f, -6.277f },
    { -563.80f, -4963.35f, -6.277f },
    { -563.88f, -5069.82f, -6.277f },
    { -737.17f, -5072.31f, -6.277f }
};

class boss_niuzao : public CreatureScript
{
public:
    boss_niuzao() : CreatureScript("boss_niuzao") { }

    struct boss_niuzaoAI : public CelestialAI
    {
        boss_niuzaoAI(Creature* creature) : CelestialAI(creature, SHAOHAO_OUTRO_NIUZAO, SAY_AGGRO, SAY_INTRO, SAY_DEATH, SAY_KILL),
            charges(0), pendingRounds(0), roundActive(false), nextChargePct(66.0f) { }

        void Reset() OVERRIDE
        {
            CelestialAI::Reset();
            me->RemoveAllAuras();
            charges = 0;
            pendingRounds = 0;
            roundActive = false;
            nextChargePct = 66.0f;
        }

        // A charge round at 66% and 33% health.
        void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
        {
            CelestialAI::DamageTaken(attacker, damage);

            // Defeated by this hit: no charge round.
            if (me->getFaction() == FACTION_CELESTIAL_FRIENDLY)
                return;

            if (nextChargePct > 0.0f && me->HealthBelowPctDamaged(uint32(nextChargePct), damage))
            {
                nextChargePct -= 33.0f;
                // A threshold crossed while a round runs waits for that round to end.
                ++pendingRounds;
                if (!roundActive)
                {
                    // Marked now, not when the event fires, so a second crossing inside the delay only queues.
                    roundActive = true;
                    events.ScheduleEvent(EVENT_CHARGE_PREPARE, 500);
                }
            }
        }

        // Called by the Charge aura when it is applied.
        void DoAction(int32 action) OVERRIDE
        {
            if (action == ACTION_CHARGE)
                me->GetMotionMaster()->MoveCharge(ChargePositions[charges][0], ChargePositions[charges][1],
                    ChargePositions[charges][2], 15.0f, POINT_CHARGE);
        }

        void MovementInform(uint32 type, uint32 pointId) OVERRIDE
        {
            if (type == POINT_MOTION_TYPE && pointId == POINT_CHARGE)
            {
                if (charges >= CHARGE_LAST_POSITION)
                {
                    // Last stop: the round ends and the normal rotation starts again, sooner.
                    charges = 0;
                    me->RemoveAurasDueToSpell(SPELL_CHARGE);
                    events.Reset();
                    events.ScheduleEvent(EVENT_OXEN_FORTITUDE, 12000);
                    events.ScheduleEvent(EVENT_MASSIVE_QUAKE, 15000);
                    events.ScheduleEvent(EVENT_HEADBUTT, 6000);
                    roundActive = false;
                    if (pendingRounds > 0)
                    {
                        roundActive = true;
                        events.ScheduleEvent(EVENT_CHARGE_PREPARE, 500);
                    }
                }
                else
                    events.ScheduleEvent(EVENT_CHARGE_NEXT, 100);
                return;
            }

            CelestialAI::MovementInform(type, pointId);
        }

    protected:
        void ScheduleCombatEvents() OVERRIDE
        {
            events.ScheduleEvent(EVENT_OXEN_FORTITUDE, 25000);
            events.ScheduleEvent(EVENT_MASSIVE_QUAKE, 30000);
            events.ScheduleEvent(EVENT_HEADBUTT, 12000);
        }

        void ExecuteCombatEvent(uint32 eventId) OVERRIDE
        {
            // While charging only the charge events run; the others are dropped and the round
            // reschedules them when it ends.
            if (eventId != EVENT_CHARGE_PREPARE && eventId != EVENT_CHARGE_NEXT && me->HasAura(SPELL_CHARGE))
                return;

            switch (eventId)
            {
                case EVENT_CHARGE_PREPARE:
                    roundActive = true;
                    if (pendingRounds > 0)
                        --pendingRounds;
                    DoCast(me, SPELL_CHARGE);
                    Talk(SAY_CHARGE);
                    break;
                case EVENT_CHARGE_NEXT:
                    ++charges;
                    DoAction(ACTION_CHARGE);
                    break;
                case EVENT_MASSIVE_QUAKE:
                    DoCast(me, SPELL_MASSIVE_QUAKE);
                    Talk(SAY_QUAKE);
                    events.ScheduleEvent(EVENT_MASSIVE_QUAKE, 35000);
                    break;
                case EVENT_HEADBUTT:
                    if (Unit* victim = me->GetVictim())
                        DoCast(victim, SPELL_HEADBUTT);
                    events.ScheduleEvent(EVENT_HEADBUTT, 20500);
                    break;
                case EVENT_OXEN_FORTITUDE:
                    DoCast(me, SPELL_OXEN_FORTITUDE);
                    events.ScheduleEvent(EVENT_OXEN_FORTITUDE, 30000);
                    break;
                default:
                    break;
            }
        }

    private:
        uint8 charges;
        uint8 pendingRounds;
        bool roundActive;
        float nextChargePct;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_niuzaoAI(creature);
    }
};

// 144610 - Headbutt: drops the hit target's threat on Niuzao. alexkulya's SetRedirectThreat call sets the
// redirect on the boss, which the threat code never reads (it looks at the unit being credited), so it did nothing.
class spell_niuzao_headbutt : public SpellScriptLoader
{
public:
    spell_niuzao_headbutt() : SpellScriptLoader("spell_niuzao_headbutt") { }

    class spell_niuzao_headbutt_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_niuzao_headbutt_SpellScript);

        void HandleKnockBack(SpellEffIndex /*effIndex*/)
        {
            if (Unit* target = GetHitUnit())
                GetCaster()->getThreatManager().modifyThreatPercent(target, -100);
        }

        void Register() OVERRIDE
        {
            OnEffectHitTarget += SpellEffectFn(spell_niuzao_headbutt_SpellScript::HandleKnockBack, EFFECT_1, SPELL_EFFECT_KNOCK_BACK);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_niuzao_headbutt_SpellScript();
    }
};

// 144608 - Charge: the aura starts the run through the court.
class spell_niuzao_charge : public SpellScriptLoader
{
public:
    spell_niuzao_charge() : SpellScriptLoader("spell_niuzao_charge") { }

    class spell_niuzao_charge_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_niuzao_charge_AuraScript);

        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Creature* owner = GetTarget()->ToCreature())
                if (owner->AI())
                    owner->AI()->DoAction(ACTION_CHARGE);
        }

        void Register() OVERRIDE
        {
            OnEffectApply += AuraEffectApplyFn(spell_niuzao_charge_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_niuzao_charge_AuraScript();
    }
};

void AddSC_boss_niuzao()
{
    new boss_niuzao();
    new spell_niuzao_headbutt();
    new spell_niuzao_charge();
}
