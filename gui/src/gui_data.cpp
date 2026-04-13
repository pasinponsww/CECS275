/**
 * @file gui_data.cpp
 * @brief Loads shared tax data for the GTK app.
 */

#include "gui_data.h"

#include "../../common/core/utils.h"
#include "../../common/math/taxmath.h"
#include "constants.h"

#include <cstdio>
#include <string>

namespace
{

std::string first_existing_path(const std::string& projectRootPath, const std::string& guiPath)
{
    FILE* file = fopen(projectRootPath.c_str(), "r");
    if (file)
    {
        fclose(file);
        return projectRootPath;
    }

    file = fopen(guiPath.c_str(), "r");
    if (file)
    {
        fclose(file);
        return guiPath;
    }

    return projectRootPath;
}

} // namespace

namespace GuiData
{

void load_all()
{
    initializeTaxData();

    std::string deductionPath =
        first_existing_path("data/tax_deduction.txt", "../data/tax_deduction.txt");

    std::string bracketPath =
        first_existing_path("data/tax_rate_and_bracket.txt", "../data/tax_rate_and_bracket.txt");

    loadDeductionData(deductionPath);
    loadBracketData(bracketPath);

    int yearIndex = Tax::yearToIndex(2025);
    taxDeduction[yearIndex][Tax::statusToIndex('S')] = 15750.0;
    taxDeduction[yearIndex][Tax::statusToIndex('P')] = 15750.0;
    taxDeduction[yearIndex][Tax::statusToIndex('H')] = 23625.0;
    taxDeduction[yearIndex][Tax::statusToIndex('J')] = 31500.0;
}

bool is_loaded()
{
    return deductionLoaded;
}

} // namespace GuiData
