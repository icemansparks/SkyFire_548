/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_Xuen
SD%Complete: 40
SDComment: Timeless Isle Celestial Court. The fight skeleton only; Xuen's abilities follow in
           a separate change.
SDCategory: Timeless Isle
EndScriptData */

#include "celestial_court.h"

// creature_text groups of Xuen (71953).
enum XuenTexts
{
    SAY_AGGRO   = 0,
    SAY_INTRO   = 1,
    SAY_DEATH   = 2,
    SAY_KILL    = 3
};

class boss_xuen : public CreatureScript
{
public:
    boss_xuen() : CreatureScript("boss_xuen") { }

    struct boss_xuenAI : public CelestialAI
    {
        boss_xuenAI(Creature* creature) : CelestialAI(creature, SHAOHAO_OUTRO_XUEN, SAY_AGGRO, SAY_INTRO, SAY_DEATH, SAY_KILL) { }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_xuenAI(creature);
    }
};

void AddSC_boss_xuen()
{
    new boss_xuen();
}
