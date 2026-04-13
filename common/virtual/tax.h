/**
 * @file tax.h
 * @brief Tax profile model and calculator interface.
 *
 * @details This header demonstrates two OOP concepts used by the GUI:
 * virtual functions and function overloading. GUI code can depend on the
 * `TaxCalculator` base class while concrete calculators decide which tax value
 * to return.
 */

#pragma once
#include "../core/utils.h"
#include "../math/taxmath.h"

namespace Tax
{

/**
 * @class TaxData
 * @brief Small value object for one taxpayer calculation request.
 */
class TaxData
{
  public:
    /**
     * @brief Compatibility alias for the shared filing status enum.
     */
    using FilingStatus = Tax::FilingStatus;

    /**
     * @brief Creates a tax profile using a type-safe filing status.
     * @param income Gross income before standard deduction.
     * @param year Tax year.
     * @param filingStatus Filing status enum value.
     */
    TaxData(double income = 0.0, int year = END_YEAR,
            FilingStatus filingStatus = FilingStatus::Single)
        : income(income), year(year), filingStatus(filingStatus)
    {
    }

    /**
     * @brief Creates a tax profile using an S/P/H/J filing status code.
     * @param income Gross income before standard deduction.
     * @param year Tax year.
     * @param filingStatus Filing status character.
     *
     * @note This is function overloading: callers may pass either a
     * `FilingStatus` enum or a simple status character.
     */
    TaxData(double income, int year, char filingStatus)
        : income(income), year(year), filingStatus(charToFilingStatus(filingStatus))
    {
    }

    /** @brief Gets gross income. */
    double getIncome() const
    {
        return income;
    }

    /** @brief Gets tax year. */
    int getYear() const
    {
        return year;
    }

    /** @brief Gets filing status enum. */
    FilingStatus getFilingStatus() const
    {
        return filingStatus;
    }

    /** @brief Gets S/P/H/J filing status code. */
    char getFilingStatusCode() const
    {
        return filingStatusToChar(filingStatus);
    }

    /** @brief Sets gross income. */
    void setIncome(double newIncome)
    {
        income = newIncome;
    }

    /** @brief Sets tax year. */
    void setYear(int newYear)
    {
        year = newYear;
    }

    /** @brief Sets filing status from an enum value. */
    void setFilingStatus(FilingStatus newFilingStatus)
    {
        filingStatus = newFilingStatus;
    }

    /** @brief Sets filing status from an S/P/H/J character. */
    void setFilingStatus(char newFilingStatus)
    {
        filingStatus = charToFilingStatus(newFilingStatus);
    }

  private:
    double income;
    int year;
    FilingStatus filingStatus;
};

/**
 * @class TaxCalculator
 * @brief Virtual base class for all tax calculators.
 */
class TaxCalculator
{
  public:
    /**
     * @brief Calculates a tax-related value from a complete tax profile.
     * @param data Taxpayer profile.
     * @return Calculator-specific result.
     *
     * @note This is the virtual function used by derived calculators.
     */
    virtual double calculateTax(const TaxData& data) const = 0;

    /**
     * @brief Overload for callers with a type-safe filing status.
     * @param income Gross income before standard deduction.
     * @param year Tax year.
     * @param filingStatus Filing status enum value.
     * @return Calculator-specific result.
     */
    double calculateTax(double income, int year, FilingStatus filingStatus) const
    {
        return calculateTax(TaxData(income, year, filingStatus));
    }

    /**
     * @brief Overload for callers with an S/P/H/J filing status code.
     * @param income Gross income before standard deduction.
     * @param year Tax year.
     * @param filingStatus Filing status character.
     * @return Calculator-specific result.
     */
    double calculateTax(double income, int year, char filingStatus) const
    {
        return calculateTax(TaxData(income, year, filingStatus));
    }

    /** @brief Ensures safe cleanup through base-class pointers. */
    virtual ~TaxCalculator() = default;
};

/**
 * @class StandardDeductionCalculator
 * @brief Returns the loaded standard deduction for a profile.
 */
class StandardDeductionCalculator : public TaxCalculator
{
  public:
    /**
     * @brief Inherit the overload calculateTax functions from the base class
     */
    using TaxCalculator::calculateTax;

    // Override the main virtual function to return the standard deduction for the given profile.
    double calculateTax(const TaxData& data) const override
    {
        return standardDeductionFor(data.getYear(), data.getFilingStatus(), taxDeduction);
    }
};

/**
 * @class TaxableIncomeCalculator
 * @brief Returns income after applying the loaded standard deduction.
 */
class TaxableIncomeCalculator : public TaxCalculator
{
  public:
    using TaxCalculator::calculateTax;

    double calculateTax(const TaxData& data) const override
    {
        double deduction =
            standardDeductionFor(data.getYear(), data.getFilingStatus(), taxDeduction);
        return taxableIncomeAfterDeduction(data.getIncome(), deduction);
    }
};

/**
 * @class BracketTaxCalculator
 * @brief Returns estimated progressive federal tax from loaded bracket data.
 */
class BracketTaxCalculator : public TaxCalculator
{
  public:
    using TaxCalculator::calculateTax;

    double calculateTax(const TaxData& data) const override
    {
        double deduction =
            standardDeductionFor(data.getYear(), data.getFilingStatus(), taxDeduction);
        double taxableIncome = taxableIncomeAfterDeduction(data.getIncome(), deduction);

        return progressiveTaxFor(taxableIncome, data.getYear(), data.getFilingStatus(), taxBracket,
                                 numBrackets);
    }
};

} // namespace Tax
