/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_Oondasta
SD%Complete: 75
SDComment: Isle of Giants world boss. The Dohaman the Beast Lord intro is left out:
           his template is missing on this database.
SDCategory: Isle of Giants
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "Player.h"

enum OondastaSpells
{
    SPELL_CRUSH             = 137504,
    SPELL_ALPHA_MALE        = 138390,
    SPELL_FRILL_BLAST       = 137505,
    SPELL_GROWING_FURY      = 137502,
    SPELL_PIERCING_ROAR     = 137457,
    SPELL_SPIRITFIRE_BEAM   = 137508
};

enum OondastaEvents
{
    EVENT_CRUSH             = 1,
    EVENT_FRILL_BLAST,
    EVENT_GROWING_FURY,
    EVENT_PIERCING_ROAR,
    EVENT_SPIRITFIRE_BEAM,
    EVENT_SPIRITFIRE_BEAM_EXTRA
};

static bool IsTankSpec(Player const* player)
{
    switch (player->GetTalentSpecialization(player->GetActiveSpec()))
    {
        case SPEC_PALADIN_PROTECTION:
        case SPEC_WARRIOR_PROTECTION:
        case SPEC_DRUID_GUARDIAN:
        case SPEC_DEATH_KNIGHT_BLOOD:
        case SPEC_MONK_BREWMASTER:
            return true;
        default:
            return false;
    }
}

class boss_oondasta : public CreatureScript
{
public:
    boss_oondasta() : CreatureScript("boss_oondasta") { }

    struct boss_oondastaAI : public ScriptedAI
    {
        boss_oondastaAI(Creature* creature) : ScriptedAI(creature)
        {
            // Alpha Male decides who tanks Oondasta, so taunts don't work on it.
            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);
        }

        void Reset() OVERRIDE
        {
            events.Reset();
            spiritfireChain = 0;
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            DoCast(me, SPELL_ALPHA_MALE, true);
            events.ScheduleEvent(EVENT_CRUSH, 3 * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_SPIRITFIRE_BEAM, 5 * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_GROWING_FURY, 18500);
            events.ScheduleEvent(EVENT_PIERCING_ROAR, 20 * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_FRILL_BLAST, 40 * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_SPIRITFIRE_BEAM_EXTRA, urand(35, 60) * IN_MILLISECONDS);
        }

        // Spiritfire Beam prefers players who are not tanking: first a non-tank spec,
        // then anyone but the current victim, then the victim.
        Unit* SelectSpiritfireTarget()
        {
            std::list<Unit*> candidates;
            std::list<Unit*> fallback;
            ThreatContainer::StorageType const& threatList = me->getThreatManager().getThreatList();
            for (ThreatContainer::StorageType::const_iterator itr = threatList.begin(); itr != threatList.end(); ++itr)
            {
                Player* player = ObjectAccessor::GetPlayer(*me, (*itr)->getUnitGuid());
                if (!player || !player->IsAlive() || player == me->GetVictim())
                    continue;
                if (IsTankSpec(player))
                    fallback.push_back(player);
                else
                    candidates.push_back(player);
            }

            if (candidates.empty())
                candidates.swap(fallback);
            if (candidates.empty())
                return me->GetVictim();

            Skyfire::Containers::RandomResizeList(candidates, 1);
            return candidates.front();
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
                    case EVENT_CRUSH:
                        DoCastVictim(SPELL_CRUSH);
                        events.ScheduleEvent(EVENT_CRUSH, 30 * IN_MILLISECONDS);
                        break;
                    case EVENT_FRILL_BLAST:
                        DoCastVictim(SPELL_FRILL_BLAST);
                        events.ScheduleEvent(EVENT_FRILL_BLAST, urand(25, 30) * IN_MILLISECONDS);
                        break;
                    case EVENT_PIERCING_ROAR:
                        DoCast(me, SPELL_PIERCING_ROAR);
                        events.ScheduleEvent(EVENT_PIERCING_ROAR, urand(26, 60) * IN_MILLISECONDS);
                        break;
                    case EVENT_SPIRITFIRE_BEAM:
                        if (Unit* target = SelectSpiritfireTarget())
                            DoCast(target, SPELL_SPIRITFIRE_BEAM);
                        // Two beams 5 s apart, then a 45 s pause.
                        spiritfireChain = (spiritfireChain + 1) % 2;
                        events.ScheduleEvent(EVENT_SPIRITFIRE_BEAM, (spiritfireChain ? 5 : 45) * IN_MILLISECONDS);
                        break;
                    case EVENT_SPIRITFIRE_BEAM_EXTRA:
                        if (Unit* target = SelectSpiritfireTarget())
                            DoCast(target, SPELL_SPIRITFIRE_BEAM);
                        events.ScheduleEvent(EVENT_SPIRITFIRE_BEAM_EXTRA, urand(35, 60) * IN_MILLISECONDS);
                        break;
                    case EVENT_GROWING_FURY:
                        DoCast(me, SPELL_GROWING_FURY);
                        events.ScheduleEvent(EVENT_GROWING_FURY, 30 * IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        EventMap events;
        uint8 spiritfireChain;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_oondastaAI(creature);
    }
};

// 138390 - Alpha Male threat effect: only players in a tank specialization get the threat bonus.
class spell_alpha_male_eff : public SpellScriptLoader
{
public:
    spell_alpha_male_eff() : SpellScriptLoader("spell_alpha_male_eff") { }

    class spell_alpha_male_eff_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_alpha_male_eff_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            for (std::list<WorldObject*>::iterator itr = targets.begin(); itr != targets.end();)
            {
                Player* player = (*itr)->ToPlayer();
                if (!player || !IsTankSpec(player))
                    itr = targets.erase(itr);
                else
                    ++itr;
            }
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_alpha_male_eff_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_alpha_male_eff_SpellScript();
    }
};

void AddSC_boss_oondasta()
{
    new boss_oondasta();
    new spell_alpha_male_eff();
}
