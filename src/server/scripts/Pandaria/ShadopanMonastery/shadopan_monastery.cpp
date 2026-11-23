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
// Spell scripts of the trash and the boss encounters come in later PRs.

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "shadopan_monastery.h"

class npc_shadopan_ambusher : public CreatureScript
{
    public:
        npc_shadopan_ambusher() : CreatureScript("npc_shadopan_ambusher") { }

        enum eSpells
        {
            SPELL_SHA_CORRUPTION   = 124337,
            SPELL_STEALTH_COSMETIC = 91194,
            SPELL_CRISE            = 128245,
            SPELL_FLIP_OUT_AURA    = 128248,
            SPELL_STEALTH          = 102921,
            SPELL_SHADOWSTEP       = 128766 
        };

        enum eEvents
        {
            EVENT_CRISE      = 1,
            EVENT_SHADOWSTEP = 2
        };

        struct npc_shadopan_ambusherAI : public ScriptedAI
        {
            npc_shadopan_ambusherAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();
                me->CastSpell(me, SPELL_STEALTH, false);
                me->CastSpell(me, SPELL_STEALTH_COSMETIC, false);
                me->CastSpell(me, SPELL_SHA_CORRUPTION, false);
                me->RemoveAurasDueToSpell(SPELL_FLIP_OUT_AURA);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                me->RemoveAura(SPELL_STEALTH);
                events.ScheduleEvent(EVENT_CRISE, urand(6 * IN_MILLISECONDS, 8 * IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_SHADOWSTEP, 0.5 * IN_MILLISECONDS);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CRISE:
                            me->CastSpell(me, SPELL_CRISE, false);
                            events.ScheduleEvent(EVENT_CRISE, urand(10 * IN_MILLISECONDS, 14 * IN_MILLISECONDS));
                            break;
                        case EVENT_SHADOWSTEP:
                            if (auto const target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true))
                                me->CastSpell(target, SPELL_SHADOWSTEP, true);
                            break;
                    }
                }

               DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_shadopan_ambusherAI(creature);
        }
};

const float spawnAngles[5] = {M_PI, -M_PI / 2, M_PI / 2, -3 * M_PI / 4, 3 * M_PI / 4};

class npc_shadopan_warden: public CreatureScript
{
    public:
        npc_shadopan_warden() : CreatureScript("npc_shadopan_warden") { }

        enum eSpells
        {
            SPELL_INCAPACITATING_LEAP = 115517
        };

        enum eEvents
        {
            EVENT_INCAPACITATING_LEAP = 1
        };

        enum eTalks
        {
            TALK_AGGRO = 0
        };

        struct npc_shadopan_wardenAI : public ScriptedAI
        {
            npc_shadopan_wardenAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;
            bool aggroTalked;
            uint8 aggroTalk;

            void InitializeAI() OVERRIDE
            {
                // The AI is built again on every respawn: keep the ambushers of the previous batch if any of them still lives.
                bool ownAmbusherAlive = false;
                std::list<Creature*> ambushers;
                me->GetCreatureListWithEntryInGrid(ambushers, NPC_SHADO_PAN_AMBUSHER, 200.0f);
                for (std::list<Creature*>::const_iterator itr = ambushers.begin(); itr != ambushers.end(); ++itr)
                    if (TempSummon* summon = (*itr)->ToTempSummon())
                        if ((*itr)->IsAlive() && summon->GetSummonerGUID() == me->GetGUID())
                            ownAmbusherAlive = true;

                if (me->IsAlive() && !ownAmbusherAlive)
                {
                    for (int i = 0; i < 5; ++i)
                    {
                        float x, y, z;
                        z = me->GetPositionZ();
                        me->GetNearPoint2D(x, y, 3.0f, spawnAngles[i]);
                        me->SummonCreature(NPC_SHADO_PAN_AMBUSHER, x, y, z, me->GetAngle(x, y), TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, 604800);
                    }
                }

                aggroTalked = false;

                Reset();
            }

            void Reset() OVERRIDE
            {
                events.Reset();
            }
     
            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                if (!aggroTalked)
                {
                    aggroTalked = true;
                    Talk(TALK_AGGRO);
                }

