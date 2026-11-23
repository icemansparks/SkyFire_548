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
// Master Snowdrift: the boss with his clones and fireballs. The adds and spell scripts are in master_snowdrift_adds.cpp.

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "MoveSplineInit.h"
#include "Player.h"
#include "shadopan_monastery.h"
#include "Vehicle.h"
#include <random>


static Position const SnowdriftPos[2] =
{
    {3680.56f, 3045.27f, 816.20f},// event place
    {3713.60f, 3091.87f, 817.31f} // fight place
};

static Position const ClonePos[3] =
{
    {3683.37f, 3087.65f, 815.70f, 5.67f},
    {3719.41f, 3062.31f, 815.70f, 1.79f},
    {3718.04f, 3097.97f, 817.40f, 3.46f}
};

class boss_master_snowdrift : public CreatureScript
{
    public:
        boss_master_snowdrift() : CreatureScript("boss_master_snowdrift") { }

        enum eActions
        {
            ACTION_START_EVENT   = 0,
            ACTION_NOVOCE_DEFEAT = 1,
            ACTION_TALK_WAVE_1   = 2,
            ACTION_TALK_WAVE_2   = 3,
            ACTION_TALK_OUTRO    = 4,
            ACTION_TALK_WAVE_1_S = 5,
            ACTION_DISSAPEAR     = 6
        };

        enum eSpells
        {
            SPELL_BALL_OF_FIRE   = 113760,
            SPELL_FIST_OF_FURY   = 106853,
            SPELL_PARRY_STANCE   = 106454,
            SPELL_QUIVERING_PALM = 106422,
            SPELL_TORNADO_KICK   = 106434,
            SPELL_SMOKE_BOMB     = 110444,
            SPELL_HURL_CHI       = 106825,
            SPELL_CHASE_DOWN     = 118961,
            SPELL_TORNADO_SLAM   = 106330 // force-casts 106332, the spell that carries spell_snowdrift_tornado_slam
        };

        enum eEvents
        {
            // Snowdrift
            EVENT_FIST_OF_FURY   = 1,
            EVENT_TORNADO_KICK   = 2,
            EVENT_PARRY_STANCE   = 3,
            EVENT_TORNADO_SLAM   = 4,
            EVENT_DISAPPEAR      = 5,
            EVENT_INTRO_1        = 6,
            EVENT_INTRO_2        = 7,
            EVENT_INTRO_3        = 8,
            EVENT_OUTRO_1        = 9,
            EVENT_OUTRO_2        = 10,
            EVENT_OUTRO_END      = 11,
            EVENT_CHECK_DEST     = 12,
            EVENT_CHECK_WIPE     = 13,
            EVENT_PHASE_2        = 14,
            EVENT_FIST_END       = 16,
        };

        enum ePhases
        {
            PHASE_FIGHT_1 = 1,
            PHASE_FIGHT_2 = 2,
            PHASE_FIGHT_3 = 3,
            PHASE_FIGHT_4 = 4,
        };

        enum eTalks
        {
            TALK_INTRO_1     = 0,
            TALK_INTRO_2     = 1,
            TALK_WAWE1_START = 2,
            TALK_WAWE1_END   = 3,
            TALK_WAWE2_START = 4,
            TALK_WAWE2_END   = 5,
            TALK_OUTRO_1     = 6,
            TALK_OUTRO_2     = 7,
            TALK_AGGRO       = 8,
            TALK_DEATH       = 9,
            TALK_SLAY        = 10,
            TALK_PHASE_2     = 11,
            TALK_PHASE_3     = 12,
            TALK_STUDENTS    = 13,
            TALK_FISTS       = 14,
            TALK_TORNADO     = 15,
            TALK_PARRY       = 16,
            TALK_P_2_EMOTE   = 17,
            TALK_P_3_EMOTE   = 18
        };

        struct boss_master_snowdriftAI : public BossAI
        {
            boss_master_snowdriftAI(Creature* creature) : BossAI(creature, DATA_MASTER_SNOWDRIFT) { }

            EventMap nonCombatEvents;
            bool aggroDone, isBossSummoned, isFightWon;
            uint8 phase, hurlChiCnt;
            uint8 randIndex[3];
            uint64 targetGUID;

