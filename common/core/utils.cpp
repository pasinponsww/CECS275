/**
 * @file utils.cpp
 * @brief Implements shared tax-data storage and file loaders for the GUI app.
 */

#include "utils.h"
#include <cctype>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "taxmath.h"

using namespace std;
using namespace Tax;

double taxDeduction[NUM_YEARS][NUM_STATUSES];
double taxBracket[NUM_YEARS][NUM_STATUSES][BRACKET_ARRAY_SIZE];
int numBrackets[NUM_YEARS][NUM_STATUSES];
bool bracketLoaded[NUM_YEARS];
bool deductionLoaded = false;

namespace
{
struct BracketRow
{
    int year;
    double rate[NUM_STATUSES];
    double lower[NUM_STATUSES];
};

vector<string> parseCsvLine(const string& line)
{
    vector<string> fields;
    bool inQuote = false;
    string field;

    for (char c : line)
    {
        if (c == '"')
        {
            inQuote = !inQuote;
        }
        else if (c == ',' && !inQuote)
        {
            fields.push_back(field);
            field.clear();
        }
        else
        {
            field += c;
        }
    }

    fields.push_back(field);
    return fields;
}

} // namespace

void initializeTaxData()
{
    for (int i = 0; i < NUM_YEARS; i++)
    {
        bracketLoaded[i] = false;
        for (int j = 0; j < NUM_STATUSES; j++)
        {
            taxDeduction[i][j] = 0.0;
            numBrackets[i][j] = 0;
            for (int k = 0; k < BRACKET_ARRAY_SIZE; k++)
            {
                taxBracket[i][j][k] = 0.0;
            }
        }
    }

    deductionLoaded = false;
}

bool loadDeductionData(const string& filename)
{
    ifstream fin(filename);
    if (!fin)
    {
        deductionLoaded = false;
        return false;
    }

    for (int i = 0; i < NUM_YEARS; i++)
    {
        for (int j = 0; j < NUM_STATUSES; j++)
        {
            taxDeduction[i][j] = 0.0;
        }
    }

    bool loadedAny = false;
    string line;

    while (getline(fin, line))
    {
        istringstream iss(line);
        vector<string> tokens;
        string token;

        while (iss >> token)
        {
            tokens.push_back(token);
        }

        for (int t = 0; t + 3 < static_cast<int>(tokens.size()); t++)
        {
            string yearText = stripChars(tokens[t], "$,\"\r\n");
            if (yearText.size() != 4 || !isdigit(static_cast<unsigned char>(yearText[0])))
            {
                continue;
            }

            int year = stoi(yearText);
            if (!isValidYear(year))
            {
                continue;
            }

            double single = parseDollar(tokens[t + 1]);
            double headOfHousehold = parseDollar(tokens[t + 2]);
            double married = parseDollar(tokens[t + 3]);
            if (single < 0.0 || headOfHousehold < 0.0 || married < 0.0)
            {
                continue;
            }

            int yi = yearToIndex(year);
            taxDeduction[yi][statusToIndex('S')] = single;
            taxDeduction[yi][statusToIndex('P')] = single;
            taxDeduction[yi][statusToIndex('H')] = headOfHousehold;
            taxDeduction[yi][statusToIndex('J')] = married;
            loadedAny = true;
            t += 3;
        }
    }

    deductionLoaded = loadedAny;
    return loadedAny;
}

bool loadBracketData(const string& filename)
{
    ifstream fin(filename);
    if (!fin)
    {
        for (int i = 0; i < NUM_YEARS; i++)
        {
            bracketLoaded[i] = false;
        }
        return false;
    }

    for (int i = 0; i < NUM_YEARS; i++)
    {
        bracketLoaded[i] = false;
        for (int j = 0; j < NUM_STATUSES; j++)
        {
            numBrackets[i][j] = 0;
            for (int k = 0; k < BRACKET_ARRAY_SIZE; k++)
            {
                taxBracket[i][j][k] = 0.0;
            }
        }
    }

    vector<BracketRow> rows;
    string line;
    getline(fin, line);

    while (getline(fin, line))
    {
        vector<string> fields = parseCsvLine(line);
        if (static_cast<int>(fields.size()) < CSV_MIN_FIELDS)
        {
            continue;
        }

        string yearText = stripChars(fields[0], " \r\n");
        if (yearText.empty() || !isdigit(static_cast<unsigned char>(yearText[0])))
        {
            continue;
        }

        int year = stoi(yearText);
        if (!isValidYear(year))
        {
            continue;
        }

        BracketRow row{};
        row.year = year;
        bool valid = true;

        for (int fi = 0; fi < NUM_STATUSES; fi++)
        {
            int rateCol = CSV_RATE_START_COL + fi * CSV_COLS_PER_STATUS;
            int lowerCol = rateCol + 2;
            double rate = parsePercent(fields[rateCol]);
            double lower = parseDollar(fields[lowerCol]);

            if (rate < 0.0 || lower < 0.0)
            {
                valid = false;
                break;
            }

            int statusIndex = CSV_STATUS_ORDER[fi];
            row.rate[statusIndex] = rate;
            row.lower[statusIndex] = lower;
        }

        if (valid)
        {
            rows.push_back(row);
        }
    }

    int i = 0;
    while (i < static_cast<int>(rows.size()))
    {
        int year = rows[i].year;
        int yearIndex = yearToIndex(year);
        int j = i;

        while (j < static_cast<int>(rows.size()) && rows[j].year == year)
        {
            j++;
        }

        int bracketCount = j - i;
        for (int statusIndex = 0; statusIndex < NUM_STATUSES; statusIndex++)
        {
            numBrackets[yearIndex][statusIndex] = bracketCount;
            for (int bracket = 0; bracket < bracketCount; bracket++)
            {
                int flat = bracket * 2;
                taxBracket[yearIndex][statusIndex][flat] = rows[i + bracket].rate[statusIndex];
                taxBracket[yearIndex][statusIndex][flat + 1] =
                    bracket < bracketCount - 1 ? rows[i + bracket + 1].lower[statusIndex]
                                               : static_cast<double>(TOP_BRACKET_SENTINEL);
            }
        }

        bracketLoaded[yearIndex] = true;
        i = j;
    }

    return !rows.empty();
}