                events.ScheduleEvent(EVENT_INCAPACITATING_LEAP, urand(10 * IN_MILLISECONDS, 14 * IN_MILLISECONDS));
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                if (events.ExecuteEvent() == EVENT_INCAPACITATING_LEAP)
                {
                    if (auto const target = SelectTarget(SELECT_TARGET_RANDOM))
                        me->CastSpell(target, SPELL_INCAPACITATING_LEAP, false);
                    events.ScheduleEvent(EVENT_INCAPACITATING_LEAP, urand(10 * IN_MILLISECONDS, 14 * IN_MILLISECONDS));
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_shadopan_wardenAI(creature);
        }
};

class npc_shadopan_stormbringer : public CreatureScript
{
    public:
        npc_shadopan_stormbringer() : CreatureScript("npc_shadopan_stormbringer") { }

        enum eSpells
        {
            SPELL_FOCUSING_ENERGY = 115009,
            SPELL_FOCUSING_COMBAT = 115433,
            SPELL_ARC_LIGHTNING   = 115026 
        };

        enum eEvents
        {
            EVENT_FOCUSING_COMBAT = 1
        };

        struct npc_shadopan_stormbringerAI : public ScriptedAI
        {
            npc_shadopan_stormbringerAI(Creature* creature) : ScriptedAI(creature), focusingTimer(0) { }

            EventMap events;
            uint32 focusingTimer;

            void InitializeAI() OVERRIDE
            {
                SetCombatMovement(false);

                Reset();
            }

            void Reset() OVERRIDE
            {
                events.Reset();
                if (auto const energy = me->FindNearestCreature(NPC_UNSTABLE_ENERGY, 10.0f))
                    if (energy->HasAura(SPELL_FOCUSING_COMBAT))
                        energy->RemoveAura(SPELL_FOCUSING_COMBAT);
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_FOCUSING_COMBAT, urand(4 * IN_MILLISECONDS, 8 * IN_MILLISECONDS));
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                if (auto const energy = me->FindNearestCreature(NPC_UNSTABLE_ENERGY, 10.0f))
                    if (energy->IsAIEnabled)
                        energy->AI()->DoAction(0); // ACTION_STORMBRINGER_KILLED
            }

            void SpellHitTarget(Unit* target, const SpellInfo* spell) OVERRIDE
            {
                if (spell->Id == SPELL_FOCUSING_COMBAT)
                {
                    if (auto const sTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true))
                        target->CastSpell(sTarget, SPELL_ARC_LIGHTNING, false);
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                // Re-link to the energy at most once a second instead of on every tick.
                if (focusingTimer <= diff)
                {
                    focusingTimer = 1 * IN_MILLISECONDS;
                    if (!me->HasUnitState(UNIT_STATE_CASTING))
                        if (auto const energy = me->FindNearestCreature(NPC_UNSTABLE_ENERGY, 10.0f))
                            me->CastSpell(energy, SPELL_FOCUSING_ENERGY, false);
                }
                else
                    focusingTimer -= diff;

                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (events.ExecuteEvent() == EVENT_FOCUSING_COMBAT)
                {
                    if (auto const energy = me->FindNearestCreature(NPC_UNSTABLE_ENERGY, 10.0f))
                        me->CastSpell(energy, SPELL_FOCUSING_COMBAT, true);
                    events.ScheduleEvent(EVENT_FOCUSING_COMBAT, urand(6 * IN_MILLISECONDS, 14 * IN_MILLISECONDS));
                }

                if (!me->HasUnitState(UNIT_STATE_CASTING))
                    DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_shadopan_stormbringerAI(creature);
        }
};

class npc_shadopan_disciple : public CreatureScript
{
    public:
        npc_shadopan_disciple() : CreatureScript("npc_shadopan_disciple") { }

        enum eSpells
        {
            SPELL_PALM_STRIKE       = 106421,
            SPELL_ROUNDHOUSE_STRIKE = 128304
        };

        enum eEvents
        {
            EVENT_PALM_STRIKE       = 1,
            EVENT_ROUNDHOUSE_STRIKE = 2
        };

