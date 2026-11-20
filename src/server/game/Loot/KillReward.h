/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_KILL_REWARD_H
#define SKYFIRE_KILL_REWARD_H

#include "Define.h"

#include <vector>

class Creature;
class GameObject;
class Item;
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

// Rewards a creature kill gives beyond experience: currency on kill, personal loot and the
// bonus roll prompt.
namespace Skyfire::KillReward
{
    void LoadCreatureLootCurrency();
    CreatureLootCurrencyList const* GetCreatureLootCurrency(uint32 entry);

    void LoadPersonalLoot();
    PersonalLootTemplate const* GetPersonalLoot(uint32 entry);

    // True when this kill pays personal loot instead of corpse loot (world boss in the open world, raid finder, flex).
    bool UsesPersonalLoot(Creature const* creature);

    // True for flex items (ItemNameDescription "Flexible").
    bool IsFlexibleItem(uint32 itemId);

    // Stores a loot item in the player's bags, or mails it from the boss when they are full.
    // what names the reward in the mail subject and chat ("Personal loot"). Returns false when the
    // item could not be created (the player gets nothing); true when stored or mailed. stored,
    // when given, receives the stored item, nullptr when it was mailed.
    bool GiveLootItem(Player* player, uint32 bossEntry, uint32 itemId, char const* what, Item** stored = nullptr);

    // Called once per creature kill with the player credited for it (the loot recipient).
    // Rewards each group member in reward range, or the killer alone; on a world boss with
    // personal loot also every other player in reward range who fought it.
    void Reward(Player* killer, Creature* victim);

    // Called when a chest's loot is filled on first opening: a boss whose loot sits in that chest
    // gives the bonus roll prompt to the opener's group members in reward range.
    void RewardChestOpen(Player* opener, GameObject* chest);
}

#endif
