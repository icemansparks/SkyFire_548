/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_LOOT_LOCKOUT_H
#define SKYFIRE_LOOT_LOCKOUT_H

#include <cstdint>
#include <set>
#include <tuple>

// Weekly per-character loot lockouts, kept free of game headers so unit tests can include it.
// Stored in characters.character_loot_lockout and cleared at the weekly reset.
enum class LootLockoutType : std::uint8_t
{
    PersonalLoot = 0,   // world bosses, raid finder and flex: one personal roll per boss per week
    BonusLoot    = 1,   // one bonus roll per boss per difficulty per week
    Max
};

class LootLockoutSet
{
public:
    bool Has(LootLockoutType type, std::uint32_t entry, std::uint8_t difficulty) const
    {
        return _lockouts.count(Key(type, entry, difficulty)) != 0;
    }

    // Returns false when the lockout was already held or the type is unknown.
    bool Add(LootLockoutType type, std::uint32_t entry, std::uint8_t difficulty)
    {
        if (type >= LootLockoutType::Max)
            return false;

        return _lockouts.insert(Key(type, entry, difficulty)).second;
    }

    void Clear() { _lockouts.clear(); }
    std::size_t Size() const { return _lockouts.size(); }

private:
    typedef std::tuple<std::uint8_t, std::uint32_t, std::uint8_t> LockoutKey;

    static LockoutKey Key(LootLockoutType type, std::uint32_t entry, std::uint8_t difficulty)
    {
        return LockoutKey(std::uint8_t(type), entry, difficulty);
    }

    std::set<LockoutKey> _lockouts;
};

#endif
