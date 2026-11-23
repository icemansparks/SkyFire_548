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

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "GameObject.h"
#include "Player.h"
#include "shadopan_monastery.h"

enum Spells
{
    SPELL_SMOKE_BLADES       = 106826,
    SPELL_SHA_SPIKE          = 106871,
    SPELL_DISORIENTING_SMASH = 106872,
    SPELL_ENRAGE             = 130196,
    SPELL_ICE_TRAP           = 110610,

    SPELL_SHA_FLAY           = 110119,
    SPELL_SHA_BLAST          = 110117,
};

enum Events
{
    EVENT_SMOKE_BLADES = 1,
    EVENT_SHA_SPIKE,
    EVENT_DISORIENTING_SMASH,

    // Prisoned sha
    EVENT_SHA_BLAST,
    EVENT_SHA_FLAY,
};

class boss_sha_of_violence : public CreatureScript
{
    public:
        boss_sha_of_violence() : CreatureScript("boss_sha_of_violence") { }

        enum eTalks
        {
            TALK_AGGRO = 0,
            TALK_DEATH = 1,
            TALK_SLAY  = 2
        };

        struct boss_sha_of_violenceAI : public BossAI
        {
            boss_sha_of_violenceAI(Creature* creature) : BossAI(creature, DATA_SHA_VIOLENCE)
            {
                enrageDone = false;
                shaCount = 0;
            }

            bool enrageDone;
            uint32 shaCount;
            std::vector<uint64> prisonedShaGUIDs;

            void InitializeAI() OVERRIDE
            {
                me->setActive(true);
                SetCombatMovement(false);
                Reset();
            }

            void Reset() OVERRIDE
            {
                _Reset();
                enrageDone = false;
                prisonedShaGUIDs.clear();
                summons.DespawnAll();

                shaCount = 0;
                me->GetMap()->SetWorldState(WORLDSTATE_OBVIOUS_SOLUTION, 0);

                // The five prisoned sha of the room; they stay until the containment override frees them.
                for (std::multimap<uint32, Position>::const_iterator itr = invPrisonedShaType.begin(); itr != invPrisonedShaType.end(); ++itr)
                    me->SummonCreature(itr->first, itr->second, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();
                Talk(TALK_AGGRO);
                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

                events.ScheduleEvent(EVENT_SMOKE_BLADES, urand(25000, 35000));
                events.ScheduleEvent(EVENT_SHA_SPIKE, urand(10000, 20000));
                events.ScheduleEvent(EVENT_DISORIENTING_SMASH, 13 * IN_MILLISECONDS);
            }

            void SetData(uint32 type, uint32 /*data*/) OVERRIDE
            {
                if (type == TYPE_OBVIOUS_SOLUTION && ++shaCount > 4)
                    me->GetMap()->SetWorldState(WORLDSTATE_OBVIOUS_SOLUTION, 1);
            }

            // The containment override frees the prisoned sha: they fight the group with the boss.
            void DoAction(int32 /*actionId*/) OVERRIDE
            {
                for (std::vector<uint64>::const_iterator itr = prisonedShaGUIDs.begin(); itr != prisonedShaGUIDs.end(); ++itr)
                    if (Creature* prisonedSha = Unit::GetCreature(*me, *itr))
                        prisonedSha->SetInCombatWithZone();
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                _JustDied();
                summons.DespawnAll();
                Talk(TALK_DEATH);

                if (instance)
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }

            void KilledUnit(Unit* victim) OVERRIDE
            {
                if (victim->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(TALK_SLAY);
            }

            void EnterEvadeMode() OVERRIDE
            {
                BossAI::EnterEvadeMode();

                if (!instance)
                    return;

                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                // Close the prisons and rearm the containment override for the next try.
                static uint32 const prisons[3] = { DATA_SHA_PRISON_1, DATA_SHA_PRISON_2, DATA_SHA_PRISON_3 };
                for (uint8 i = 0; i < 3; ++i)
                    if (GameObject* prison = GameObject::GetGameObject(*me, instance->GetData64(prisons[i])))
                        prison->SetGoState(GOState::GO_STATE_READY);

                if (GameObject* button = GameObject::GetGameObject(*me, instance->GetData64(DATA_CONTAINMENT_OVERRIDE)))
                {
                    button->SetGoState(GOState::GO_STATE_READY);
                    button->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_IN_USE);
                    button->SetLootState(LootState::GO_NOT_READY);
                }
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                summons.Summon(summon);

                if (summon->GetEntry() != NPC_VOLATILE_ENERGY && summon->GetEntry() != NPC_DESTROYING_SHA && summon->GetEntry() != NPC_SHA_MANIFESTATION)
                    summon->CastSpell(summon, SPELL_ICE_TRAP, true);
                else
                    prisonedShaGUIDs.push_back(summon->GetGUID());
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                if (!enrageDone && me->HealthBelowPctDamaged(20, damage))
                {
                    enrageDone = true;
                    me->CastSpell(me, SPELL_ENRAGE, true);
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_SMOKE_BLADES:
                        me->CastSpell(me, SPELL_SMOKE_BLADES, false);
                        events.ScheduleEvent(EVENT_SMOKE_BLADES, urand(25000, 35000));
                        break;
                    case EVENT_SHA_SPIKE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true))
                            me->CastSpell(target, SPELL_SHA_SPIKE, false);
                        events.ScheduleEvent(EVENT_SHA_SPIKE, urand(10000, 20000));
                        break;
                    case EVENT_DISORIENTING_SMASH:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true))
                            me->CastSpell(target, SPELL_DISORIENTING_SMASH, false);
                        events.ScheduleEvent(EVENT_DISORIENTING_SMASH, 13 * IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_sha_of_violenceAI(creature);
        }
};

