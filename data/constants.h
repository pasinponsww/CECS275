/**
 * @file constants.h
 * @brief Compile-time constants for the Tax Data Extraction program.
 *        All magic numbers and hard-coded values live here.
 * @author Bex Sawetrattanathumrong
 * @date 2026
 */

#pragma once 

/**
* @note Constants for the tax data extraction program.  These include:
* - Supported year range and array sizes
* - Sentinel values for bracket upper bounds
* - Formatting widths for console display
* - File naming templates for output files
* - CSV column offsets for parsing the raw bracket file
* - Status label arrays for output and display
*/
const int START_YEAR = 1970;
const int END_YEAR   = 2025;
const int NUM_YEARS  = END_YEAR - START_YEAR + 1;   // 56
const int NUM_STATUSES = 4;
const int MAX_BRACKETS       = 35;
const int BRACKET_ARRAY_SIZE = MAX_BRACKETS * 2;
const int TOP_BRACKET_SENTINEL = -1;

// Display formatting widths 
const int TABLE_WIDTH          = 60;
const int COL_YEAR_WIDTH       = 6;
const int COL_SINGLE_WIDTH     = 20;
const int COL_SEPARATE_WIDTH   = 28;
const int COL_HH_WIDTH         = 18;

// File naming templates (prefix/suffix pieces, no full literals in code)
const char DEDUCTION_FILE_PREFIX[] = "tax_standard_deduction_";
const char DEDUCTION_FILE_SUFFIX[] = ".txt";
const char BRACKET_FILE_PREFIX[]   = "tax_bracket_";
const char BRACKET_FILE_SUFFIX[]   = ".txt";
const char YEAR_RANGE_SEP[]        = "_to_";

// CSV column offsets for the raw bracket file 
// Row layout: year | rateJ > lowerJ | rateP > lowerP | rateS > lowerS | rateH > lowerH | notes
// Each status group is 3 columns wide (rate, ">", lower).
const int CSV_RATE_START_COL  = 1;   // first rate column index
const int CSV_COLS_PER_STATUS = 3;   // columns per filing-status group
const int CSV_MIN_FIELDS      = 13;  // minimum fields required for a valid data row

// Order of statuses as they appear in the raw CSV file (J, P, S, H) 
// Maps CSV column group index → internal status index
const int CSV_STATUS_ORDER[NUM_STATUSES] = {3, 1, 0, 2}; // J→3, P→1, S→0, H→2

// Status labels used in bracket output files 
const char STATUS_LABELS[NUM_STATUSES] = {'S', 'P', 'H', 'J'};

// Human-readable status names for console display 
const char* const STATUS_NAMES[NUM_STATUSES] = 
{
    "Single",
    "Married Filing Separately",
    "Head of Household",
    "Married Filing Jointly"
};

