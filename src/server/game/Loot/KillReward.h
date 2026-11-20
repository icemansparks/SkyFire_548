/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_KILL_REWARD_H
#define SKYFIRE_KILL_REWARD_H

#include "Define.h"

#include <vector>

class Creature;
class Player;

// One row of `creature_loot_currency`: currency paid straight into the currency tab when the creature dies.
struct CreatureLootCurrency
{
    uint32 LootMode;    // difficulty bitmask (1 << DifficultyID), 0 = any difficulty
    uint32 CurrencyId;
    uint32 Count;       // storage units: hundredths for high precision currencies such as Justice and Valor
};

typedef std::vector<CreatureLootCurrency> CreatureLootCurrencyList;

// One row of `personal_loot_template` with its `personal_loot_item` rows.
struct PersonalLootTemplate
{
    uint32 MoneyBag = 0;            // gold bag when the roll fails or no item fits
    uint32 MoneyBagFlex = 0;        // flex gold bag, 0 = MoneyBag
    uint32 QuestTracker = 0;        // weekly tracking quest marked done on receipt
    std::vector<uint32> Items;
};

// Rewards a creature kill gives beyond experience: currency on kill and personal loot.
// The bonus roll trigger extends Reward().
namespace Skyfire::KillReward
{
    void LoadCreatureLootCurrency();
    CreatureLootCurrencyList const* GetCreatureLootCurrency(uint32 entry);

    void LoadPersonalLoot();
    PersonalLootTemplate const* GetPersonalLoot(uint32 entry);

    // True when this kill pays personal loot instead of corpse loot (world boss in the open world, raid finder, flex).
    bool UsesPersonalLoot(Creature const* creature);

    // Called once per creature kill with the player credited for it (the loot recipient).
    // Rewards each group member in reward range, or the killer alone; on a world boss with
    // personal loot also every other player in reward range who fought it.
    void Reward(Player* killer, Creature* victim);
}

#endif