            void InitializeAI() OVERRIDE
            {
                isFightWon = false;
                me->setActive(true);
                targetGUID = 0;
                me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                for (uint8 i = 0; i < 3; ++i)
                    randIndex[i] = i;

                if (instance)
                {
                    if (instance->GetData(DATA_SNOWDRIFT_STATE) != DONE)
                    {
                        me->SetReactState(REACT_PASSIVE);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        me->NearTeleportTo(SnowdriftPos[0].GetPositionX(), SnowdriftPos[0].GetPositionY(), SnowdriftPos[0].GetPositionZ(), 4.114f);
                    }

                    if (instance->GetBossState(DATA_MASTER_SNOWDRIFT) == DONE)
                    {
                        me->setFaction(35);
                        me->SetReactState(REACT_PASSIVE);
                        me->SetStandState(UNIT_STAND_STATE_KNEEL);

                        float x, y, z;
                        z = me->GetPositionZ();
                        GetPositionWithDistInOrientation(me, -5.0f, me->GetOrientation(), x, y);
                        me->NearTeleportTo(x, y, z, me->GetOrientation());

                        if (instance->GetBossState(DATA_TARAN_ZHU) == DONE)
                            me->SetVisible(false);
                    }
                }

                Reset();
            }

            void Reset() OVERRIDE
            {
                if (instance)
                {
                    if (instance->GetData(DATA_SNOWDRIFT_STATE) == DONE && instance->GetBossState(DATA_MASTER_SNOWDRIFT) != DONE)
                    {
                        _Reset();
                        events.Reset();
                        nonCombatEvents.Reset();
                        me->SetReactState(REACT_AGGRESSIVE);
                    }
                }

                me->setRegeneratingHealth(true);
                me->SetHealth(me->GetMaxHealth());
                phase = PHASE_FIGHT_1;
                isBossSummoned = false;
                aggroDone = false;
                hurlChiCnt = 0;
                targetGUID = 0;
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                if (!aggroDone)
                {
                    aggroDone = true;
                    Talk(TALK_AGGRO);
                    events.ScheduleEvent(EVENT_FIST_OF_FURY, 15 * IN_MILLISECONDS);
                    events.ScheduleEvent(EVENT_TORNADO_KICK, 30 * IN_MILLISECONDS);

                    if (instance)
                    {
                        instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    }

                    uint32 delay = 0;
                    me->m_Events.Schedule(delay += 2500, 10, [this]()
                    {
                        if (me->IsInCombat())
                            _EnterCombat();
                    });
                }
            }

            void EnterEvadeMode() OVERRIDE
            {
                BossAI::EnterEvadeMode();
                if (instance)
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    instance->SetBossState(DATA_MASTER_SNOWDRIFT, FAIL);
                }
            }

