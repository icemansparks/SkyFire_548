/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "NpcTextLocaleColumns.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace
{
    using namespace Skyfire::NpcTextColumns;

    bool Expect(bool condition, char const* message)
    {
        if (!condition)
            std::cerr << message << '\n';
        return condition;
    }

    std::vector<std::string> Columns()
    {
        std::vector<std::string> columns{ "ID" };
        std::stringstream list(SelectList());
        std::string column;
        while (std::getline(list, column, ','))
            columns.push_back(column.substr(column.find_first_not_of(' ')));
        return columns;
    }

    bool TestColumnCount()
    {
        // `locales_npc_text`: ID plus 11 locales of 12 options with two texts each
        return Expect(Columns().size() == 1 + 11 * 12 * 2, "The select list should hold 264 locale columns after ID");
    }

    bool TestEveryFieldIndexReadsItsOwnColumn()
    {
        std::vector<std::string> const columns = Columns();
        bool ok = true;
        for (std::uint8_t locale = 1; locale <= LOCALES; ++locale)
            for (std::uint8_t option = 0; option < OPTIONS; ++option)
                for (std::uint8_t text = 0; text < 2; ++text)
                {
                    std::string const want = "Text" + std::to_string(option) + "_" + std::to_string(text) + "_loc" + std::to_string(locale);
                    std::size_t const index = FieldIndex(locale, option, text);
                    if (index >= columns.size() || columns[index] != want)
                    {
                        std::cerr << "FieldIndex(" << int(locale) << ", " << int(option) << ", " << int(text) << ") should read " << want << '\n';
                        ok = false;
                    }
                }
        return ok;
    }

    bool TestGermanGreeting()
    {
        // deDE is locale 3: its first text starts after ID and two locales of 24 columns
        return Expect(FieldIndex(3, 0, 0) == 49 && Columns()[49] == "Text0_0_loc3", "deDE option 0 should read field 49, Text0_0_loc3");
    }
}

int main()
{
    bool ok = true;
    ok &= TestColumnCount();
    ok &= TestEveryFieldIndexReadsItsOwnColumn();
    ok &= TestGermanGreeting();
    return ok ? 0 : 1;
}
