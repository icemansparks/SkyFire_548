/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

 /* ScriptData
 SDName: Mulgore
 SD%Complete: 100
 SDComment: Support for quest: 14491, Fledgling Braves sparring with Bristleback Invaders at Camp Narache
 SDCategory: Mulgore
 EndScriptData */

 /* ContentData
 npc_agitated_earth_spirit
 npc_fledgling_brave
 npc_bristleback_invader
 EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "Player.h"
#include "SpellInfo.h"

enum eAgitatedEarthSpirit
{
    SPELL_SOOTHE_EARTH_SPIRIT       = 69453,
    SPELL_ROCK_BARRAGE              = 81305,

    NPC_EARTH_SPIRIT_CREDIT_BUNNY   = 36872,

    EVENT_ROCK_BARRAGE              = 1
};

// 36845 - Agitated Earth Spirit
class npc_agitated_earth_spirit : public CreatureScript
{
public:
    npc_agitated_earth_spirit() : CreatureScript("npc_agitated_earth_spirit") { }

    struct npc_agitated_earth_spiritAI : public ScriptedAI
    {
        npc_agitated_earth_spiritAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() OVERRIDE
        {
            events.Reset();
            playerGUID = 0;
            me->setFaction(190);
        }

        void SpellHit(Unit* caster, SpellInfo const* spell) OVERRIDE
        {
            if (spell->Id == SPELL_SOOTHE_EARTH_SPIRIT && me->getFaction() == 190)
            {
                Position pos;
                caster->GetNearPoint(caster, pos.m_positionX, pos.m_positionY, pos.m_positionZ, 0.0f, MIN_MELEE_REACH, caster->GetAngle(me));
                me->GetMotionMaster()->MovePoint(1, pos);
                playerGUID = caster->GetGUID();
            }
        }

        void MovementInform(uint32 type, uint32 pointId) OVERRIDE
        {
            if (type == POINT_MOTION_TYPE && pointId == 1)
            {
                switch (std::rand() % 1)
                {
                    case 0:
                    {
                        me->setFaction(35);
                        if (Player* player = ObjectAccessor::GetPlayer(*me, playerGUID))
                            player->KilledMonsterCredit(NPC_EARTH_SPIRIT_CREDIT_BUNNY);

                        me->DespawnOrUnsummon(1000);
                        break;
                    }
                    case 1:
                        me->setFaction(14);
                        if (Player* player = ObjectAccessor::GetPlayer(*me, playerGUID))
                        {
                            Talk(0, player);
                            AttackStart(player);
                        }
                        break;
                }
            }
        }

        void EnterCombat(Unit* /*victim*/) OVERRIDE
        {
            events.ScheduleEvent(EVENT_ROCK_BARRAGE, std::rand() % 5000 + 4000);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_ROCK_BARRAGE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                            DoCast(target, SPELL_ROCK_BARRAGE);
                        events.ScheduleEvent(EVENT_ROCK_BARRAGE, std::rand() % 21000 + 18000);
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        uint64 playerGUID;
        EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_agitated_earth_spiritAI(creature);
    }
};

enum CampNaracheSparring
{
    NPC_FLEDGLING_BRAVE         = 36942,
    NPC_BRISTLEBACK_INVADER     = 36943,
};

// neither side takes sparring damage below this %
int32 const SPARRING_HEALTH_MIN = 85;

// The braves and invaders stand 1.5-6 yd apart in pairs; braves farther away guard the camp
float const SPARRING_SEEK_RANGE = 6.5f;

// 36942 - Fledgling Brave
class npc_fledgling_brave : public CreatureScript
{
public:
    npc_fledgling_brave() : CreatureScript("npc_fledgling_brave") { }

    struct npc_fledgling_braveAI : public ScriptedAI
    {
        npc_fledgling_braveAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() OVERRIDE
        {
            seekTimer = std::rand() % 2000 + 1000;
        }

        void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
        {
            if (attacker && attacker->GetEntry() == NPC_BRISTLEBACK_INVADER && me->HealthBelowPctDamaged(SPARRING_HEALTH_MIN, damage))
                damage = 0;
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!me->IsInCombat())
            {
                if (seekTimer <= diff)
                {
                    // only from the spawn point, so a brave back from evading picks its own invader again
                    if (me->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) <= 1.0f)
                        if (Creature* invader = me->FindNearestCreature(NPC_BRISTLEBACK_INVADER, SPARRING_SEEK_RANGE))
                            if (!invader->IsInCombat())
                                AttackStart(invader);
                    seekTimer = std::rand() % 2000 + 1000;
                }
                else
                    seekTimer -= diff;
            }

            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }

    private:
        uint32 seekTimer;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_fledgling_braveAI(creature);
    }
};

// 36943 - Bristleback Invader
class npc_bristleback_invader : public CreatureScript
{
public:
    npc_bristleback_invader() : CreatureScript("npc_bristleback_invader") { }

    struct npc_bristleback_invaderAI : public ScriptedAI
    {
        npc_bristleback_invaderAI(Creature* creature) : ScriptedAI(creature) { }

        void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
        {
            if (!attacker)
                return;

            // a brave never kills an invader, so the kill stays with the player; once a player
            // has the invader, the brave's hits stay at zero and its threat is dropped, so it
            // can't win the invader back
            if (attacker->GetEntry() == NPC_FLEDGLING_BRAVE)
            {
                if (me->HealthBelowPctDamaged(SPARRING_HEALTH_MIN, damage))
                    damage = 0;
                else if (me->GetVictim() && me->GetVictim() != attacker)
                {
                    damage = 0;
                    me->getThreatManager().modifyThreatPercent(attacker, -100);
                }
                return;
            }

            // a player or pet joining in takes the invader off the brave
            if (attacker->GetCharmerOrOwnerPlayerOrPlayerItself())
                if (Unit* victim = me->GetVictim())
                    if (victim->GetEntry() == NPC_FLEDGLING_BRAVE)
                    {
                        me->getThreatManager().modifyThreatPercent(victim, -100);
                        AttackStart(attacker);
                    }
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_bristleback_invaderAI(creature);
    }
};

void AddSC_mulgore()
{
    new npc_agitated_earth_spirit();
    new npc_fledgling_brave();
    new npc_bristleback_invader();
}