            void JustSummoned(Creature* summon) OVERRIDE
            {
                summons.Summon(summon);
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                switch (actionId)
                {
                    case ACTION_START_EVENT:
                        nonCombatEvents.ScheduleEvent(EVENT_INTRO_1, 1 * IN_MILLISECONDS);
                        break;
                    case ACTION_NOVOCE_DEFEAT:
                        Talk(TALK_STUDENTS);
                        break;
                    case ACTION_TALK_WAVE_1:
                        Talk(TALK_WAWE1_END);
                        break;
                    case ACTION_TALK_WAVE_1_S:
                        Talk(TALK_WAWE1_START);
                        break;
                    case ACTION_TALK_OUTRO:
                        Talk(TALK_WAWE2_END);
                        nonCombatEvents.ScheduleEvent(EVENT_OUTRO_1, 6.35 * IN_MILLISECONDS);
                        break;
                    case ACTION_TALK_WAVE_2:
                        Talk(TALK_WAWE2_START);
                        break;
                    case ACTION_DISSAPEAR:
                        ++hurlChiCnt;
                        isBossSummoned = false;
                        if (hurlChiCnt < 3)
                        {
                            me->SetVisible(false);
                            me->CastSpell(me, SPELL_SMOKE_BOMB, true);
                            summons.DespawnEntry(NPC_SNOWDRIFT_CLONE);
                            nonCombatEvents.ScheduleEvent(EVENT_DISAPPEAR, 2 * IN_MILLISECONDS);

                            if (instance)
                                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                        }
                        else
                        {
                            phase = PHASE_FIGHT_3;

                            Talk(TALK_PHASE_3);
                            Talk(TALK_P_3_EMOTE);
                            me->SetReactState(REACT_AGGRESSIVE);
                            DoZoneInCombat();
                            me->setRegeneratingHealth(true);
                            events.ScheduleEvent(urand(EVENT_PARRY_STANCE, EVENT_TORNADO_SLAM), urand(12, 15) * IN_MILLISECONDS);
                            nonCombatEvents.Reset();

                            std::list<Creature*> clonesList;
                            me->GetCreatureListWithEntryInGrid(clonesList, NPC_SNOWDRIFT_CLONE, 150.0f);

                            for (auto&& clone : clonesList)
                            {
                                if (clone->IsAIEnabled)
                                    clone->AI()->DoAction(0); // ACTION_BALL_OF_FIRE
                            }

                            clonesList.clear();
                        }
                        break;
                }
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                if (phase == PHASE_FIGHT_4)
                {
                    damage = 0;
                    return;
                }

                if (me->HealthBelowPctDamaged(51, damage) && phase == PHASE_FIGHT_1)
                {
                    phase = PHASE_FIGHT_2;
                    Talk(TALK_PHASE_2);
                    events.Reset();
                    me->StopMoving();
                    me->CombatStop(true);
                    me->setRegeneratingHealth(false);
                    me->SetReactState(REACT_PASSIVE);
                    nonCombatEvents.ScheduleEvent(EVENT_PHASE_2, 7 * IN_MILLISECONDS);
                }

                if (damage >= me->GetHealth() && phase != PHASE_FIGHT_4)
                {
                    damage = 0;
                    phase = PHASE_FIGHT_4;
                    me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);

                    if (!isFightWon)
                    {
                        isFightWon = true;

                        _JustDied();
                        Talk(TALK_DEATH);
                        me->StopMoving();
                        me->setFaction(35);
                        me->RemoveAllAuras();
                        me->CombatStop(true);
                        me->DeleteThreatList();
                        me->SetReactState(REACT_PASSIVE);
                        me->SetStandState(UNIT_STAND_STATE_KNEEL);
                        events.Reset();
                        nonCombatEvents.Reset();

                        if (instance)
                        {
                            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                            for (auto&& itr : instance->instance->GetPlayers())
                                itr.GetSource()->ModifyCurrency(CURRENCY_TYPE_JUSTICE_POINTS, (instance->instance->IsHeroic() ? 125 : 70) * CURRENCY_PRECISION);
                        }
                    }
                }
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (pointId == EVENT_CHARGE)
                    nonCombatEvents.ScheduleEvent(EVENT_OUTRO_END, me->GetSplineDuration());
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                nonCombatEvents.Update(diff);

