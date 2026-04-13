/**
 * @file taxmath.h
 * @brief Pure math and lookup functions for the Tax Data Extraction program.
 *        No I/O here — only parsing, indexing, and numerical operations.
 * @author Bex Saw
 * @date 2026
 */

#pragma once

#include <string>
#include "constants.h"

namespace Tax
{
/**
 * @brief Type-safe filing status used by the structured tax API.
 */
enum class FilingStatus : int
{
    Single = 0,
    MarriedSeparately = 1,
    HeadOfHousehold = 2,
    MarriedJointly = 3
};

/**
 * @brief Converts a filing-status character to its internal array index.
 * @param s  'S', 'P', 'H', or 'J' (case-insensitive)
 * @return   0=S, 1=P, 2=H, 3=J, or -1 if unrecognized
 */
int statusToIndex(char s);

/**
 * @brief Converts an internal status index back to its character label.
 * @param idx  0–3
 * @return     'S', 'P', 'H', or 'J'
 */
char indexToStatus(int idx);

// String parsing helpers

/**
 * @brief Removes every character in `chars` from `s`.
 * @param s      Source string
 * @param chars  Set of characters to strip
 * @return       Cleaned string
 */
std::string stripChars(const std::string& s, const std::string& chars);

/**
 * @brief Parses a dollar-amount string (e.g. "$23,850") into a double.
 * @param s  Raw string that may contain '$', ',', quotes, or whitespace
 * @return   Numeric value, or -1.0 on parse failure
 */
double parseDollar(const std::string& s);

/**
 * @brief Parses a percentage string (e.g. "22.0%") into a double.
 * @param s  Raw string that may contain '%', quotes, or whitespace
 * @return   Numeric value (e.g. 22.0), or -1.0 on parse failure
 */
double parsePercent(const std::string& s);

// Year/index arithmetic

/**
 * @brief Converts a calendar year to its array index.
 * @param year  Calendar year (e.g. 2025)
 * @return      year - START_YEAR
 */
int yearToIndex(int year);

/**
 * @brief Checks whether a calendar year falls within the supported range.
 * @param year  Calendar year to validate
 * @return      true if START_YEAR <= year <= END_YEAR
 */
bool isValidYear(int year);

/**
 * @brief Checks whether a status character is one of S, P, H, J.
 * @param s  Character to validate (case-insensitive)
 * @return   true if valid
 */
bool isValidStatus(char s);

/**
 * @brief Converts a filing-status enum value to its internal array index.
 * @param status  Filing status enum value
 * @return        0=S, 1=P, 2=H, 3=J
 */
int filingStatusToIndex(FilingStatus status);

/**
 * @brief Converts a filing-status character into a FilingStatus value.
 * @param s  'S', 'P', 'H', or 'J' (case-insensitive)
 * @return   Matching FilingStatus value, or Single if unrecognized
 */
FilingStatus charToFilingStatus(char s);

/**
 * @brief Converts a FilingStatus value back to its character label.
 * @param status  FilingStatus enum value
 * @return        'S', 'P', 'H', or 'J'
 */
char filingStatusToChar(FilingStatus status);

/**
 * @brief Finds the standard deduction for a year/status from loaded deduction data.
 */
double standardDeductionFor(int year, FilingStatus status,
                            const double deductions[NUM_YEARS][NUM_STATUSES]);

/**
 * @brief Applies the standard deduction and floors taxable income at zero.
 */
double taxableIncomeAfterDeduction(double income, double deduction);

/**
 * @brief Calculates progressive tax from the flattened bracket array.
 */
double progressiveTaxFor(double taxableIncome, int year, FilingStatus status,
                         const double brackets[NUM_YEARS][NUM_STATUSES][BRACKET_ARRAY_SIZE],
                         const int bracketCounts[NUM_YEARS][NUM_STATUSES]);

} // namespace Tax
