/**
 * @file pie_chart.cpp
 * @brief Implements a simple GTK/Cairo pie chart widget.
 */

#include "pie_chart.h"

#include <cairo.h>
#include <cmath>

struct PieChartData
{
    std::vector<double> percentages;
    std::vector<std::string> labels;
};

static gboolean on_draw(GtkWidget* widget, cairo_t* cr, gpointer data)
{
    (void)widget;

    PieChartData* chart = static_cast<PieChartData*>(data);
    double cx = 150.0;
    double cy = 150.0;
    double radius = 100.0;
    double angle = 0.0;

    double colors[][3] = {{0.8, 0.2, 0.2}, {0.2, 0.8, 0.2}, {0.2, 0.2, 0.8}, {0.8, 0.8, 0.2},
                          {0.2, 0.8, 0.8}, {0.8, 0.2, 0.8}, {0.5, 0.5, 0.5}};

    for (size_t i = 0; i < chart->percentages.size(); ++i)
    {
        double fraction = chart->percentages[i] / 100.0;
        double nextAngle = angle + fraction * 2.0 * M_PI;

        cairo_set_source_rgb(cr, colors[i % 7][0], colors[i % 7][1], colors[i % 7][2]);
        cairo_move_to(cr, cx, cy);
        cairo_arc(cr, cx, cy, radius, angle, nextAngle);
        cairo_close_path(cr);
        cairo_fill(cr);

        angle = nextAngle;
    }

    angle = 0.0;
    for (size_t i = 0; i < chart->percentages.size(); ++i)
    {
        double fraction = chart->percentages[i] / 100.0;
        double middleAngle = angle + fraction * M_PI;
        double labelX = cx + cos(middleAngle) * (radius + 30.0);
        double labelY = cy + sin(middleAngle) * (radius + 30.0);

        cairo_move_to(cr, labelX, labelY);
        cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
        cairo_show_text(cr, chart->labels[i].c_str());

        angle += fraction * 2.0 * M_PI;
    }

    return FALSE;
}

GtkWidget* create_pie_chart(const std::vector<double>& percentages,
                            const std::vector<std::string>& labels)
{
    GtkWidget* drawingArea = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawingArea, 320, 320);

    PieChartData* chart = new PieChartData{percentages, labels};
    g_signal_connect(drawingArea, "draw", G_CALLBACK(on_draw), chart);

    return drawingArea;
}
