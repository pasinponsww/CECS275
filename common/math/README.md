# common/math

This folder contains the math helper layer for the tax study app.

The goal of this layer is to keep tax calculations, parsing helpers, and index
conversion logic separate from user input, menus, file display, and interface
code. In other words, this folder answers: "Given clean data, how do we convert
or calculate it?"

## Files

- `taxmath.h`
  Declares the math helper API used by the rest of the project.

- `taxmath.cpp`
  Implements the helper functions declared in `taxmath.h`.

## Responsibilities

This module is responsible for:

- converting filing status codes like `S`, `P`, `H`, and `J` into internal indexes
- converting indexes back into filing status labels
- parsing dollar strings such as `$23,850`
- parsing percent strings such as `22.0%`
- validating tax years and filing statuses
- converting a tax year into an array index
- calculating taxable income after standard deduction
- calculating progressive federal tax from loaded bracket data

## What Belongs Here

Put code here when it is pure helper logic and does not need to talk directly to
the user.

Good examples:

- `parseDollar("$15,750")`
- `yearToIndex(2025)`
- `standardDeductionFor(...)`
- `progressiveTaxFor(...)`

## What Does Not Belong Here

Do not put display, menu, login, file output, or user prompt code in this folder.

These do not belong here:

- `cout` menu screens
- `cin` input prompts
- login/avatar flow
- chart display
- file loading messages

Those belong in the GTK GUI layer under `gui/src`.

## Why This Separation Matters

Keeping math separate makes the program easier to test and easier to reuse.

For example, Part B and Part C both need tax calculations. They should not each
rewrite bracket math. Instead, they both call the same math helpers from this
folder.

The structure is:

```text
gui/src       -> GTK screens, callbacks, charts, and user flow
common/core   -> file loading and shared tax arrays
common/math   -> pure parsing, validation, and tax calculations
```

This keeps the interface cleaner because the app can ask for a tax result
without needing to know every detail about bracket arrays or deduction indexes.
