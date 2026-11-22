/*
* This file is part of the Pandaria 5.4.8 Project. See THANKS file for Copyright information
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

// Ported from the Pandaria 5.4.8 Project (alexkulya) and adapted to SkyFire's script API.
// Spell scripts of the Master Snowdrift event.

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Containers.h"
#include "Player.h"
#include "shadopan_monastery.h"

#define SPELL_ROOT_FOR_EVER 31366

class spell_spm_swirling_steel : public SpellScriptLoader
{
    public:
        spell_spm_swirling_steel() : SpellScriptLoader("spell_spm_swirling_steel") { }

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_spm_swirling_steel_SpellScript();
        }

        class spell_spm_swirling_steel_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_spm_swirling_steel_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                if (!targets.empty())
                    Skyfire::Containers::RandomResizeList(targets, 1);
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_spm_swirling_steel_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };
};

class BallOfFireTargetSelector
{
    public:
        bool operator()(WorldObject* object) const
        {
            if (object->GetEntry() == NPC_SNOWDRIFT_CLONE)
                return false;

            return true;
        }
};

class spell_snowdrift_ball_of_fire : public SpellScriptLoader
{
    public:
        spell_snowdrift_ball_of_fire() : SpellScriptLoader("spell_snowdrift_ball_of_fire") { }

        enum eSpells
        {
            SPELL_BALL_OF_FIRE = 113760
        };

        class spell_snowdrift_ball_of_fire_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_snowdrift_ball_of_fire_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if (BallOfFireTargetSelector());
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_snowdrift_ball_of_fire_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        class spell_snowdrift_ball_of_fire_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_snowdrift_ball_of_fire_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                auto const owner = GetOwner()->ToCreature();

                if (owner)
                {
                    std::list<Player*> playerList;
                    GetPlayerListInGrid(playerList, owner, 150.0f);
                    if (!playerList.empty())
                    {
                        for (std::list<Player*>::iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
                        {
                            if (auto const player = (*itr)->ToPlayer())
                            {
                                if (player->isInFront(owner, M_PI))
                                {
                                    owner->SetFacingToObject(player);
                                    owner->SetOrientation(owner->GetAngle(player));
                                    break;
                                }
                            }
                        }
                    }

                    owner->CastSpell(owner, SPELL_BALL_OF_FIRE, true);
                }
            }

            void Register() OVERRIDE
            {
                OnEffectApply += AuraEffectApplyFn(spell_snowdrift_ball_of_fire_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_snowdrift_ball_of_fire_SpellScript();
        }

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_snowdrift_ball_of_fire_AuraScript();
        }
};

class spell_snowdrift_ball_of_fire_aura : public SpellScriptLoader
{
    public:
        spell_snowdrift_ball_of_fire_aura() : SpellScriptLoader("spell_snowdrift_ball_of_fire_aura") { }

        enum eSpells
        {
            SPELL_BALL_OF_FIRE = 113760
        };

        class spell_snowdrift_ball_of_fire_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_snowdrift_ball_of_fire_aura_AuraScript);

            void OnPeriodic(AuraEffect const* /*aurEff*/)
            {
                auto const owner = GetOwner()->ToCreature();

                if (owner)
                {
                    std::list<Player*> playerList;
                    GetPlayerListInGrid(playerList, owner, 150.0f);
                    if (!playerList.empty())
                    {
                        for (std::list<Player*>::iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
                        {
                            if (auto const player = (*itr)->ToPlayer())
                            {
                                if (player->isInFront(owner, M_PI))
                                {
                                    owner->SetFacingToObject(player);
                                    owner->SetOrientation(owner->GetAngle(player));
                                    break;
                                }
                            }
                        }
                    }

                    owner->CastSpell(owner, SPELL_BALL_OF_FIRE, true);
                }
            }

            void Register() OVERRIDE
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_snowdrift_ball_of_fire_aura_AuraScript::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_snowdrift_ball_of_fire_aura_AuraScript();
        }
};

class spell_snowdrift_hurl_chi : public SpellScriptLoader
{
    public:
        spell_snowdrift_hurl_chi() : SpellScriptLoader("spell_snowdrift_hurl_chi") { }

