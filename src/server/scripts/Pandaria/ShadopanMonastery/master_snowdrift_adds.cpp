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
// The adds of the Master Snowdrift event: referee, novices, minibosses and staff. The spell scripts and the boss come in their own PRs.

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "Containers.h"
#include "Player.h"
#include "Vehicle.h"
#include "shadopan_monastery.h"

static std::vector<Position> const NovicePositionsList(NovicePositions, NovicePositions + 7);

// alexkulya's core casts a jump at a Position; SkyFire casts it at the coordinates.
static void JumpToPosition(Unit* caster, Position const& pos, uint32 spellId)
{
    caster->CastSpell(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), spellId, false);
}

static Position const InitiateSpawnPos[5] =
{
    {3708.56f, 3039.60f, 816.28f},
    {3699.19f, 3049.62f, 816.28f},
    {3688.03f, 3055.68f, 816.28f},
    {3675.10f, 3066.98f, 816.28f},
    {3668.95f, 3070.81f, 816.28f}
};

static Position const MinibossSpawnPos[2] =
{
    {3683.72f, 3053.94f, 816.28f},
    {3687.77f, 3051.30f, 816.28f}
};

class npc_spm_pandaren_referee : public CreatureScript
{
    public:
        npc_spm_pandaren_referee() : CreatureScript("npc_spm_pandaren_referee") { }

        enum eActions
        {
            ACTION_START_EVENT   = 0,
            ACTION_NOVICE_DONE   = 1,
            ACTION_MINIBOSS_DONE = 2,
            ACTION_NOVICE_DEFEAT = 3
        };

        enum eEvents
        {
            EVENT_INIT_EVENT   = 1,
            EVENT_CHECK_WIPE   = 2,
            EVENT_1_WAVE_END   = 3,
            EVENT_SUMMON_NOVIE = 4
        };

        struct npc_spm_pandaren_refereeAI : public ScriptedAI
        {
            npc_spm_pandaren_refereeAI(Creature* creature) : ScriptedAI(creature), summons(me) { }

            uint8 eventPhase, novoiceSummoned;

            void InitializeAI() OVERRIDE
            {
                instance = me->GetInstanceScript();

                eventPhase = 0;
                novoiceSummoned = 1;

                if (instance && instance->GetData(DATA_SNOWDRIFT_STATE) != DONE)
                {
                    summons.DespawnAll();
                    if (instance)
                        instance->SetData(DATA_SNOWDRIFT_STATE, NOT_STARTED);
                }
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                summons.Summon(summon);
            }

            void DoEvent(uint8 eventId)
            {
                ++eventPhase;
                switch (eventPhase)
                {
                    case 1:
                    {
                        if (instance)
                            if (Creature* snowdrift = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_MASTER_SNOWDRIFT)))
                                if (snowdrift->IsAIEnabled)
                                    snowdrift->AI()->DoAction(5); //ACTION_TALK_WAVE_1_S

                        for (uint8 i = 0; i < 5; i++)
                        {
                            ++novoiceSummoned;
                            me->SummonCreature(NPC_NOVICE, InitiateSpawnPos[i].GetPositionX(), InitiateSpawnPos[i].GetPositionY(), InitiateSpawnPos[i].GetPositionZ());
                        }
                        events.ScheduleEvent(EVENT_CHECK_WIPE, 1 * IN_MILLISECONDS);
                        break;
                    }
                    case 2:
                        if (instance)
                            if (auto const snowdrift = Unit::GetCreature(*me, instance->GetData64(DATA_MASTER_SNOWDRIFT)))
                                if (snowdrift->IsAIEnabled)
                                    snowdrift->AI()->DoAction(3); //ACTION_TALK_WAVE_2