                while (uint32 eventId = nonCombatEvents.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_INTRO_1:
                            Talk(TALK_INTRO_1);
                            nonCombatEvents.ScheduleEvent(EVENT_INTRO_2, 7.4 * IN_MILLISECONDS);
                            break;
                        case EVENT_INTRO_2:
                            Talk(TALK_INTRO_2);
                            nonCombatEvents.ScheduleEvent(EVENT_INTRO_3, 10.5 * IN_MILLISECONDS);
                            break;
                        case EVENT_INTRO_3:
                            if (instance)
                                if (Creature* referee = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_PANDAREN_REFEREE)))
                                    if (referee->IsAIEnabled)
                                        referee->AI()->DoAction(ACTION_START_EVENT);
                            break;
                        case EVENT_OUTRO_1:
                            Talk(TALK_OUTRO_1);
                            nonCombatEvents.ScheduleEvent(EVENT_OUTRO_2, 7.3 * IN_MILLISECONDS);
                            break;
                        case EVENT_OUTRO_2:
                            {
                                if (instance)
                                    instance->SetData(DATA_SNOWDRIFT_STATE, DONE);

                                Talk(TALK_OUTRO_2);
                                me->GetMotionMaster()->MoveCharge(SnowdriftPos[1].GetPositionX(), SnowdriftPos[1].GetPositionY(), SnowdriftPos[1].GetPositionZ(), 25.0f, EVENT_CHARGE);
                            }
                            break;
                        case EVENT_OUTRO_END:
                            me->SetFacingTo(4.114f);
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                            if (instance)
                            {
                                instance->SetBossState(DATA_MASTER_SNOWDRIFT, NOT_STARTED);
                            }
                            break;
                        case EVENT_DISAPPEAR:
                            {
                               std::shuffle(randIndex, randIndex + 3, std::mt19937(std::random_device()()));
                               for (int i = 0; i < 3; ++i)
                               {
                                   if (!isBossSummoned)
                                   {
                                       isBossSummoned = true;
                                       me->NearTeleportTo(ClonePos[randIndex[i]].GetPositionX(), ClonePos[randIndex[i]].GetPositionY(), ClonePos[randIndex[i]].GetPositionZ(), ClonePos[randIndex[i]].GetOrientation());
                                       me->SetVisible(true);
                                       me->CastSpell(me, SPELL_HURL_CHI, true);
                                       me->CastSpell(me, SPELL_SMOKE_BOMB, true);

                                       if (instance)
                                           instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                                   }
                                   else
                                   {
                                       if (auto const clone = me->SummonCreature(NPC_SNOWDRIFT_CLONE, ClonePos[randIndex[i]].GetPositionX(), ClonePos[randIndex[i]].GetPositionY(), ClonePos[randIndex[i]].GetPositionZ(), ClonePos[randIndex[i]].GetOrientation(), TempSummonType::TEMPSUMMON_CORPSE_DESPAWN))
                                       {
                                           clone->SetReactState(REACT_PASSIVE);
                                           clone->CastSpell(clone, SPELL_SMOKE_BOMB, true);
                                       }
                                   }
                               }
                            }
                            break;
                        case EVENT_CHECK_WIPE:
                            if (instance)
                            {
                                if (instance->IsWipe())
                                    EnterEvadeMode();
                                else
                                    nonCombatEvents.ScheduleEvent(EVENT_CHECK_WIPE, 0.5 * IN_MILLISECONDS);
                            }
                            break;
                        case EVENT_PHASE_2:
                            Talk(TALK_P_2_EMOTE);
                            me->CastSpell(me, SPELL_SMOKE_BOMB, true);
                            me->SetVisible(false);
                            events.Reset();
                            nonCombatEvents.ScheduleEvent(EVENT_DISAPPEAR, 2 * IN_MILLISECONDS);
                            nonCombatEvents.ScheduleEvent(EVENT_CHECK_WIPE, 3 * IN_MILLISECONDS);
                            break;
                    }
                }

                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_TORNADO_KICK:
                            Talk(TALK_TORNADO);
                            DoCast(me, SPELL_TORNADO_KICK);
                            me->ClearUnitState(UNIT_STATE_CASTING);
                            events.ScheduleEvent(EVENT_TORNADO_KICK, 30 * IN_MILLISECONDS);
                            break;
                        case EVENT_FIST_OF_FURY:
                            Talk(TALK_FISTS);
                            if (Unit* victim = me->GetVictim())
                            {
                                targetGUID = victim->GetGUID();
                                me->PrepareChanneledCast(me->GetAngle(victim), SPELL_FIST_OF_FURY);
                            }
                            events.ScheduleEvent(EVENT_FIST_OF_FURY, 15 * IN_MILLISECONDS);
                            events.ScheduleEvent(EVENT_FIST_END, 6500);
                            break;
                        case EVENT_PARRY_STANCE:
                            Talk(TALK_PARRY);
                            me->CastSpell((Unit*)NULL, SPELL_PARRY_STANCE, false);
                            events.ScheduleEvent(EVENT_TORNADO_SLAM, urand(12, 15) * IN_MILLISECONDS);
                            break;
                        case EVENT_TORNADO_SLAM:
                            if (auto const target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                                me->CastSpell(target, SPELL_CHASE_DOWN, false);
                            me->SetReactState(REACT_PASSIVE);
                            events.ScheduleEvent(EVENT_CHECK_DEST, 0.5 * IN_MILLISECONDS);
                            break;
                        case EVENT_CHECK_DEST:
                            if (me->GetVictim() && (me->GetVictim()->HasAura(SPELL_CHASE_DOWN, me->GetGUID()) || me->HasAura(SPELL_CHASE_DOWN)))
                            {
                                if (me->GetDistance2d(me->GetVictim()) <= 1.5f)
                                {
                                    me->CastSpell(me->GetVictim(), SPELL_TORNADO_SLAM, false);
                                    me->SetReactState(REACT_AGGRESSIVE);
                                    events.ScheduleEvent(EVENT_PARRY_STANCE, urand(13, 15) * IN_MILLISECONDS);
                                }
                                else
                                    events.ScheduleEvent(EVENT_CHECK_DEST, 0.5 * IN_MILLISECONDS);
                            }
                            else
                            {
                                me->SetReactState(REACT_AGGRESSIVE);
                                events.ScheduleEvent(EVENT_PARRY_STANCE, urand(13, 15) * IN_MILLISECONDS);
                            }
                            break;
                        case EVENT_FIST_END:
                            me->RemoveChanneledCast(targetGUID);
                            break;
                    }
                }

                if (phase != PHASE_FIGHT_2)
                {
                    DoMeleeAttackIfReady();
                    EnterEvadeIfOutOfCombatArea(diff, 41.0f);
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_master_snowdriftAI(creature);
        }
};

