/**
 * @file taxmath.cpp
 * @brief Implementations of pure math and lookup functions.
 * @author Bex Saw
 * @date 2026
 */

#include <algorithm>
#include <cctype>
#include <stdexcept>
#include "taxmath.h"
#include "constants.h"

using namespace std;

namespace Tax
{

// Status index conversion

int statusToIndex(char s)
{
    switch (toupper(static_cast<unsigned char>(s)))
    {
    case 'S':
        return 0;
    case 'P':
        return 1;
    case 'H':
        return 2;
    case 'J':
        return 3;
    default:
        return -1;
    }
}

char indexToStatus(int idx)
{
    if (idx >= 0 && idx < NUM_STATUSES)
    {
        return STATUS_LABELS[idx];
    }

    return '?';
}

// *** String parsing helpers ***

string stripChars(const string& s, const string& chars)
{
    string result;
    result.reserve(s.size()); // reserve space to avoid multiple allocations
    for (char c : s)
    {
        if (chars.find(c) == string::npos)
        {
            result += c;
        }
    }

    return result;
}

double parseDollar(const string& s)
{
    string clean = stripChars(s, "$,\" \r\n");
    if (clean.empty())
    {
        return -1.0;
    }

    try
    {
        return stod(clean);
    }
    catch (...)
    {
        return -1.0;
    }
}

double parsePercent(const string& s)
{
    string clean = stripChars(s, "% \"\r\n");
    if (clean.empty())
    {
        return -1.0;
    }

    try
    {
        return stod(clean);
    }
    catch (...)
    {
        return -1.0;
    }
}

// *** Year/index arithmetic ***

int yearToIndex(int year)
{
    return year - START_YEAR;
}

bool isValidYear(int year)
{
    return year >= START_YEAR && year <= END_YEAR;
}

bool isValidStatus(char s)
{
    return statusToIndex(s) != -1;
}

int filingStatusToIndex(FilingStatus status)
{
    return static_cast<int>(status);
}

FilingStatus charToFilingStatus(char s)
{
    switch (toupper(static_cast<unsigned char>(s)))
    {
    case 'S':
        return FilingStatus::Single;
    case 'P':
        return FilingStatus::MarriedSeparately;
    case 'H':
        return FilingStatus::HeadOfHousehold;
    case 'J':
        return FilingStatus::MarriedJointly;
    default:
        return FilingStatus::Single;
    }
}

char filingStatusToChar(FilingStatus status)
{
    return indexToStatus(filingStatusToIndex(status));
}

double standardDeductionFor(int year, FilingStatus status,
                            const double deductions[NUM_YEARS][NUM_STATUSES])
{
    if (!isValidYear(year))
    {
        return 0.0;
    }

    int statusIndex = filingStatusToIndex(status);
    if (statusIndex < 0 || statusIndex >= NUM_STATUSES)
    {
        return 0.0;
    }

    return deductions[yearToIndex(year)][statusIndex];
}

double taxableIncomeAfterDeduction(double income, double deduction)
{
    double taxableIncome = income - deduction;
    return taxableIncome > 0.0 ? taxableIncome : 0.0;
}

double progressiveTaxFor(double taxableIncome, int year, FilingStatus status,
                         const double brackets[NUM_YEARS][NUM_STATUSES][BRACKET_ARRAY_SIZE],
                         const int bracketCounts[NUM_YEARS][NUM_STATUSES])
{
    if (taxableIncome <= 0.0 || !isValidYear(year))
    {
        return 0.0;
    }

    int yearIndex = yearToIndex(year);
    int statusIndex = filingStatusToIndex(status);
    if (statusIndex < 0 || statusIndex >= NUM_STATUSES)
    {
        return 0.0;
    }

    double tax = 0.0;
    double lower = 0.0;
    int count = bracketCounts[yearIndex][statusIndex];

    for (int bracket = 0; bracket < count; bracket++)
    {
        double rate = brackets[yearIndex][statusIndex][bracket * 2] / 100.0;
        double upper = brackets[yearIndex][statusIndex][bracket * 2 + 1];

        if (upper < 0.0)
        {
            tax += (taxableIncome - lower) * rate;
            break;
        }

        if (taxableIncome > lower)
        {
            double taxableAtRate = min(taxableIncome, upper) - lower;
            tax += taxableAtRate * rate;
        }

        if (taxableIncome <= upper)
        {
            break;
        }

        lower = upper;
    }

    return tax;
}

} // namespace Tax