        struct npc_shadopan_discipleAI : public ScriptedAI
        {
            npc_shadopan_discipleAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_PALM_STRIKE, urand(4 * IN_MILLISECONDS, 8 * IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_ROUNDHOUSE_STRIKE, urand(6 * IN_MILLISECONDS, 15 * IN_MILLISECONDS));
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_PALM_STRIKE:
                            me->CastSpell((Unit*)NULL, SPELL_PALM_STRIKE, false);
                            events.ScheduleEvent(EVENT_PALM_STRIKE, urand(16 * IN_MILLISECONDS, 21 * IN_MILLISECONDS));
                            break;
                        case EVENT_ROUNDHOUSE_STRIKE:
                            me->CastSpell((Unit*)NULL, SPELL_ROUNDHOUSE_STRIKE, false);
                            events.ScheduleEvent(EVENT_ROUNDHOUSE_STRIKE, urand(13 * IN_MILLISECONDS, 20 * IN_MILLISECONDS));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_shadopan_discipleAI(creature);
        }
};

class npc_shadopan_unstable_energy : public CreatureScript
{
    public:
        npc_shadopan_unstable_energy() : CreatureScript("npc_shadopan_unstable_energy") { }

        enum eActions
        {
            ACTION_STORMBRINGER_KILLED = 0
        };

        struct npc_shadopan_unstable_energyAI : public ScriptedAI
        {
            npc_shadopan_unstable_energyAI(Creature* creature) : ScriptedAI(creature) { }

            uint8 stormbringerKilled;

            void InitializeAI() OVERRIDE
            {
                stormbringerKilled = 0;
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                if (actionId == ACTION_STORMBRINGER_KILLED)
                {
                    if (++stormbringerKilled == 6)
                        me->DespawnOrUnsummon(2 * IN_MILLISECONDS);
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_shadopan_unstable_energyAI(creature);
        }
};

class npc_ethereal_sha : public CreatureScript
{
    public:
        npc_ethereal_sha() : CreatureScript("npc_ethereal_sha") { }

        enum eSpells
        {
            SPELL_SHA_BLAST              = 110117,
            SPELL_REACH_THROUGH_THE_VOID = 128339
        };

        enum eEvents
        {
            EVENT_SHA_BLAST              = 1,
            EVENT_REACH_THROUGH_THE_VOID = 2
        };

        struct npc_ethereal_shaAI : public ScriptedAI
        {
            npc_ethereal_shaAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_SHA_BLAST, urand(4 * IN_MILLISECONDS, 8 * IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_REACH_THROUGH_THE_VOID, urand(11 * IN_MILLISECONDS, 13 * IN_MILLISECONDS));
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SHA_BLAST:
                            me->CastSpell(me->GetVictim(), SPELL_SHA_BLAST, false);
                            events.ScheduleEvent(EVENT_SHA_BLAST, urand(12 * IN_MILLISECONDS, 14 * IN_MILLISECONDS));
                            break;
                        case EVENT_REACH_THROUGH_THE_VOID:
                            me->CastSpell(me, SPELL_REACH_THROUGH_THE_VOID, false);
                            events.ScheduleEvent(EVENT_REACH_THROUGH_THE_VOID, urand(18 * IN_MILLISECONDS, 23 * IN_MILLISECONDS));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_ethereal_shaAI(creature);
        }
};

class npc_spm_void_sha : public CreatureScript
{
    public:
        npc_spm_void_sha() : CreatureScript("npc_spm_void_sha") { }

        enum eSpells
        {
            SPELL_ON_SPAWN    = 128342,
            SPELL_VOID_ENERGY = 128343
        };

        enum eEvents
        {
            EVENT_VOID_ENERGY = 1
        };

        struct npc_spm_void_shAI : public ScriptedAI
        {
            npc_spm_void_shAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void InitializeAI() OVERRIDE
            {
                me->SetReactState(REACT_AGGRESSIVE);
                me->CastSpell(me, SPELL_ON_SPAWN, false);

                Reset();
            }

            void Reset() OVERRIDE
            {
                DoZoneInCombat();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_VOID_ENERGY, 5 * IN_MILLISECONDS);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (events.ExecuteEvent() == EVENT_VOID_ENERGY)
                {
                    me->CastSpell((Unit*)NULL, SPELL_VOID_ENERGY, false);
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_spm_void_shAI(creature);
        }
};

class npc_consuming_sha : public CreatureScript
{
    public:
        npc_consuming_sha() : CreatureScript("npc_consuming_sha") { }

        enum eSpells
        {
            SPELL_CONSUMING_BITE = 106925,
            SPELL_CONSUMPTION    = 106929
        };

        enum eEvents
        {
            EVENT_CONSUMING_BITE = 1,
            EVENT_CONSUMPTION    = 2
        };