        class spell_snowdrift_hurl_chi_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_snowdrift_hurl_chi_AuraScript);

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (auto const caster = GetCaster())
                   if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_EXPIRE)
                       if (caster->IsAIEnabled)
                           caster->GetAI()->DoAction(6);
            }

            void Register() OVERRIDE
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_snowdrift_hurl_chi_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_snowdrift_hurl_chi_AuraScript();
        }
};

class spell_snowdrift_tornado_slam : public SpellScriptLoader
{
    public:
        spell_snowdrift_tornado_slam() : SpellScriptLoader("spell_snowdrift_tornado_slam") { }

        enum eSpells
        {
            SPELL_TORNADO_SLAM_EFFECT = 106345,
            SPELL_CHASE_DOWN          = 118961,
        };

        class spell_snowdrift_tornado_slam_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_snowdrift_tornado_slam_SpellScript);

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                auto const target = GetHitUnit();
                auto const caster = GetCaster();
                if (!target || !caster)
                    return;

                caster->RemoveAura(SPELL_CHASE_DOWN);
                target->CastSpell((Unit*)NULL, SPELL_TORNADO_SLAM_EFFECT, false);
                target->AddAura(SPELL_ROOT_FOR_EVER, target);
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_snowdrift_tornado_slam_SpellScript::HandleScript, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_snowdrift_tornado_slam_SpellScript();
        }
};

class spell_snowdrift_tornado_slam_effect : public SpellScriptLoader
{
    public:
        spell_snowdrift_tornado_slam_effect() : SpellScriptLoader("spell_snowdrift_tornado_slam_effect") { }

        enum eSpells
        {
            SPELL_EJECT_PASS = 68576
        };

        class spell_snowdrift_tornado_slam_effect_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_snowdrift_tornado_slam_effect_SpellScript);

            void HandleScript(SpellEffIndex effIndex)
            {
                auto const caster = GetCaster();
                if (!caster)
                    return;

                caster->CastSpell(caster, SPELL_EJECT_PASS, TRIGGERED_FULL_MASK);
                caster->RemoveAura(SPELL_ROOT_FOR_EVER);
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_snowdrift_tornado_slam_effect_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_DAMAGE_FROM_MAX_HEALTH_PCT);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_snowdrift_tornado_slam_effect_SpellScript();
        }
};

class spell_snowdrift_parry_stance : public SpellScriptLoader
{
    public:
        spell_snowdrift_parry_stance() : SpellScriptLoader("spell_snowdrift_parry_stance") { }

        enum eSpells
        {
            SPELL_FLYING_KICK    = 106439,
            SPELL_QUIVERING_PALM = 106422
        };

        class spell_snowdrift_parry_stance_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_snowdrift_parry_stance_AuraScript);

            void HanleOnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
            {
                auto const caster = eventInfo.GetActionTarget();
                auto const target = eventInfo.GetActor();
                if (!caster || !target)
                    return;

                if (auto const aura = caster->GetAura(aurEff->GetId()))
                {
                    if (aura->GetDuration() > 1.5 * IN_MILLISECONDS)
                        caster->CastSpell(target, SPELL_QUIVERING_PALM, true);
                    else
                        caster->CastSpell(target, SPELL_FLYING_KICK, true);
                }
            }

            void Register() OVERRIDE
            {
                OnEffectProc += AuraEffectProcFn(spell_snowdrift_parry_stance_AuraScript::HanleOnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_snowdrift_parry_stance_AuraScript();
        }
};

class spell_snowdrift_copy_health : public SpellScriptLoader
{
    public:
        spell_snowdrift_copy_health() : SpellScriptLoader("spell_snowdrift_copy_health") { }

        class spell_snowdrift_copy_health_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_snowdrift_copy_health_SpellScript);

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                auto const target = GetHitUnit();
                auto const caster = GetCaster();
                if (!target || !caster)
                    return;

                caster->SetHealth(caster->CountPctFromMaxHealth(target->GetHealthPct()));
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_snowdrift_copy_health_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_snowdrift_copy_health_SpellScript();
        }
};

class BallOfFireDamageTargetSelector
{
    public:
        explicit BallOfFireDamageTargetSelector(Unit* _caster) : caster(_caster) { }