                        me->SummonCreature(NPC_FLAGRANT_LOTUS, MinibossSpawnPos[0].GetPositionX(), MinibossSpawnPos[0].GetPositionY(), MinibossSpawnPos[0].GetPositionZ());
                        me->SummonCreature(NPC_FLYING_SNOW, MinibossSpawnPos[1].GetPositionX(), MinibossSpawnPos[1].GetPositionY(), MinibossSpawnPos[1].GetPositionZ());
                        break;
                }
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                switch (actionId)
                {
                    case ACTION_START_EVENT:
                        DoEvent(eventPhase);
                        break;
                    case ACTION_NOVICE_DONE:
                        events.Reset();
                        events.ScheduleEvent(EVENT_CHECK_WIPE, 1 * IN_MILLISECONDS);
                        events.ScheduleEvent(EVENT_1_WAVE_END, 8 * IN_MILLISECONDS);
                        if (instance)
                        {
                            if (auto const snowdrift = Unit::GetCreature(*me, instance->GetData64(DATA_MASTER_SNOWDRIFT)))
                               if (snowdrift->IsAIEnabled)
                                   snowdrift->AI()->DoAction(2); //ACTION_TALK_WAVE_1
                        }
                        break;
                    case ACTION_MINIBOSS_DONE:
                        if (instance)
                        {
                            events.Reset();

                            if (auto const snowdrift = Unit::GetCreature(*me, instance->GetData64(DATA_MASTER_SNOWDRIFT)))
                               if (snowdrift->IsAIEnabled)
                                   snowdrift->AI()->DoAction(4); //ACTION_TALK_OUTRO
                        }
                        break;
                    case ACTION_NOVICE_DEFEAT:
                        if (novoiceSummoned <= MAX_NOVICE)
                        {
                            ++novoiceSummoned;
                            events.ScheduleEvent(EVENT_SUMMON_NOVIE, urand(1, 3) * IN_MILLISECONDS);

                            switch (novoiceSummoned)
                            {
                                case 10:
                                case 15:
                                case 20:
                                    if (instance)
                                        if (auto const snowdrift = Unit::GetCreature(*me, instance->GetData64(DATA_MASTER_SNOWDRIFT)))
                                            if (snowdrift->IsAIEnabled)
                                                snowdrift->AI()->DoAction(1); //ACTION_NOVOCE_DEFEAT
                                break;
                            }
                        }
                        break;
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_INIT_EVENT:
                            DoEvent(eventPhase);
                            break;
                        case EVENT_CHECK_WIPE:
                            if (instance && instance->IsWipe())
                            {
                                eventPhase = 0;
                                novoiceSummoned = 1;
                                events.Reset();
                                summons.DespawnAll();
                                instance->SetData(DATA_SNOWDRIFT_STATE, FAIL);
                            }
                            else
                                events.ScheduleEvent(EVENT_CHECK_WIPE, 0.5 * IN_MILLISECONDS);
                            break;
                        case EVENT_1_WAVE_END:
                            {
                                std::list<Creature*> noviceList;
                                GetCreatureListWithEntryInGrid(noviceList, me, NPC_NOVICE, 100.0f);

                                for (auto&& novice : noviceList)
                                {
                                    if (novice->IsAIEnabled)
                                        novice->AI()->DoAction(0);
                                }

                                noviceList.clear();
                            }
                            events.ScheduleEvent(EVENT_INIT_EVENT, 1 * IN_MILLISECONDS);
                            break;
                        case EVENT_SUMMON_NOVIE:
                            {
                                // One random spawn point for all three coordinates.
                                Position const& pos = InitiateSpawnPos[irand(0, 4)];
                                me->SummonCreature(NPC_NOVICE, pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ());
                            }
                            break;
                    }
                }
            }

        private:
            EventMap events;
            InstanceScript* instance;
            SummonList summons;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_spm_pandaren_refereeAI(creature);
        }
};

class npc_snowdrift_novice : public CreatureScript
{
    public:
        npc_snowdrift_novice() : CreatureScript("npc_snowdrift_novice") { }

        enum eEvents
        {
            EVENT_KNEEL          = 2,
            EVENT_ATTACK         = 3,
            EVENT_FLYING_KICK    = 4,
            EVENT_HUNDRED_STRIKE = 5,
            EVENT_PALM_STRIKE    = 6,
            EVENT_PARRY_STANCE   = 7
        };

        enum eActions
        {
            ACTION_JUMP = 0
        };

        enum eSpells
        {
            SPELL_JUMP           = 106046,
            SPELL_FLYING_KICK    = 106439,
            SPELL_HUNDRED_STRIKE = 106428,
            SPELL_PALM_STRIKE    = 106421,
            SPELL_PARRY_STANCE   = 106447
        };

        struct npc_snowdrift_noviceAI : public ScriptedAI
        {
            npc_snowdrift_noviceAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;
            InstanceScript* instance;
            bool stillInFight;
            bool hasReward;
            bool allowReward;

