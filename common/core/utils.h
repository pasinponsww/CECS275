/**
 * @file utils.h
 * @brief Shared tax-data storage and file-loading helpers for the GUI app.
 *
 * @details This layer owns the arrays populated from the bundled tax data
 * files. It intentionally does not contain console menus, prompts, or output
 * screens. GUI code reads this loaded data and math code performs calculations.
 */

#pragma once

#include <string>

#include "constants.h"

/**
 * @note Shared tax data arrays are declared here and defined in utils.cpp.
 */
extern double taxDeduction[NUM_YEARS][NUM_STATUSES];
extern double taxBracket[NUM_YEARS][NUM_STATUSES][BRACKET_ARRAY_SIZE];
extern int numBrackets[NUM_YEARS][NUM_STATUSES];
extern bool bracketLoaded[NUM_YEARS];
extern bool deductionLoaded;

/**
 * @brief Clears all loaded tax arrays and resets loaded flags.
 */
void initializeTaxData();

/**
 * @brief Reads standard deduction data from a raw text file.
 *
 * @param filename Path to the standard deduction data file.
 * @return true when at least one valid deduction row was loaded.
 */
bool loadDeductionData(const std::string& filename);

/**
 * @brief Reads federal tax bracket data from a raw CSV file.
 *
 * @param filename Path to the tax bracket CSV file.
 * @return true when at least one valid bracket year was loaded.
 */
bool loadBracketData(const std::string& filename);
