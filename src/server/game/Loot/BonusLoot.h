/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_BONUS_LOOT_H
#define SKYFIRE_BONUS_LOOT_H

#include "Define.h"

class Player;
class WorldObject;

// One row of `bonus_loot_template`.
struct BonusLootTemplate
{
    uint32 Entry = 0;           // base creature entry of the boss
    uint32 Spell = 0;           // prompt aura (SPELL_AURA_BONUS_ROLL_TRIGGER)
    uint32 Currency = 0;        // spent by the roll
    uint32 LootIdNormal = 0;
    uint32 LootIdHeroic = 0;
    uint8 Source = 0;           // Skyfire::BonusLoot::SOURCE_*
};

// Bonus rolls: a player spends one boss currency for an extra roll on the boss's loot,
// once per boss and difficulty a week.
namespace Skyfire::BonusLoot
{
    void Load();                                            // after the spell info store and creature templates
    BonusLootTemplate const* Get(uint32 entry);
    BonusLootTemplate const* GetForChestLoot(uint32 lootId); // gameobject source whose chest has this loot id

    // Gives the player the roll prompt (the row's aura) when they hold the currency, are not
    // locked out and have no prompt for it yet. source is the boss or chest; the aura keeps it as
    // its caster and the difficulty as its amount, because several bosses share one prompt spell.
    // A player holds one prompt per prompt spell: a newer boss's prompt replaces the older one.
    void GrantPrompt(Player* player, BonusLootTemplate const& loot, uint8 difficulty, WorldObject* source);
    // The row a prompt aura was granted for, from the aura's caster.
    BonusLootTemplate const* GetForPrompt(uint64 casterGuid);

    // Spends the currency, sets the weekly lockout and rolls: an item for the player's loot
    // specialization at the configured chance, else gold. False when the player is locked out
    // or lacks the currency; nothing is spent then.
    bool Roll(Player* player, BonusLootTemplate const& loot, uint8 difficulty);
}

#endif