            void InitializeAI() OVERRIDE
            {
                instance = me->GetInstanceScript();
                ASSERT(me->IsSummon());
                stillInFight = true;
                JumpToPosition(me, Skyfire::Containers::SelectRandomContainerElement(NovicePositionsList), SPELL_JUMP);
                events.ScheduleEvent(EVENT_ATTACK, me->GetSplineDuration());

                hasReward = false;
                allowReward = false;
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                if (actionId == ACTION_JUMP)
                {
                    events.Reset();
                    me->SetStandState(UNIT_STAND_STATE_STAND);
                    if (instance)
                    {
                        if (auto const targetGUID = instance->GetData64(DATA_RANDOM_SECOND_POS))
                        {
                            if (auto const target = ObjectAccessor::FindUnit(targetGUID))
                            {
                                me->AddUnitState(UNIT_STATE_ROOT | UNIT_STATE_CANNOT_TURN);
                                me->CastSpell(target, SPELL_JUMP, false);
                                events.ScheduleEvent(EVENT_KNEEL, me->GetSplineDuration() + 3.5f);
                            }
                        }
                    }
                }
            }

            void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
            {
                if (!stillInFight)
                {
                    damage = 0;
                    return;
                }

                if (damage >= me->GetHealth())
                {
                    damage = 0;
                    stillInFight = false;
                    me->StopMoving();
                    me->setFaction(35);
                    me->RemoveAllAuras();
                    me->CombatStop(true);
                    me->DeleteThreatList();
                    me->SetReactState(REACT_PASSIVE);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    me->HandleEmoteCommand(EMOTE_ONESHOT_SALUTE);
                    allowReward = true;

                    events.Reset();
                    events.ScheduleEvent(EVENT_JUMP, 2 * IN_MILLISECONDS);

                    if (instance)
                        if (auto const referee = Unit::GetCreature(*me, instance->GetData64(DATA_PANDAREN_REFEREE)))
                            if (referee->IsAIEnabled)
                                referee->AI()->DoAction(3); // ACTION_NOVICE_DEFEAT

                    if (instance)
                        instance->SetData(DATA_DEFEATED_NOVICE, 1);
                }
            }

            void ReceiveEmote(Player* player, uint32 uiTextEmote) OVERRIDE
            {
                if (uiTextEmote == TEXT_EMOTE_BOW && !hasReward && allowReward)
                {
                    hasReward = true;
                    // Respect is a self-only dummy aura: the novice returns the bow
                    DoCast(me, SPELL_RESPECT);
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_ATTACK:
                            DoZoneInCombat();
                            events.ScheduleEvent(EVENT_FLYING_KICK, urand(6, 8) * IN_MILLISECONDS);
                            events.ScheduleEvent(EVENT_HUNDRED_STRIKE, urand(4, 6) * IN_MILLISECONDS);
                            events.ScheduleEvent(EVENT_PALM_STRIKE, urand(7, 9) * IN_MILLISECONDS);
                            events.ScheduleEvent(EVENT_PARRY_STANCE, urand(12, 14) * IN_MILLISECONDS);
                            break;
                        case EVENT_KNEEL:
                            me->SetFacingTo(me->GetAngle(3659.212f, 3015.495f));
                            me->SetStandState(UNIT_STAND_STATE_KNEEL);
                            break;
                        case EVENT_JUMP:
                            if (instance)
                            {
                                if (auto const targetGUID = instance->GetData64(DATA_RANDOM_FIRST_POS))
                                {
                                    if (auto const target = ObjectAccessor::FindUnit(targetGUID))
                                    {
                                        me->AddUnitState(UNIT_STATE_ROOT | UNIT_STATE_CANNOT_TURN);
                                        me->CastSpell(target, SPELL_JUMP, false);
                                        events.ScheduleEvent(EVENT_KNEEL, me->GetSplineDuration() + 2.0f);
                                    }
                                }
                            }
                            allowReward = false;
                            break;
                        case EVENT_FLYING_KICK:
                            me->CastSpell((Unit*)NULL, SPELL_FLYING_KICK, false);
                            events.ScheduleEvent(EVENT_FLYING_KICK, urand(12, 15) * IN_MILLISECONDS);
                            break;
                        case EVENT_HUNDRED_STRIKE:
                            me->CastSpell(me, SPELL_HUNDRED_STRIKE, false);
                            events.ScheduleEvent(EVENT_HUNDRED_STRIKE, urand(8, 12) * IN_MILLISECONDS);
                            break;
                        case EVENT_PALM_STRIKE:
                            me->CastSpell((Unit*)NULL, SPELL_PALM_STRIKE, false);
                            events.ScheduleEvent(EVENT_PALM_STRIKE, urand(16, 21) * IN_MILLISECONDS);
                            break;
                        case EVENT_PARRY_STANCE:
                            me->CastSpell((Unit*)NULL, SPELL_PARRY_STANCE, false);
                            events.ScheduleEvent(EVENT_PARRY_STANCE, urand(15, 19) * IN_MILLISECONDS);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_snowdrift_noviceAI(creature);
        }
};

class npc_snowdrift_miniboss : public CreatureScript
{
    public:
        npc_snowdrift_miniboss() : CreatureScript("npc_snowdrift_miniboss") { }

