/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Npc_Emperor_Shaohao
SD%Complete: 80
SDComment: Timeless Isle Celestial Court. Calls the celestials one after the other.
SDCategory: Timeless Isle
EndScriptData */

#include "celestial_court.h"
#include <algorithm>
#include <vector>

enum ShaohaoEvents
{
    EVENT_REGROUP = 1
};

uint32 const REGROUP_AFTER_RESPAWN = 5500;  // the celestial respawns 4 s after it despawns
uint32 const REGROUP_RETRY = 2000;

class npc_emperor_shaohao : public CreatureScript
{
public:
    npc_emperor_shaohao() : CreatureScript("npc_emperor_shaohao") { }

    struct npc_emperor_shaohaoAI : public ScriptedAI
    {
        npc_emperor_shaohaoAI(Creature* creature) : ScriptedAI(creature), previous(0) { }

        void Reset() OVERRIDE
        {
            me->setActive(true);
            events.Reset();
            bag.clear();
            previous = 0;
            events.ScheduleEvent(EVENT_REGROUP, 5500);
        }

        // One round calls every celestial once in random order; a new round never starts with
        // the celestial that ended the last one.
        uint32 NextCelestial()
        {
            if (bag.empty())
            {
                for (uint32 i = 0; i < sizeof(CelestialsInRotation) / sizeof(uint32); ++i)
                    bag.push_back(CelestialsInRotation[i]);
                // Fisher-Yates shuffle.
                for (size_t i = bag.size(); i > 1; --i)
                    std::swap(bag[i - 1], bag[urand(0, i - 1)]);
                if (bag.size() > 1 && bag.front() == previous)
                    std::swap(bag.front(), bag.back());
            }

            uint32 entry = bag.front();
            bag.erase(bag.begin());
            return entry;
        }

        static uint8 IntroText(uint32 entry)
        {
            switch (entry)
            {
                case NPC_YU_LON:  return SHAOHAO_INTRO_YU_LON;
                case NPC_XUEN:    return SHAOHAO_INTRO_XUEN;
                case NPC_CHI_JI:  return SHAOHAO_INTRO_CHI_JI;
                default:          return SHAOHAO_INTRO_NIUZAO;
            }
        }

        void DoAction(int32 action) OVERRIDE
        {
            if (action == ACTION_CELESTIAL_DONE)
            {
                events.ScheduleEvent(EVENT_REGROUP, REGROUP_AFTER_RESPAWN);
                return;
            }

            if (action != ACTION_SELECT_CELESTIAL)
                return;

            uint32 entry = NextCelestial();
            if (Creature* celestial = me->FindNearestCreature(entry, 500.0f, true))
            {
                previous = entry;
                celestial->GetMotionMaster()->MovePoint(POINT_COURT_MIDDLE, CourtMiddle);
                Talk(IntroText(entry));
            }
            else
            {
                // The celestial is not there (still respawning): put it back and try again shortly.
                bag.insert(bag.begin(), entry);
                events.ScheduleEvent(EVENT_REGROUP, REGROUP_RETRY);
            }
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            events.Update(diff);

            if (events.ExecuteEvent() == EVENT_REGROUP)
                DoAction(ACTION_SELECT_CELESTIAL);
        }

    private:
        EventMap events;
        std::vector<uint32> bag;
        uint32 previous;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_emperor_shaohaoAI(creature);
    }
};

void AddSC_npc_emperor_shaohao()
{
    new npc_emperor_shaohao();
}