        bool operator() (WorldObject* unit) const
        {
            if (unit->GetDistance(caster) <= 0.5 && unit->GetTypeId() == TypeID::TYPEID_PLAYER && !unit->ToPlayer()->HasAura(SPELL_BALL_OF_FIRE_AURA_BLUE) && !unit->ToPlayer()->HasAura(SPELL_BALL_OF_FIRE_AURA_RED))
                return false;

            return true;
        }

    private:
        Unit* caster;

        enum eSpells
        {
            SPELL_BALL_OF_FIRE_AURA_BLUE = 115324,
            SPELL_BALL_OF_FIRE_AURA_RED  = 115325
        };
};

class spell_snowdrift_ball_of_fire_damage : public SpellScriptLoader
{
    public:
        spell_snowdrift_ball_of_fire_damage() : SpellScriptLoader("spell_snowdrift_ball_of_fire_damage") { }

        class spell_snowdrift_ball_of_fire_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_snowdrift_ball_of_fire_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if (BallOfFireDamageTargetSelector(GetCaster()));
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_snowdrift_ball_of_fire_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        class spell_snowdrift_ball_of_fire_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_snowdrift_ball_of_fire_AuraScript);

            void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                auto const caster = GetCaster();
                auto const owner = GetOwner()->ToUnit();
                if (!caster || !owner)
                    return;

                caster->CastSpell(owner, aurEff->GetSpellInfo()->Effects[0].BasePoints, TRIGGERED_FULL_MASK);
            }

            void Register() OVERRIDE
            {
                OnEffectApply += AuraEffectApplyFn(spell_snowdrift_ball_of_fire_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_snowdrift_ball_of_fire_SpellScript();
        }

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_snowdrift_ball_of_fire_AuraScript();
        }
};

class spell_snowdrift_fists_of_fury : public SpellScriptLoader
{
    public:
        spell_snowdrift_fists_of_fury() : SpellScriptLoader("spell_snowdrift_fists_of_fury") { }

        class spell_snowdrift_fists_of_fury_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_snowdrift_fists_of_fury_SpellScript);

            void HandleBeforeCast()
            {
                auto const caster = GetCaster();
                if (!caster)
                    return;

                if (caster->ToCreature())
                    caster->ToCreature()->SetReactState(REACT_PASSIVE);
            }

            void Register() OVERRIDE
            {
                BeforeCast += SpellCastFn(spell_snowdrift_fists_of_fury_SpellScript::HandleBeforeCast);
            }
        };

        class spell_snowdrift_fists_of_fury_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_snowdrift_fists_of_fury_AuraScript);

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                auto const owner = GetOwner()->ToCreature();
                if (!owner)
                    return;

                owner->SetReactState(REACT_AGGRESSIVE);
            }

            void Register() OVERRIDE
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_snowdrift_fists_of_fury_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_snowdrift_fists_of_fury_SpellScript();
        }

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_snowdrift_fists_of_fury_AuraScript();
        }
};

class spell_spm_parry_stance : public SpellScriptLoader
{
    public:
        spell_spm_parry_stance() : SpellScriptLoader("spell_spm_parry_stance") { }

        class spell_spm_parry_stance_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_spm_parry_stance_AuraScript);

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                auto const owner = GetOwner()->ToCreature();
                if (!owner)
                    return;

                if (owner->IsAIEnabled)
                    owner->AI()->DoZoneInCombat();
            }

            void Register() OVERRIDE
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_spm_parry_stance_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_PACIFY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_spm_parry_stance_AuraScript();
        }
};

void AddSC_master_snowdrift_spells()
{
    new spell_spm_swirling_steel();
    new spell_snowdrift_ball_of_fire();
    new spell_snowdrift_ball_of_fire_aura();
    new spell_snowdrift_hurl_chi();
    new spell_snowdrift_tornado_slam();
    new spell_snowdrift_tornado_slam_effect();
    new spell_snowdrift_parry_stance();
    new spell_snowdrift_copy_health();
    new spell_snowdrift_ball_of_fire_damage();
    new spell_snowdrift_fists_of_fury();
    new spell_spm_parry_stance();
}
