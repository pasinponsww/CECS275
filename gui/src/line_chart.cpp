/**
 * @file line_chart.cpp
 * @brief Implements a simple GTK/Cairo line chart widget for statistics.
 */

#include "line_chart.h"
#include <cairo.h>
#include <algorithm>
#include <string>
#include <vector>
#include <sstream>

struct LineChartData
{
    std::vector<int> years;
    std::vector<double> avg_income;
    std::vector<double> med_income;
    std::vector<double> avg_rate;
};

static void draw_line(cairo_t* cr, const std::vector<double>& data, double minY, double maxY,
                      double x0, double y0, double x1, double y1, double colorR, double colorG,
                      double colorB, int n)
{
    cairo_set_source_rgb(cr, colorR, colorG, colorB);
    cairo_set_line_width(cr, 2.0);
    for (int i = 0; i < n - 1; ++i)
    {
        double xA = x0 + (x1 - x0) * i / (n - 1);
        double xB = x0 + (x1 - x0) * (i + 1) / (n - 1);
        double yA = y1 - (data[i] - minY) / (maxY - minY) * (y1 - y0);
        double yB = y1 - (data[i + 1] - minY) / (maxY - minY) * (y1 - y0);
        cairo_move_to(cr, xA, yA);
        cairo_line_to(cr, xB, yB);
        cairo_stroke(cr);
    }
}

static gboolean on_draw_line_chart(GtkWidget* widget, cairo_t* cr, gpointer data)
{
    LineChartData* chart = static_cast<LineChartData*>(data);
    int n = chart->years.size();
    if (n < 2)
        return FALSE;
    double x0 = 60, y0 = 30, x1 = 540, y1 = 320;
    // Find min/max for incomes
    double minIncome = *std::min_element(chart->med_income.begin(), chart->med_income.end());
    double maxIncome = *std::max_element(chart->avg_income.begin(), chart->avg_income.end());
    double minRate = 0.0;
    double maxRate = *std::max_element(chart->avg_rate.begin(), chart->avg_rate.end());
    // Draw axes
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 1.5);
    cairo_move_to(cr, x0, y0);
    cairo_line_to(cr, x0, y1);
    cairo_line_to(cr, x1, y1);
    cairo_stroke(cr);
    // Draw income lines
    draw_line(cr, chart->avg_income, minIncome, maxIncome, x0, y0, x1, y1, 0.2, 0.2, 0.8, n);
    draw_line(cr, chart->med_income, minIncome, maxIncome, x0, y0, x1, y1, 0.2, 0.8, 0.2, n);
    // Draw rate line (scaled to right Y axis)
    double rateX0 = x1 + 10, rateX1 = x1 + 40;
    draw_line(cr, chart->avg_rate, minRate, maxRate, x0, y0, x1, y1, 0.8, 0.2, 0.2, n);
    // Draw labels
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 13);
    cairo_move_to(cr, x0, y0 - 10);
    cairo_show_text(cr, "Income");
    cairo_move_to(cr, x1 + 10, y1);
    cairo_show_text(cr, "Year");
    cairo_move_to(cr, x1 - 120, y0 + 10);
    cairo_set_source_rgb(cr, 0.2, 0.2, 0.8);
    cairo_show_text(cr, "Avg Income");
    cairo_move_to(cr, x1 - 120, y0 + 30);
    cairo_set_source_rgb(cr, 0.2, 0.8, 0.2);
    cairo_show_text(cr, "Med Income");
    cairo_move_to(cr, x1 - 120, y0 + 50);
    cairo_set_source_rgb(cr, 0.8, 0.2, 0.2);
    cairo_show_text(cr, "Avg Eff. Rate");
    // Draw year ticks
    cairo_set_source_rgb(cr, 0, 0, 0);
    for (int i = 0; i < n; i += 5)
    {
        double x = x0 + (x1 - x0) * i / (n - 1);
        cairo_move_to(cr, x, y1);
        cairo_line_to(cr, x, y1 + 8);
        cairo_stroke(cr);
        std::ostringstream oss;
        oss << chart->years[i];
        cairo_move_to(cr, x - 10, y1 + 22);
        cairo_show_text(cr, oss.str().c_str());
    }
    return FALSE;
}

GtkWidget* create_line_chart(const std::vector<int>& years, const std::vector<double>& avg_income,
                             const std::vector<double>& med_income,
                             const std::vector<double>& avg_rate)
{
    GtkWidget* drawingArea = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawingArea, 600, 360);
    LineChartData* chart = new LineChartData{years, avg_income, med_income, avg_rate};
    g_signal_connect(drawingArea, "draw", G_CALLBACK(on_draw_line_chart), chart);
    return drawingArea;
}
