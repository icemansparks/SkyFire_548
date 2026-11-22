/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Eastern_Plaguelands
SD%Complete: 100
SDComment: Quest support: 5211, 5742. Special vendor Augustus the Touched
SDCategory: Eastern Plaguelands
EndScriptData */

/* ContentData
npc_ghoul_flayer
npc_augustus_the_touched
npc_darrowshire_spirit
npc_tirion_fordring
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "Player.h"
#include "Group.h"
#include "WorldSession.h"

class npc_ghoul_flayer : public CreatureScript
{
public:
    npc_ghoul_flayer() : CreatureScript("npc_ghoul_flayer") { }

    struct npc_ghoul_flayerAI : public ScriptedAI
    {
        npc_ghoul_flayerAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() OVERRIDE { }

        void EnterCombat(Unit* /*who*/) OVERRIDE { }

        void JustDied(Unit* killer) OVERRIDE
        {
            if (killer->GetTypeId() == TypeID::TYPEID_PLAYER)
                me->SummonCreature(11064, 0.0f, 0.0f, 0.0f, 0.0f, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 60000);
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_ghoul_flayerAI(creature);
    }
};

/*######
## npc_augustus_the_touched
######*/

class npc_augustus_the_touched : public CreatureScript
{
public:
    npc_augustus_the_touched() : CreatureScript("npc_augustus_the_touched") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
    {
        player->PlayerTalkClass->ClearMenus();
        if (action == GOSSIP_ACTION_TRADE)
            player->GetSession()->SendListInventory(creature->GetGUID());
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
    {
        if (creature->IsQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if (creature->IsVendor() && player->GetQuestRewardStatus(6164))
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }
};

/*######
## npc_darrowshire_spirit
######*/

enum DarrowshireSpirit
{
    SPELL_SPIRIT_SPAWNIN    = 17321
};

class npc_darrowshire_spirit : public CreatureScript
{
public:
    npc_darrowshire_spirit() : CreatureScript("npc_darrowshire_spirit") { }

    bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
    {
        player->SEND_GOSSIP_MENU(3873, creature->GetGUID());
        player->TalkedToCreature(creature->GetEntry(), creature->GetGUID());
        creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        return true;
    }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_darrowshire_spiritAI(creature);
    }

    struct npc_darrowshire_spiritAI : public ScriptedAI
    {
        npc_darrowshire_spiritAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() OVERRIDE
        {
            DoCast(me, SPELL_SPIRIT_SPAWNIN);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE { }
    };
};

/*######
## npc_tirion_fordring
######*/

#define GOSSIP_HELLO    "I am ready to hear your tale, Tirion."
#define GOSSIP_SELECT1  "Thank you, Tirion.  What of your identity?"
#define GOSSIP_SELECT2  "That is terrible."
#define GOSSIP_SELECT3  "I will, Tirion."

class npc_tirion_fordring : public CreatureScript
{
public:
    npc_tirion_fordring() : CreatureScript("npc_tirion_fordring") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
    {
        player->PlayerTalkClass->ClearMenus();
        switch (action)
        {
            case GOSSIP_ACTION_INFO_DEF+1:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SELECT1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                player->SEND_GOSSIP_MENU(4493, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+2:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SELECT2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                player->SEND_GOSSIP_MENU(4494, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+3:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SELECT3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
                player->SEND_GOSSIP_MENU(4495, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+4:
                player->CLOSE_GOSSIP_MENU();
                player->AreaExploredOrEventHappens(5742);
                break;
        }
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
    {
        if (creature->IsQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if (player->GetQuestStatus(5742) == QUEST_STATUS_INCOMPLETE && player->getStandState() == UNIT_STAND_STATE_SIT)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HELLO, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());

        return true;
    }
};

/*######
## Quest: Beat It Out of Them (27522)
## npc_scourge_siege_engineer
######*/

enum ScourgeSiegeEngineer
{
    TALK_DIED                  = 0,
    TALK_CREDIT_1              = 1,
    TALK_CREDIT_2              = 2,

    EVENT_THROW_DYNAMITE       = 1,
    SPELL_THROW_DYNAMITE       = 86088,

    QUEST_BEAT_IT_OUT_OF_THEM  = 27522,

    OBJECTIVE_ENGINEERS        = 265859,
    OBJECTIVE_GIDVINS_LOCATION = 265860,
    OBJECTIVE_GIDVINS_CAPTOR   = 265861,

    NPC_CREDIT_1               = 45845,
    NPC_CREDIT_2               = 45846
};

class npc_scourge_siege_engineer : public CreatureScript
{
public:
    npc_scourge_siege_engineer() : CreatureScript("npc_scourge_siege_engineer") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_scourge_siege_engineerAI(creature);
    }

    struct npc_scourge_siege_engineerAI : public ScriptedAI
    {
        npc_scourge_siege_engineerAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() OVERRIDE
        {
            _events.Reset();
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            _events.ScheduleEvent(EVENT_THROW_DYNAMITE, 10000);
        }

        void JustDied(Unit* killer) OVERRIDE
        {
            Player* killerPlayer = killer ? killer->GetCharmerOrOwnerPlayerOrPlayerItself() : NULL;
            if (!killerPlayer)
                return;

            // The engine shares the kill counter with the whole group, so the bonus credits go to every member in range.
            if (Group* group = killerPlayer->GetGroup())
            {
                for (GroupReference* ref = group->GetFirstMember(); ref; ref = ref->next())
                    if (Player* member = ref->GetSource())
                        if (member->IsAtGroupRewardDistance(me) && (member->IsAlive() || !member->GetCorpse()))
                            CreditPlayer(member, member == killerPlayer);
            }
            else
                CreditPlayer(killerPlayer, true);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            _events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_THROW_DYNAMITE:
                        DoCastVictim(SPELL_THROW_DYNAMITE);
                        _events.ScheduleEvent(EVENT_THROW_DYNAMITE, urand(5000, 7000));
                        break;
                    default:
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        // The third kill reveals where Gidwin is, the sixth who holds him; >= keeps a credit missed on an earlier kill.
        void CreditPlayer(Player* player, bool talk)
        {
            if (player->GetQuestStatus(QUEST_BEAT_IT_OUT_OF_THEM) != QUEST_STATUS_INCOMPLETE)
                return;

            uint32 engineers = player->GetQuestObjectiveCounter(OBJECTIVE_ENGINEERS);
            bool credited = false;

            if (engineers >= 3 && !player->GetQuestObjectiveCounter(OBJECTIVE_GIDVINS_LOCATION))
            {
                if (talk)
                    Talk(TALK_CREDIT_1, player);
                player->KilledMonsterCredit(NPC_CREDIT_1);
                credited = true;
            }

            if (engineers >= 6 && !player->GetQuestObjectiveCounter(OBJECTIVE_GIDVINS_CAPTOR))
            {
                if (talk)
                    Talk(TALK_CREDIT_2, player);
                player->KilledMonsterCredit(NPC_CREDIT_2);
                credited = true;
            }

            if (!credited && talk)
                Talk(TALK_DIED, player);
        }

        EventMap _events;
    };
};

void AddSC_eastern_plaguelands()
{
    new npc_ghoul_flayer();
    new npc_augustus_the_touched();
    new npc_darrowshire_spirit();
    new npc_tirion_fordring();
    new npc_scourge_siege_engineer();
}