        enum eEvents
        {
            EVENT_MOVE_AWAY             = 1,
            EVENT_BALL_OF_FIRE          = 2,
            EVENT_FLYING_KICK           = 3,
            EVENT_CALL_STAFF            = 4,
            EVENT_RELEASE_STAFF         = 5,
            EVENT_WHIRLING_STEEL        = 6,
            EVENT_SUMMON_STAFF          = 7,
            EVENT_KNEEL                 = 8
        };

        enum eSpells
        {
            SPELL_EJECT_PASSENGERS = 68576,
            SPELL_WHIRLING_STEEL   = 106646,
            SPELL_JUMP             = 106046,
            SPELL_JADE_STAFF_PRO   = 106620,
            SPELL_BALL_OF_FIRE     = 113760,
            SPELL_FLYING_KICK      = 106439,
            SPELL_CALL_STAFF       = 106681,
            SPELL_RELEASE_STAFF    = 106680
        };

        enum eTalks
        {
            TALK_WHIRLING_STEEL = 0
        };

        struct npc_snowdrift_minibossAI : public ScriptedAI
        {
            npc_snowdrift_minibossAI(Creature* creature) : ScriptedAI(creature), summons(me) { }

            SummonList summons;
            EventMap events, nonCombatEvents;
            InstanceScript* instance;
            bool stillInFight;

            void PassengerBoarded(Unit* who, int8 seatId, bool apply) OVERRIDE
            {
                if (who->GetTypeId() != TypeID::TYPEID_UNIT)
                    return;

                if (apply)
                {
                    who->ToCreature()->SetReactState(REACT_PASSIVE);
                    who->CombatStop(true);
                    who->AddUnitState(UNIT_STATE_ROOT);
                    who->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    who->CastSpell(me, SPELL_JADE_STAFF_PRO, false);

                    if (who->IsAIEnabled)
                        who->GetAI()->DoAction(2);
                }
                else
                {
                    who->ToCreature()->SetReactState(REACT_AGGRESSIVE);
                    who->ClearUnitState(UNIT_STATE_ROOT);
                    who->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    me->RemoveAura(SPELL_JADE_STAFF_PRO);

                    if (who->IsAIEnabled)
                        who->GetAI()->DoAction(1);
                }
            }

            void InitializeAI() OVERRIDE
            {
                instance = me->GetInstanceScript();
                ASSERT(me->IsSummon());
                stillInFight = true;
                JumpToPosition(me, Skyfire::Containers::SelectRandomContainerElement(NovicePositionsList), SPELL_JUMP);
                DoZoneInCombat();

                switch (me->GetEntry())
                {
                    case NPC_FLAGRANT_LOTUS:
                        nonCombatEvents.ScheduleEvent(EVENT_SUMMON_STAFF, 0.5 * IN_MILLISECONDS);
                        events.ScheduleEvent(EVENT_BALL_OF_FIRE, urand(2, 4) * IN_MILLISECONDS);
                        events.ScheduleEvent(EVENT_FLYING_KICK, urand(3, 6) * IN_MILLISECONDS);
                        events.ScheduleEvent(EVENT_RELEASE_STAFF, urand(8, 12) * IN_MILLISECONDS);
                        break;
                    case NPC_FLYING_SNOW:
                        events.ScheduleEvent(EVENT_WHIRLING_STEEL, urand(4, 6) * IN_MILLISECONDS);
                        break;
                }
            }

