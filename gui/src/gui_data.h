/**
 * @file gui_data.h
 * @brief GUI-facing tax data loader helpers.
 *
 * @details This small module keeps file-path lookup and data initialization out
 * of the window code. The GUI should ask this layer to load data, then read the
 * shared arrays through common/core and calculate through common/math or
 * common/virtual.
 */

#pragma once

namespace GuiData
{

/**
 * @brief Initializes shared storage and loads deduction/bracket files.
 *
 * @note The loader supports launching from either the project root or the
 * `gui/` directory.
 */
void load_all();

/**
 * @brief Checks whether the deduction data was loaded successfully.
 *
 * @return true when the standard deduction data is available.
 */
bool is_loaded();

} // namespace GuiData