        struct npc_consuming_shaAI : public ScriptedAI
        {
            npc_consuming_shaAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_CONSUMING_BITE, urand(4 * IN_MILLISECONDS, 8 * IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_CONSUMPTION, urand(6 * IN_MILLISECONDS, 10 * IN_MILLISECONDS));
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CONSUMING_BITE:
                            me->CastSpell((Unit*)NULL, SPELL_CONSUMING_BITE, false);
                            events.ScheduleEvent(EVENT_CONSUMING_BITE, urand(10 * IN_MILLISECONDS, 14 * IN_MILLISECONDS));
                            break;
                        case EVENT_CONSUMPTION:
                            me->CastSpell((Unit*)NULL, SPELL_CONSUMPTION, false);
                            events.ScheduleEvent(EVENT_CONSUMPTION, urand(15 * IN_MILLISECONDS, 19 * IN_MILLISECONDS));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_consuming_shaAI(creature);
        }
    };

class npc_destroying_sha : public CreatureScript
{
    public:
        npc_destroying_sha() : CreatureScript("npc_destroying_sha") { }

        enum eSpells
        {
            SPELL_SHADOWS_OF_DESTRUCTION = 106942
        };

        enum eEvents
        {
            EVENT_SHADOWS_OF_DESTRUCTION = 1
        };

        struct npc_destroying_shaAI : public ScriptedAI
        {
            npc_destroying_shaAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_SHADOWS_OF_DESTRUCTION, urand(4 * IN_MILLISECONDS, 8 * IN_MILLISECONDS));
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

                if (events.ExecuteEvent() == EVENT_SHADOWS_OF_DESTRUCTION)
                {
                    me->CastSpell((Unit*)NULL, SPELL_SHADOWS_OF_DESTRUCTION, false);
                    events.ScheduleEvent(EVENT_SHADOWS_OF_DESTRUCTION, urand(16 * IN_MILLISECONDS, 20 * IN_MILLISECONDS));
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_destroying_shaAI(creature);
        }
};

class npc_shadopan_novice : public CreatureScript
{
    public:
        npc_shadopan_novice() : CreatureScript("npc_shadopan_novice") { }

        struct npc_shadopan_noviceAI : public ScriptedAI
        {
            npc_shadopan_noviceAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE
            {
                if (auto const training = me->FindNearestCreature(NPC_TRAINING_TARGET, 10.0f))
                    AttackStart(training);
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_shadopan_noviceAI(creature);
        }
};

class npc_shadopan_archery : public CreatureScript
{
    public:
        npc_shadopan_archery() : CreatureScript("npc_shadopan_archery") { }

        enum eEvents
        {
            EVENT_ICE_ARROW  = 1,
            EVENT_FIRE_ARROW = 2
        };

        enum eSpells
        {
            SPELL_ICE_ARROW = 126114,
            SPELL_FIRE_ARROW = 106992
        };

        struct npc_shadopan_archeryAI : public ScriptedAI
        {
            npc_shadopan_archeryAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;
            InstanceScript* instance;      

            void InitializeAI() OVERRIDE
            {
                instance = me->GetInstanceScript();
                me->setActive(true);
                switch (me->GetEntry())
                {
                    case NPC_ARCHERY_FIRST:
                        if (instance)
                            events.ScheduleEvent(EVENT_ICE_ARROW, 2 * IN_MILLISECONDS);
                        break;
                    case NPC_ARCHERY_SECOND:
                        if (instance)
                            events.ScheduleEvent(EVENT_FIRE_ARROW, 2 * IN_MILLISECONDS);
                        break;
                }
            }

            uint64 ArcheryTarget()
            {
                std::list<Creature*> ArcheryTargets;
                me->GetCreatureListWithEntryInGrid(ArcheryTargets, NPC_ARCHERY_TARGET, 200.0f);

                if (ArcheryTargets.empty())
                    return 0;

                return Skyfire::Containers::SelectRandomContainerElement(ArcheryTargets)->GetGUID();
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_ICE_ARROW:
                            me->UpdateObjectVisibility();

                            if (Creature* ArcherTarget = ObjectAccessor::GetCreature(*me, ArcheryTarget()))
                                me->CastSpell(ArcherTarget, SPELL_ICE_ARROW, false);

                            events.ScheduleEvent(EVENT_ICE_ARROW, urand(1 * IN_MILLISECONDS, 2 * IN_MILLISECONDS));
                            break;
                        case EVENT_FIRE_ARROW:
                            me->CastSpell(me, SPELL_FIRE_ARROW, false);
                            events.ScheduleEvent(EVENT_FIRE_ARROW, urand(14 * IN_MILLISECONDS, 19 * IN_MILLISECONDS));
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_shadopan_archeryAI(creature);
        }
};

class npc_residual_hatred : public CreatureScript
{
    public:
        npc_residual_hatred() : CreatureScript("npc_residual_hatred") { }

