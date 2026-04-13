/**
 * @file line_chart.h
 * @brief GTK helper for displaying line charts for statistics over years.
 */

#pragma once

#include <gtk/gtk.h>
#include <vector>
#include <string>

/**
 * @brief Creates a GTK line chart widget.
 *
 * @param years X-axis values (years).
 * @param avg_income Y-values for average income.
 * @param med_income Y-values for median income.
 * @param avg_rate Y-values for average effective tax rate.
 * @return GTK widget containing the chart drawing area.
 */
GtkWidget* create_line_chart(const std::vector<int>& years, const std::vector<double>& avg_income,
                             const std::vector<double>& med_income,
                             const std::vector<double>& avg_rate);