// Lesser Volatile Energy: explodes when killed, or after 30 seconds.
class npc_sv_lesser : public CreatureScript
{
    public:
        npc_sv_lesser() : CreatureScript("npc_sv_lesser") { }

        enum eSpells
        {
            SPELL_EXPLOSION = 106966
        };

        enum eEvents
        {
            EVENT_DIE = 1
        };

        struct npc_sv_lesser_AI : public ScriptedAI
        {
            npc_sv_lesser_AI(Creature* creature) : ScriptedAI(creature), canDead(false) { }

            EventMap events;
            bool canDead;

            void InitializeAI() OVERRIDE
            {
                canDead = false;
                events.ScheduleEvent(EVENT_DIE, 30 * IN_MILLISECONDS);
            }

            void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
            {
                if (me->GetHealth() <= damage)
                {
                    damage = 0;
                    if (!canDead)
                    {
                        canDead = true;
                        me->CastSpell((Unit*)NULL, SPELL_EXPLOSION, false);
                        (attacker ? attacker : me)->Kill(me);
                        me->DespawnOrUnsummon(2 * IN_MILLISECONDS);
                    }
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (events.ExecuteEvent() == EVENT_DIE)
                    me->DealDamage(me, me->GetHealth());

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_sv_lesser_AI(creature);
        }
};

// Prisoned sha: Volatile Energy 56766, Sha Manifestation 58201
struct npc_achiev_prisoned_sha : public ScriptedAI
{
    npc_achiev_prisoned_sha(Creature* creature) : ScriptedAI(creature) { }

    EventMap events;

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        if (me->GetEntry() == NPC_SHA_MANIFESTATION)
        {
            events.ScheduleEvent(EVENT_SHA_BLAST, urand(4 * IN_MILLISECONDS, 8 * IN_MILLISECONDS));
            events.ScheduleEvent(EVENT_SHA_FLAY, 6 * IN_MILLISECONDS);
        }
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        // Only the prisoned sha the boss summons count for Obvious Solution, not the spawns of the room.
        if (me->GetDBTableGUIDLow())
            return;

        if (InstanceScript* instance = me->GetInstanceScript())
            if (Creature* shaOfViolence = Unit::GetCreature(*me, instance->GetData64(DATA_SHA_VIOLENCE)))
                if (shaOfViolence->IsAIEnabled)
                    shaOfViolence->AI()->SetData(TYPE_OBVIOUS_SOLUTION, 1);
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
                case EVENT_SHA_BLAST:
                    if (Unit* target = me->GetVictim())
                        DoCast(target, SPELL_SHA_BLAST);

