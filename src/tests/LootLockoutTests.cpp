/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "LootLockout.h"

#include <iostream>

namespace
{
    // DifficultyID values of 5.4.8 (DBCEnums.h)
    constexpr std::uint8_t NONE = 0;
    constexpr std::uint8_t RAID_FINDER = 7;
    constexpr std::uint8_t FLEX = 14;

    constexpr std::uint32_t SHA_OF_ANGER = 60491;
    constexpr std::uint32_t IMMERSEUS = 71543;

    bool Expect(bool condition, char const* message)
    {
        if (!condition)
            std::cerr << message << '\n';

        return condition;
    }

    bool TestSecondAddIsRefused()
    {
        LootLockoutSet set;
        bool first = set.Add(LootLockoutType::PersonalLoot, SHA_OF_ANGER, NONE);
        bool second = set.Add(LootLockoutType::PersonalLoot, SHA_OF_ANGER, NONE);
        return Expect(first && !second && set.Size() == 1 && set.Has(LootLockoutType::PersonalLoot, SHA_OF_ANGER, NONE),
            "A boss locked once should report the lockout and refuse a second add");
    }

    bool TestDifficultiesAreSeparate()
    {
        LootLockoutSet set;
        set.Add(LootLockoutType::PersonalLoot, IMMERSEUS, RAID_FINDER);
        return Expect(set.Has(LootLockoutType::PersonalLoot, IMMERSEUS, RAID_FINDER)
                && !set.Has(LootLockoutType::PersonalLoot, IMMERSEUS, FLEX),
            "A raid finder lockout should not lock the same boss on flex");
    }

    bool TestTypesAreSeparate()
    {
        LootLockoutSet set;
        set.Add(LootLockoutType::PersonalLoot, SHA_OF_ANGER, NONE);
        return Expect(!set.Has(LootLockoutType::BonusLoot, SHA_OF_ANGER, NONE)
                && set.Add(LootLockoutType::BonusLoot, SHA_OF_ANGER, NONE),
            "A personal loot lockout should not lock the bonus roll of the same boss");
    }

    bool TestUnknownTypeIsRefused()
    {
        LootLockoutSet set;
        return Expect(!set.Add(LootLockoutType::Max, SHA_OF_ANGER, NONE) && set.Size() == 0,
            "An unknown lockout type should not be stored");
    }

    bool TestClearDropsEverything()
    {
        LootLockoutSet set;
        set.Add(LootLockoutType::PersonalLoot, SHA_OF_ANGER, NONE);
        set.Add(LootLockoutType::BonusLoot, IMMERSEUS, FLEX);
        set.Clear();
        return Expect(set.Size() == 0 && !set.Has(LootLockoutType::PersonalLoot, SHA_OF_ANGER, NONE)
                && set.Add(LootLockoutType::PersonalLoot, SHA_OF_ANGER, NONE),
            "The weekly reset should drop every lockout and allow the boss again");
    }
}

int main()
{
    bool ok = TestSecondAddIsRefused();
    ok = TestDifficultiesAreSeparate() && ok;
    ok = TestTypesAreSeparate() && ok;
    ok = TestUnknownTypeIsRefused() && ok;
    ok = TestClearDropsEverything() && ok;
    return ok ? 0 : 1;
}
