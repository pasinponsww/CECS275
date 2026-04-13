# common/core

This folder contains the core utility layer for the tax study app.

The goal of this layer is to manage the shared tax data and the helper
operations that deal with files and loaded arrays. It sits
between the app interface and the pure math layer.

In simple terms, this folder answers: "How do we load and store the tax
data the app needs?"

## Files

- `utils.h`
  Declares shared tax data arrays and utility functions.

- `utils.cpp`
  Defines the shared arrays and implements file loading.

## Responsibilities

This module is responsible for:

- storing loaded standard deduction data
- storing loaded tax bracket data
- tracking whether deduction or bracket data has been loaded
- reading the deduction data file
- reading the tax bracket data file
- initializing shared tax storage
- providing loaded data that the GUI and tax calculators can read

## Shared Data

The project stores tax data in shared arrays declared in `utils.h` and defined in
`utils.cpp`:

```cpp
taxDeduction[NUM_YEARS][NUM_STATUSES]
taxBracket[NUM_YEARS][NUM_STATUSES][BRACKET_ARRAY_SIZE]
numBrackets[NUM_YEARS][NUM_STATUSES]
bracketLoaded[NUM_YEARS]
deductionLoaded
```

These arrays are the bridge between file input and tax calculation.

## Relationship To common/math

`common/core` uses `common/math` helpers when it needs parsing or validation.

For example:

- `parseDollar(...)` cleans dollar values from the raw file
- `parsePercent(...)` cleans tax rate values
- `yearToIndex(...)` maps a tax year into the storage array
- `statusToIndex(...)` maps filing status labels into the storage array

The core layer should not duplicate this math logic. It should use the math
helpers instead.

## Relationship To gui/src

`gui/src` controls the user flow for Parts A, B, and C.

The GUI layer calls into `common/core` when it needs tax data loaded.
This keeps the app interface cleaner because the GUI does not need to know the
details of parsing the raw IRS-style files.

The structure is:

```text
gui/src       -> login, avatar, Part B calculator, Part C simulation flow
common/core   -> load/store tax data
common/math   -> pure tax calculations and parsing helpers
```

## What Belongs Here

Put code here when it deals with tax data as program resources.

Good examples:

- loading deduction data from a file
- loading bracket data from a file
- initializing or checking shared arrays
- resetting loaded data before a new load

## What Does Not Belong Here

Do not put pure tax formulas here if they can live in `common/math`.

Also avoid putting app-specific user flow here, such as:

- login behavior
- avatar handling
- Part B/Part C menu loops
- fake profile generation
- chart selection flow

Those belong in `gui/src`.

## Why This Separation Matters

This separation keeps the interface organized:

- `common/math` knows how to calculate.
- `common/core` knows how to load and expose data.
- `gui/src` knows how to guide the user through the required features.

That way, changing the user interface does not require rewriting the tax math,
and changing the tax math does not require rewriting the menus.
