/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_Ordos
SD%Complete: 70
SDComment: Timeless Isle world boss. Pool of Fire needs an area-trigger aura SkyFire
           lacks, so its damage ring is left out. The eligibility teleport and the
           Celestial Door are not ported.
SDCategory: Timeless Isle
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Player.h"

enum OrdosSpells
{
    SPELL_BURNING_SOUL          = 144689,
    SPELL_BURNING_SOUL_EFF      = 144690,
    SPELL_ANCIENT_FLAME_PULSE   = 144691,
    SPELL_ETERNAL_AGONY         = 144696,
    SPELL_MAGMA_CRUSH           = 144688,
    SPELL_POOL_OF_FIRE          = 144692,
    SPELL_ANCIENT_FLAME_SUMMON  = 144695
};

enum OrdosEvents
{
    EVENT_ANCIENT_FLAME = 1,
    EVENT_BURNING_SOUL,
    EVENT_POOL_OF_FIRE,
    EVENT_ETERNAL_AGONY,
    EVENT_MAGMA_CRUSH
};

// creature_text groups of Ordos (72057) as numbered in the world DB. alexkulya's DB has ten
// groups with three intro lines (0, 4 death, 5 kill, 6 pool, 7 flame, 8 soul, 9 agony);
// the world DB has seven and no intro lines, so each line is mapped by its text.
enum OrdosTexts
{
    SAY_AGGRO           = 0,
    SAY_DEATH           = 1,
    SAY_KILL            = 2,
    SAY_ANCIENT_FLAME   = 3,
    SAY_ETERNAL_AGONY   = 4,
    SAY_POOL_OF_FIRE    = 5,
    SAY_BURNING_SOUL    = 6
};

float const MAGMA_CRUSH_RANGE = 10.0f;

class boss_ordos : public CreatureScript
{
public:
    boss_ordos() : CreatureScript("boss_ordos") { }

    struct boss_ordosAI : public ScriptedAI
    {
        boss_ordosAI(Creature* creature) : ScriptedAI(creature), summons(creature) { }

        void Reset() OVERRIDE
        {
            events.Reset();
            summons.DespawnAll();
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            Talk(SAY_AGGRO);
            events.ScheduleEvent(EVENT_MAGMA_CRUSH, 9500);
            events.ScheduleEvent(EVENT_BURNING_SOUL, 20500);
            events.ScheduleEvent(EVENT_POOL_OF_FIRE, 27 * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_ANCIENT_FLAME, 40 * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_ETERNAL_AGONY, 5 * MINUTE * IN_MILLISECONDS);
        }

        void KilledUnit(Unit* victim) OVERRIDE
        {
            if (victim->GetTypeId() == TypeID::TYPEID_PLAYER)
                Talk(SAY_KILL);
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
            Talk(SAY_DEATH);
            summons.DespawnAll();
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            // One event per tick: a second DoCast in the same tick would be dropped while the
            // first spell is still casting. Events still due run on the next tick.
            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_MAGMA_CRUSH:
                        // On the tank; when the victim is a pet, on a random player in reach.
                        if (Unit* victim = me->GetVictim())
                        {
                            if (victim->GetTypeId() == TypeID::TYPEID_PLAYER)
                                DoCast(victim, SPELL_MAGMA_CRUSH);
                            else if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, MAGMA_CRUSH_RANGE, true))
                                DoCast(target, SPELL_MAGMA_CRUSH);
                        }
                        events.ScheduleEvent(EVENT_MAGMA_CRUSH, 15 * IN_MILLISECONDS);
                        break;
                    case EVENT_ANCIENT_FLAME:
                        DoCast(me, SPELL_ANCIENT_FLAME_SUMMON);
                        Talk(SAY_ANCIENT_FLAME);
                        events.ScheduleEvent(EVENT_ANCIENT_FLAME, 42 * IN_MILLISECONDS);
                        break;
                    case EVENT_BURNING_SOUL:
                        DoCast(me, SPELL_BURNING_SOUL);
                        Talk(SAY_BURNING_SOUL);
                        events.ScheduleEvent(EVENT_BURNING_SOUL, 29 * IN_MILLISECONDS);
                        break;
                    case EVENT_ETERNAL_AGONY:
                        DoCast(me, SPELL_ETERNAL_AGONY);
                        Talk(SAY_ETERNAL_AGONY);
                        break;
                    case EVENT_POOL_OF_FIRE:
                        if (Unit* victim = me->GetVictim())
                            me->CastSpell(victim->GetPositionX(), victim->GetPositionY(), victim->GetPositionZ(), SPELL_POOL_OF_FIRE, false);
                        Talk(SAY_POOL_OF_FIRE);
                        events.ScheduleEvent(EVENT_POOL_OF_FIRE, 31 * IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        EventMap events;
        SummonList summons;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_ordosAI(creature);
    }
};

// 72059 - Ancient Flame: pulses its aura for as long as it lives and never evades.
class npc_ordos_ancient_flame : public CreatureScript
{
public:
    npc_ordos_ancient_flame() : CreatureScript("npc_ordos_ancient_flame") { }

    struct npc_ordos_ancient_flameAI : public ScriptedAI
    {
        npc_ordos_ancient_flameAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() OVERRIDE
        {
            DoCast(me, SPELL_ANCIENT_FLAME_PULSE);
        }

        void EnterEvadeMode() OVERRIDE { }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_ordos_ancient_flameAI(creature);
    }
};

// 144688 - Magma Crush: the damage is shared between everyone the spell hits.
class spell_ordos_magma_crush : public SpellScriptLoader
{
public:
    spell_ordos_magma_crush() : SpellScriptLoader("spell_ordos_magma_crush") { }

    class spell_ordos_magma_crush_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_ordos_magma_crush_SpellScript);

        bool Load() OVERRIDE
        {
            targetsCount = 1;
            return true;
        }

        void CountTargets(std::list<WorldObject*>& targets)
        {
            if (!targets.empty())
                targetsCount = uint32(targets.size());
        }

        void SplitDamage(SpellEffIndex /*effIndex*/)
        {
            if (targetsCount > 1)
                SetHitDamage(int32(GetHitDamage() / targetsCount));
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_ordos_magma_crush_SpellScript::CountTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
            OnEffectHitTarget += SpellEffectFn(spell_ordos_magma_crush_SpellScript::SplitDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }

    private:
        uint32 targetsCount;
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_ordos_magma_crush_SpellScript();
    }
};

// 144689 - Burning Soul: when the debuff runs out, the player erupts (144690) on the people around.
class spell_ordos_burning_soul : public SpellScriptLoader
{
public:
    spell_ordos_burning_soul() : SpellScriptLoader("spell_ordos_burning_soul") { }

    class spell_ordos_burning_soul_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_ordos_burning_soul_AuraScript);

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            // Only a debuff that runs out erupts: not one that was dispelled, or that killed the player.
            if (GetTargetApplication()->GetRemoveMode() != AURA_REMOVE_BY_EXPIRE)
                return;

            if (Unit* owner = GetUnitOwner())
                owner->CastSpell(owner, SPELL_BURNING_SOUL_EFF, true);
        }

        void Register() OVERRIDE
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_ordos_burning_soul_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const OVERRIDE
    {
        return new spell_ordos_burning_soul_AuraScript();
    }
};

void AddSC_boss_ordos()
{
    new boss_ordos();
    new npc_ordos_ancient_flame();
    new spell_ordos_magma_crush();
    new spell_ordos_burning_soul();
}