        enum eSpells
        {
            SPELL_CURSE_OF_AGONY    = 112999,
            SPELL_RING_OF_MALICE    = 112932,
            SPELL_SHADOW_BOLT       = 112998,
            SPELL_APPARATIONS       = 111698,
        };

        enum eEvents
        {
            EVENT_CURSE_OF_AGONY = 1,
            EVENT_RING_OF_MALICE = 2,
            EVENT_SHADOW_BOLT    = 3
        };

        struct npc_residual_hatredAI : public ScriptedAI
        {
            npc_residual_hatredAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_CURSE_OF_AGONY, urand(5 * IN_MILLISECONDS, 10 * IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_RING_OF_MALICE, urand(6 * IN_MILLISECONDS, 10 * IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_SHADOW_BOLT, 1.3 * IN_MILLISECONDS);
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                if (damage >= me->GetHealth() && me->HasAura(SPELL_APPARATIONS))
                    damage = 0;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CURSE_OF_AGONY:
                            me->CastSpell((Unit*)NULL, SPELL_CURSE_OF_AGONY, false);
                            events.ScheduleEvent(EVENT_CURSE_OF_AGONY, urand(15 * IN_MILLISECONDS, 20 * IN_MILLISECONDS));
                            break;
                        case EVENT_RING_OF_MALICE:
                            me->CastSpell(me, SPELL_RING_OF_MALICE, false);
                            events.ScheduleEvent(EVENT_RING_OF_MALICE, urand(21 * IN_MILLISECONDS, 26 * IN_MILLISECONDS));
                            break;
                        case EVENT_SHADOW_BOLT:
                            me->CastSpell((Unit*)NULL, SPELL_SHADOW_BOLT, false);
                            events.ScheduleEvent(EVENT_SHADOW_BOLT, 1.3 * IN_MILLISECONDS);
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_residual_hatredAI(creature);
        }
};

class npc_vistige_of_hatred : public CreatureScript
{
    public:
        npc_vistige_of_hatred() : CreatureScript("npc_vistige_of_hatred") { }

        enum eSpells
        {
            SPELL_BLACK_CLEAVE      = 113020,
            SPELL_DEATH_GRIP        = 113021,
            SPELL_TOUCH_OF_WEAKNESS = 113022,
            SPELL_APPARATIONS       = 111698,
        };

        enum eEvents
        {
            EVENT_BLACK_CLEAVE      = 1,
            EVENT_TOUCH_OF_WEAKNESS = 2,
            EVENT_DEATH_GRIP        = 3
        };

        struct npc_vistige_of_hatredAI : public ScriptedAI
        {
            npc_vistige_of_hatredAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_BLACK_CLEAVE, 15 * IN_MILLISECONDS);
                events.ScheduleEvent(EVENT_TOUCH_OF_WEAKNESS, urand(20 * IN_MILLISECONDS, 25 * IN_MILLISECONDS));
                events.ScheduleEvent(EVENT_DEATH_GRIP, urand(7 * IN_MILLISECONDS, 10 * IN_MILLISECONDS));
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                if (damage >= me->GetHealth() && me->HasAura(SPELL_APPARATIONS))
                    damage = 0;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BLACK_CLEAVE:
                            me->CastSpell((Unit*)NULL, SPELL_BLACK_CLEAVE, false);
                            events.ScheduleEvent(EVENT_BLACK_CLEAVE, 15 * IN_MILLISECONDS);
                            break;
                        case EVENT_TOUCH_OF_WEAKNESS:
                            me->CastSpell((Unit*)NULL, SPELL_TOUCH_OF_WEAKNESS, false);
                            events.ScheduleEvent(EVENT_TOUCH_OF_WEAKNESS, urand(20 * IN_MILLISECONDS, 25 * IN_MILLISECONDS));
                            break;
                        case EVENT_DEATH_GRIP:
                            if (me->GetVictim() && !me->IsWithinMeleeRange(me->GetVictim()))
                                me->CastSpell(me->GetVictim(), SPELL_DEATH_GRIP, false);
                            events.ScheduleEvent(EVENT_DEATH_GRIP, urand(7 * IN_MILLISECONDS, 10 * IN_MILLISECONDS));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_vistige_of_hatredAI(creature);
        }
};

class npc_fragment_of_hatred : public CreatureScript
{
    public:
        npc_fragment_of_hatred() : CreatureScript("npc_fragment_of_hatred") { }

