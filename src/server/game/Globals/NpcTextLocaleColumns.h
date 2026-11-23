/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_NPC_TEXT_LOCALE_COLUMNS_H
#define SKYFIRE_NPC_TEXT_LOCALE_COLUMNS_H

#include <cstddef>
#include <cstdint>
#include <string>

// Column layout of `locales_npc_text`, kept free of game headers so unit tests can include it.
// Per locale (loc1 to loc11) the table has 12 options of two texts each, in the order
// Text0_0_locN, Text0_1_locN, Text1_0_locN, ... Text11_1_locN.
namespace Skyfire::NpcTextColumns
{
    constexpr std::uint8_t LOCALES = 11;
    constexpr std::uint8_t OPTIONS = 12;

    // Select list of all locale columns, locale by locale, without the leading ID.
    inline std::string SelectList()
    {
        std::string list;
        for (std::uint32_t locale = 1; locale <= LOCALES; ++locale)
            for (std::uint32_t option = 0; option < OPTIONS; ++option)
                for (std::uint32_t text = 0; text < 2; ++text)
                {
                    if (!list.empty())
                        list += ", ";
                    list += "Text" + std::to_string(option) + "_" + std::to_string(text) + "_loc" + std::to_string(locale);
                }
        return list;
    }

    // Field index of TextOPTION_TEXT_locLOCALE in a row of "SELECT ID, " + SelectList().
    constexpr std::size_t FieldIndex(std::uint8_t locale, std::uint8_t option, std::uint8_t text)
    {
        return 1 + std::size_t(2) * OPTIONS * (locale - 1) + 2 * option + text;
    }
}

#endif