                    events.ScheduleEvent(EVENT_SHA_BLAST, urand(6 * IN_MILLISECONDS, 8 * IN_MILLISECONDS));
                    break;
                case EVENT_SHA_FLAY:
                    DoCast(me, SPELL_SHA_FLAY);
                    events.ScheduleEvent(EVENT_SHA_FLAY, 15500);
                    break;
                default:
                    break;
            }
        }

        DoMeleeAttackIfReady();
    }
};

// Smoke Blades 106827: the area part keeps only the players that do not carry the aura yet.
class SmokeBladesTargetSelector
{
    public:
        bool operator()(WorldObject* object) const
        {
            return !(object->GetTypeId() == TypeID::TYPEID_PLAYER && !object->ToPlayer()->HasAura(SPELL_SMOKE_BLADES_PLAYER));
        }

    private:
        enum eSpells
        {
            SPELL_SMOKE_BLADES_PLAYER = 106827
        };
};

class spell_sv_smoke_blades : public SpellScriptLoader
{
    public:
        spell_sv_smoke_blades() : SpellScriptLoader("spell_sv_smoke_blades") { }

        enum eSpells
        {
            SPELL_PARTING_SMOKE = 127576
        };

        class spell_sv_smoke_blades_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sv_smoke_blades_AuraScript);

            // Every hit shortens the aura by 5 seconds.
            void HandleOnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
            {
                if (Unit* caster = eventInfo.GetActor())
                {
                    if (Aura* smoke = caster->GetAura(aurEff->GetId()))
                    {
                        if (smoke->GetDuration() > 5 * IN_MILLISECONDS)
                            smoke->SetDuration(smoke->GetDuration() - 5 * IN_MILLISECONDS);
                        else
                            caster->RemoveAura(aurEff->GetId());
                    }
                }
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* owner = GetOwner()->ToPlayer())
                    owner->CastSpell(owner, SPELL_PARTING_SMOKE, false);
            }

            void Register() OVERRIDE
            {
                OnEffectProc += AuraEffectProcFn(spell_sv_smoke_blades_AuraScript::HandleOnProc, EFFECT_1, SPELL_AURA_DUMMY);
                AfterEffectRemove += AuraEffectRemoveFn(spell_sv_smoke_blades_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        class spell_sv_smoke_blades_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sv_smoke_blades_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                targets.remove_if(SmokeBladesTargetSelector());
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sv_smoke_blades_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_sv_smoke_blades_AuraScript();
        }

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_sv_smoke_blades_SpellScript();
        }
};

// Containment Override 213457: pressing it opens the three prisons and sends the prisoned sha at the group.
class go_containment_override : public GameObjectScript
{
    public:
        go_containment_override() : GameObjectScript("go_containment_override") { }

        void OnGameObjectStateChanged(GameObject* go, uint32 state) OVERRIDE
        {
            if (state != uint32(GOState::GO_STATE_ACTIVE))
                return;

            InstanceScript* instance = go->GetInstanceScript();
            if (!instance)
                return;

            static uint32 const prisons[3] = { DATA_SHA_PRISON_1, DATA_SHA_PRISON_2, DATA_SHA_PRISON_3 };
            for (uint8 i = 0; i < 3; ++i)
                if (GameObject* prison = GameObject::GetGameObject(*go, instance->GetData64(prisons[i])))
                    prison->SetGoState(GOState::GO_STATE_ACTIVE);

            if (Creature* shaOfViolence = Unit::GetCreature(*go, instance->GetData64(DATA_SHA_VIOLENCE)))
                if (shaOfViolence->IsAIEnabled)
                    shaOfViolence->AI()->DoAction(0);
        }
};

void AddSC_boss_sha_of_violence()
{
    new boss_sha_of_violence();
    new npc_sv_lesser();
    new creature_script<npc_achiev_prisoned_sha>("npc_achiev_prisoned_sha");
    new spell_sv_smoke_blades();
    new go_containment_override();
}