        enum eSpells
        {
            SPELL_SINISTER_STRIKE  = 112931,
            SPELL_APPARATIONS      = 111698,
        };

        enum eEvents
        {
            EVENT_SINISTER_STRIKE = 1
        };

        struct npc_fragment_of_hatredAI : public ScriptedAI
        {
            npc_fragment_of_hatredAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset() OVERRIDE
            {
                events.Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                events.ScheduleEvent(EVENT_SINISTER_STRIKE, urand(3 * IN_MILLISECONDS, 10 * IN_MILLISECONDS));
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                if (damage >= me->GetHealth() && me->HasAura(SPELL_APPARATIONS))
                    damage = 0;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (events.ExecuteEvent() == EVENT_SINISTER_STRIKE)
                {
                    me->CastSpell((Unit*)NULL, SPELL_SINISTER_STRIKE, false);
                    events.ScheduleEvent(EVENT_SINISTER_STRIKE, urand(3 * IN_MILLISECONDS, 10 * IN_MILLISECONDS));
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_fragment_of_hatredAI(creature);
        }
};

class npc_hateful_essence : public CreatureScript
{
    public:
        npc_hateful_essence() : CreatureScript("npc_hateful_essence") { }

        struct npc_hateful_essenceAI : public ScriptedAI
        {
            npc_hateful_essenceAI(Creature* creature) : ScriptedAI(creature) { }

            void InitializeAI() OVERRIDE
            {
                me->SetReactState(REACT_PASSIVE);
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_hateful_essenceAI(creature);
        }
};
class AreaTrigger_at_shadopan_archery : public AreaTriggerScript
{
    public:
        AreaTrigger_at_shadopan_archery() : AreaTriggerScript("AreaTrigger_at_shadopan_archery") { }

        enum areaTrigger
        {
            AREATRIGGER_ARCHERY_FIRST_BEGIN  = 8271, // ice archers begin
            AREATRIGGER_ARCHERY_FIRST_END    = 8272, // ice archers end
            AREATRIGGER_ARCHERY_SECOND_FIRST = 7121, // fire archers begin
            AREATRIGGER_ARCHERY_SECOND_END   = 7126  // fire archers end
        };

        bool OnTrigger(Player* player, AreaTriggerEntry const* trigger) OVERRIDE
        {
            if (InstanceScript* instance = player->GetInstanceScript())
            {
                switch (trigger->id)
                {
                    case AREATRIGGER_ARCHERY_FIRST_BEGIN:
                        if (instance->GetData(DATA_ARCHERY) != IN_PROGRESS && instance->GetData(DATA_ARCHERY) != DONE)
                            instance->SetData(DATA_ARCHERY, IN_PROGRESS);
                        break;
                    case AREATRIGGER_ARCHERY_FIRST_END:
                        if (instance->GetData(DATA_ARCHERY) != DONE)
                            instance->SetData(DATA_ARCHERY, DONE);
                        break;
                    case AREATRIGGER_ARCHERY_SECOND_FIRST:
                        if (instance->GetData(DATA_FIRE_ARCHERY) != IN_PROGRESS && instance->GetData(DATA_FIRE_ARCHERY) != DONE)
                            instance->SetData(DATA_FIRE_ARCHERY, IN_PROGRESS);
                        break;
                    case AREATRIGGER_ARCHERY_SECOND_END:
                        if (instance->GetData(DATA_FIRE_ARCHERY) != DONE)
                            instance->SetData(DATA_FIRE_ARCHERY, DONE);
                        break;
                }
            }
            return false;
        }
};

void AddSC_shadopan_monastery()
{
    new npc_shadopan_ambusher();
    new npc_shadopan_warden();
    new npc_shadopan_stormbringer();
    new npc_shadopan_disciple();
    new npc_shadopan_unstable_energy();
    new npc_ethereal_sha();
    new npc_spm_void_sha();
    new npc_consuming_sha();
    new npc_destroying_sha();
    new npc_shadopan_novice();
    new npc_shadopan_archery();
    new npc_residual_hatred();
    new npc_vistige_of_hatred();
    new npc_fragment_of_hatred();
    new npc_hateful_essence();
    new AreaTrigger_at_shadopan_archery();
}