            void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
            {
                if (!stillInFight)
                {
                    damage = 0;
                    return;
                }

                if (damage >= me->GetHealth())
                {
                    damage = 0;
                    stillInFight = false;
                    me->StopMoving();
                    me->setFaction(35);
                    me->RemoveAllAuras();
                    me->CombatStop(true);
                    me->DeleteThreatList();
                    me->SetReactState(REACT_PASSIVE);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    me->HandleEmoteCommand(EMOTE_ONESHOT_SALUTE);

                    events.Reset();
                    nonCombatEvents.ScheduleEvent(EVENT_MOVE_AWAY, 2 * IN_MILLISECONDS);

                    if (instance)
                        instance->SetData(DATA_DEFEATED_MINIBOSS, 1);
                }
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                summons.Summon(summon);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                nonCombatEvents.Update(diff);

                if (uint32 eventId = nonCombatEvents.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_MOVE_AWAY:
                            {
                               if (uint64 targetGUID = instance ? instance->GetData64(DATA_RANDOM_MINIBOSS_POS) : 0)
                               {
                                   if (Unit* target = ObjectAccessor::FindUnit(targetGUID))
                                   {
                                       me->AddUnitState(UNIT_STATE_ROOT | UNIT_STATE_CANNOT_TURN);
                                       me->CastSpell(target, SPELL_JUMP, false);
                                       nonCombatEvents.ScheduleEvent(EVENT_KNEEL, me->GetSplineDuration() + 2.0f);
                                   }
                               }
                            }
                            break;
                        case EVENT_SUMMON_STAFF:
                            if (Creature* staff = me->SummonCreature(NPC_LOTUS_STAFF, *me))
                                staff->EnterVehicle(me);
                            break;
                        case EVENT_KNEEL:
                            me->SetFacingTo(me->GetAngle(3659.212f, 3015.495f));
                            me->SetStandState(UNIT_STAND_STATE_KNEEL);
                            break;
                    }
                }

                events.Update(diff);

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BALL_OF_FIRE:
                            if (auto const target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true))
                                me->CastSpell(target, SPELL_BALL_OF_FIRE, false);
                            events.ScheduleEvent(EVENT_BALL_OF_FIRE, urand(16,24) * IN_MILLISECONDS);
                            break;
                        case EVENT_FLYING_KICK:
                            if (auto const target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true))
                                me->CastSpell(target, SPELL_FLYING_KICK, false);
                            events.ScheduleEvent(EVENT_FLYING_KICK, urand(12,16) * IN_MILLISECONDS);
                            break;
                        case EVENT_CALL_STAFF:
                            me->CastSpell(me, SPELL_CALL_STAFF, false);
                            events.ScheduleEvent(EVENT_RELEASE_STAFF, urand(18,22) * IN_MILLISECONDS);
                            break;
                        case EVENT_RELEASE_STAFF:
                            me->CastSpell(me, SPELL_EJECT_PASSENGERS, TRIGGERED_FULL_MASK);
                            events.ScheduleEvent(EVENT_CALL_STAFF, urand(8,12) * IN_MILLISECONDS);
                            break;
                        case EVENT_WHIRLING_STEEL:
                            Talk(TALK_WHIRLING_STEEL);
                            me->CastSpell(me, SPELL_WHIRLING_STEEL, TRIGGERED_FULL_MASK);
                            me->ClearUnitState(UNIT_STATE_CASTING);
                            events.ScheduleEvent(EVENT_WHIRLING_STEEL, urand(20, 24) * IN_MILLISECONDS);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_snowdrift_minibossAI(creature);
        }
};

class npc_spm_jade_staff : public CreatureScript
{
    public:
        npc_spm_jade_staff() : CreatureScript("npc_spm_jade_staff") { }

        enum eActions
        {
            ACTION_START_COMBAT = 1,
            ACTION_STOP_COMBAT  = 2
        };

        enum eEvents
        {
            EVENT_STAFF_STRIKE = 1
        };

        enum eSpells
        {
            SPELL_STAFF_STRIKE = 106659
        };

        struct npc_spm_jade_staffAI : public ScriptedAI
        {
            npc_spm_jade_staffAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void DoAction(int32 actionId) OVERRIDE
            {
                switch (actionId)
                {
                    case ACTION_START_COMBAT:
                        me->SetUInt32Value(UNIT_FIELD_SHAPESHIFT_FORM, 1); //UNIT_FIELD_BYTES_2
                        events.ScheduleEvent(EVENT_STAFF_STRIKE, urand(4, 6) * IN_MILLISECONDS);
                        break;
                    case ACTION_STOP_COMBAT:
                        events.Reset();
                        me->SetUInt32Value(UNIT_FIELD_SHAPESHIFT_FORM, 0); //UNIT_FIELD_BYTES_2
                        break;
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (!UpdateVictim())
                    return;

                if (events.ExecuteEvent() == EVENT_STAFF_STRIKE)
                {
                    me->CastSpell(me->GetVictim(), SPELL_STAFF_STRIKE, false);
                    events.ScheduleEvent(EVENT_STAFF_STRIKE, urand(6, 8) * IN_MILLISECONDS);
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_spm_jade_staffAI(creature);
        }
};

void AddSC_master_snowdrift_adds()
{
    new npc_spm_pandaren_referee();
    new npc_snowdrift_novice();
    new npc_snowdrift_miniboss();
    new npc_spm_jade_staff();
}
