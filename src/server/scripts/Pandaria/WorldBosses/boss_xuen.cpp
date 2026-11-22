/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/* ScriptData
SDName: Boss_Xuen
SD%Complete: 85
SDComment: Timeless Isle Celestial Court. Gaps: no dynamic health scaling by group size,
           no personal loot or currency, no guild achievement credit, and the caster
           preference of Leap and the ranged filter of the two area spells use the player's
           class (SkyFire has no talent role lookup).
SDCategory: Timeless Isle
EndScriptData */

#include "celestial_court.h"
#include "SpellScript.h"
#include "Containers.h"

enum XuenSpells
{
    SPELL_AGILITY                   = 144631,
    SPELL_CHI_BARRAGE               = 144642,   // area spell, scripted below
    SPELL_CRACKLING_LIGHTNING       = 144635,   // periodic aura on Xuen; each tick triggers 144634
    SPELL_LEAP                      = 144640,
    SPELL_SPECTRAL_SWIPE            = 144638,
    SPELL_SPECTRAL_SWIPE_TRIGGER    = 144652
};

enum XuenEvents
{
    EVENT_SPECTRAL_SWIPE = 1,
    EVENT_CHI_BARRAGE,
    EVENT_CRACKLING_LIGHTNING,
    EVENT_AGILITY,
    EVENT_LEAP
};

// creature_text groups of Xuen (71953).
enum XuenTexts
{
    SAY_AGGRO       = 0,
    SAY_INTRO       = 1,
    SAY_DEATH       = 2,
    SAY_KILL        = 3,
    SAY_AGILITY     = 4,
    SAY_CHI_BARRAGE = 5,
    SAY_CRACKLING   = 6
};

enum XuenData
{
    MAX_AREA_TARGETS = 8
};

class boss_xuen : public CreatureScript
{
public:
    boss_xuen() : CreatureScript("boss_xuen") { }

    struct boss_xuenAI : public CelestialAI
    {
        boss_xuenAI(Creature* creature) : CelestialAI(creature, SHAOHAO_OUTRO_XUEN, SAY_AGGRO, SAY_INTRO, SAY_DEATH, SAY_KILL) { }

        void MovementInform(uint32 type, uint32 pointId) OVERRIDE
        {
            // Leap landed: Spectral Swipe where Xuen stands.
            if (type == EFFECT_MOTION_TYPE && pointId == EVENT_JUMP)
                DoCast(me, SPELL_SPECTRAL_SWIPE_TRIGGER, true);

            CelestialAI::MovementInform(type, pointId);
        }

    protected:
        void ScheduleCombatEvents() OVERRIDE
        {
            events.ScheduleEvent(EVENT_CHI_BARRAGE, 28000);
            events.ScheduleEvent(EVENT_AGILITY, 39000);
            events.ScheduleEvent(EVENT_CRACKLING_LIGHTNING, 42000);
            events.ScheduleEvent(EVENT_SPECTRAL_SWIPE, urand(5000, 15000));
            events.ScheduleEvent(EVENT_LEAP, 16000);
        }

        void ExecuteCombatEvent(uint32 eventId) OVERRIDE
        {
            switch (eventId)
            {
                case EVENT_SPECTRAL_SWIPE:
                    DoCastVictim(SPELL_SPECTRAL_SWIPE);
                    events.ScheduleEvent(EVENT_SPECTRAL_SWIPE, urand(10000, 15000));
                    break;
                case EVENT_AGILITY:
                    Talk(SAY_AGILITY);
                    DoCast(me, SPELL_AGILITY);
                    events.ScheduleEvent(EVENT_AGILITY, 40000);
                    break;
                case EVENT_LEAP:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, RangedPlayerSelector()))
                        DoCast(target, SPELL_LEAP);
                    else if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 40.0f, true))
                        DoCast(target, SPELL_LEAP);
                    break;
                case EVENT_CRACKLING_LIGHTNING:
                    Talk(SAY_CRACKLING);
                    DoCast(me, SPELL_CRACKLING_LIGHTNING);
                    events.ScheduleEvent(EVENT_CRACKLING_LIGHTNING, 42000);
                    events.ScheduleEvent(EVENT_LEAP, 30000);
                    break;
                case EVENT_CHI_BARRAGE:
                    Talk(SAY_CHI_BARRAGE);
                    DoCast(me, SPELL_CHI_BARRAGE);
                    events.ScheduleEvent(EVENT_CHI_BARRAGE, urand(20000, 25000));
                    break;
                default:
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_xuenAI(creature);
    }
};

// 144642 - Chi Barrage and 144634 - Crackling Lightning: both hit up to 8 players around Xuen
// (ranged players first) and cast the spell id held in effect 0's base points on each of them (144633 and 144643). Only
// players are targeted.
class spell_xuen_area_barrage : public SpellScriptLoader
{
public:
    spell_xuen_area_barrage() : SpellScriptLoader("spell_xuen_area_barrage") { }

    class spell_xuen_area_barrage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_xuen_area_barrage_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            std::list<WorldObject*> ranged;
            std::list<WorldObject*> others;
            for (std::list<WorldObject*>::const_iterator itr = targets.begin(); itr != targets.end(); ++itr)
            {
                Unit* unit = (*itr)->ToUnit();
                if (!unit || unit->GetTypeId() != TypeID::TYPEID_PLAYER)
                    continue;

                if (IsRangedClass(unit))
                    ranged.push_back(*itr);
                else
                    others.push_back(*itr);
            }

            // Ranged players first; the free slots go to random other players.
            if (ranged.size() > MAX_AREA_TARGETS)
                Skyfire::Containers::RandomResizeList(ranged, MAX_AREA_TARGETS);
            else if (ranged.size() + others.size() > MAX_AREA_TARGETS)
                Skyfire::Containers::RandomResizeList(others, MAX_AREA_TARGETS - ranged.size());

            ranged.splice(ranged.end(), others);
            targets = ranged;
        }

        void HandleScriptEffect(SpellEffIndex effIndex)
        {
            if (Unit* target = GetHitUnit())
                GetCaster()->CastSpell(target, GetSpellInfo()->Effects[effIndex].BasePoints, true);
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_xuen_area_barrage_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnEffectHitTarget += SpellEffectFn(spell_xuen_area_barrage_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_xuen_area_barrage_SpellScript();
    }
};

void AddSC_boss_xuen()
{
    new boss_xuen();
    new spell_xuen_area_barrage();
}