class npc_snowdrift_clone : public CreatureScript
{
    public:
        npc_snowdrift_clone() : CreatureScript("npc_snowdrift_clone") { }

        enum eSpells
        {
            SPELL_COPY_HEALTH  = 122074,
            SPELL_BALL_OF_FIRE = 113760
        };

        enum eEvents
        {
            EVENT_BALL_OF_FIRE = 1
        };

        enum eActions
        {
            ACTION_BALL_OF_FIRE = 0
        };

        struct npc_snowdrift_cloneAI : public ScriptedAI
        {
            npc_snowdrift_cloneAI(Creature* creature) : ScriptedAI(creature) { }

            InstanceScript* instance;
            EventMap events;

            void InitializeAI() OVERRIDE
            {
                instance = me->GetInstanceScript();
                ASSERT(me->IsSummon());
                me->SetReactState(REACT_PASSIVE);
                me->setRegeneratingHealth(false);
                me->CastSpell((Unit*)NULL, SPELL_COPY_HEALTH, false);
            }

            void DoAction(int32 actionId) OVERRIDE
            {
                if (actionId == ACTION_BALL_OF_FIRE)
                    events.ScheduleEvent(EVENT_BALL_OF_FIRE, 1 * IN_MILLISECONDS);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (events.ExecuteEvent() == EVENT_BALL_OF_FIRE)
                {
                    me->CastSpell(me, SPELL_BALL_OF_FIRE, false);
                    events.ScheduleEvent(EVENT_BALL_OF_FIRE, 2 * IN_MILLISECONDS);
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_snowdrift_cloneAI(creature);
        }
};

class npc_snowdrift_fireball : public CreatureScript
{
    public:
        npc_snowdrift_fireball() : CreatureScript("npc_snowdrift_fireball") { }

        enum eSpells
        {
            SPELL_BALL_OF_FIRE_BLUE = 106534,
            SPELL_BALL_OF_FIRE_RED  = 106535
        };

        enum eEvents
        {
            EVENT_INITIALIZE = 1
        };

        struct npc_snowdrift_fireballAI : public ScriptedAI
        {
            npc_snowdrift_fireballAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void InitializeAI() OVERRIDE
            {
                events.ScheduleEvent(EVENT_INITIALIZE, 0.1 * IN_MILLISECONDS);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                events.Update(diff);

                if (events.ExecuteEvent() == EVENT_INITIALIZE)
                {
                    me->SetReactState(REACT_PASSIVE);
                    float x, y;
                    GetPositionWithDistInOrientation(me, 100.0f, me->GetOrientation(), x, y);
                    me->CastSpell(me, urand(SPELL_BALL_OF_FIRE_BLUE, SPELL_BALL_OF_FIRE_RED), false);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                    me->GetMotionMaster()->MovePoint(0, x, y, me->GetPositionZ());
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_snowdrift_fireballAI(creature);
        }
};

class AreaTrigger_at_before_snowdrift : public AreaTriggerScript
{
    public:
        AreaTrigger_at_before_snowdrift() : AreaTriggerScript("AreaTrigger_at_before_snowdrift") { }

        enum eActions
        {
            ACTION_START_EVENT = 0
        };

        bool OnTrigger(Player* player, AreaTriggerEntry const* trigger) OVERRIDE
        {
            if (auto const instance = player->GetInstanceScript())
            {
                if (instance->GetData(DATA_SNOWDRIFT_STATE) != IN_PROGRESS && instance->GetData(DATA_SNOWDRIFT_STATE) != DONE)
                {
                    instance->SetData(DATA_SNOWDRIFT_STATE, IN_PROGRESS);
                    if (auto const snowdrift = Unit::GetCreature(*player, instance->GetData64(DATA_MASTER_SNOWDRIFT)))
                        if (snowdrift->IsAIEnabled)
                            snowdrift->AI()->DoAction(ACTION_START_EVENT);
                }
            }

            return false;
        }
};

void AddSC_boss_master_snowdrift()
{
    new boss_master_snowdrift();
    new npc_snowdrift_clone();
    new npc_snowdrift_fireball();
    new AreaTrigger_at_before_snowdrift();
}
