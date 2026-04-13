/**
 * @file pie_chart.h
 * @brief GTK helper for displaying income-range percentages as a pie chart.
 */

#pragma once

#include <gtk/gtk.h>
#include <string>
#include <vector>

/**
 * @brief Creates a GTK pie chart widget.
 *
 * @param percentages Slice percentages. Values are expected to total near 100.
 * @param labels Human-readable labels matching the percentage vector.
 * @return GTK widget containing the chart drawing area.
 */
GtkWidget* create_pie_chart(const std::vector<double>& percentages,
                            const std::vector<std::string>& labels);
