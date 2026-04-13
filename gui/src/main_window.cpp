// Project includes
#include "main_window.h"
#include "../../common/math/taxmath.h"
#include "../../common/core/utils.h"
#include "../../common/virtual/tax.h"
#include "pie_chart.h"

// System/GTK includes
#include <gtk/gtk.h>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <random>
#include <sstream>
#include <vector>

namespace
{

void addClass(GtkWidget* widget, const char* className)
{
    GtkStyleContext* context = gtk_widget_get_style_context(widget);
    gtk_style_context_add_class(context, className);
}

GtkWidget* makeLabel(const std::string& text, const char* className)
{
    GtkWidget* label = gtk_label_new(text.c_str());
    gtk_label_set_xalign(GTK_LABEL(label), 0.0);
    addClass(label, className);
    return label;
}

GtkWidget* makePanel(GtkOrientation orientation, int spacing)
{
    GtkWidget* panel = gtk_box_new(orientation, spacing);
    addClass(panel, "panel");
    return panel;
}

GtkWidget* makeAvatar(const std::string& avatarPath)
{
    GtkWidget* frame = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    addClass(frame, "avatar-frame");

    GtkWidget* avatar = nullptr;
    if (!avatarPath.empty())
    {
        GError* error = nullptr;
        GdkPixbuf* pixbuf =
            gdk_pixbuf_new_from_file_at_scale(avatarPath.c_str(), 180, 180, TRUE, &error);

        if (pixbuf)
        {
            avatar = gtk_image_new_from_pixbuf(pixbuf);
            g_object_unref(pixbuf);
        }
        else
        {
            avatar = gtk_label_new("Avatar not found");
            if (error)
            {
                g_error_free(error);
            }
        }
    }
    else
    {
        avatar = gtk_label_new("No avatar selected");
    }

    gtk_box_pack_start(GTK_BOX(frame), avatar, FALSE, FALSE, 0);
    return frame;
}

} // namespace

void update_bracket_table(GtkWidget* table, int year, int statusIdx)
{
    // Remove all children
    GList* children = gtk_container_get_children(GTK_CONTAINER(table));
    for (GList* iter = children; iter != NULL; iter = iter->next)
    {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);

    // Header
    GtkWidget* rateHeader = makeLabel("Rate", "table-header");
    GtkWidget* fromHeader = makeLabel("From", "table-header");
    GtkWidget* upToHeader = makeLabel("Up To", "table-header");
    gtk_grid_attach(GTK_GRID(table), rateHeader, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(table), fromHeader, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(table), upToHeader, 2, 0, 1, 1);

    double lower = 0.0;
    int yi = Tax::yearToIndex(year);
    for (int b = 0; b < numBrackets[yi][statusIdx]; b++)
    {
        double rate = taxBracket[yi][statusIdx][b * 2];
        double upper = taxBracket[yi][statusIdx][b * 2 + 1];
        std::ostringstream rateStr;
        rateStr << rate << "%";
        std::ostringstream fromStr;
        fromStr << "$" << lower;
        std::ostringstream toStr;
        if (upper < 0.0)
        {
            toStr << "And up";
        }
        else
        {
            toStr << "$" << upper;
        }
        gtk_grid_attach(GTK_GRID(table), makeLabel(rateStr.str(), "table-cell"), 0, b + 1, 1, 1);
        gtk_grid_attach(GTK_GRID(table), makeLabel(fromStr.str(), "table-cell"), 1, b + 1, 1, 1);
        gtk_grid_attach(GTK_GRID(table), makeLabel(toStr.str(), "table-cell"), 2, b + 1, 1, 1);
        if (upper > 0.0)
        {
            lower = upper;
        }
    }
    gtk_widget_show_all(table);
}

// --- GTK callback helpers (must be outside any function/class) ---
struct UpdateUIData
{
    GtkWidget* year_spin;
    GtkWidget* status_combo;
    GtkWidget* deduction_label;
    GtkWidget* bracket_table;
};
static void update_ui_cb(GtkWidget*, gpointer user_data)
{
    UpdateUIData* d = (UpdateUIData*)user_data;
    int year = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(d->year_spin));
    int statusIdx = gtk_combo_box_get_active(GTK_COMBO_BOX(d->status_combo));
    Tax::FilingStatus status = static_cast<Tax::FilingStatus>(statusIdx);
    Tax::StandardDeductionCalculator deductionCalculator;
    double deduction = deductionCalculator.calculateTax(0.0, year, status);
    std::ostringstream oss;
    oss << "Standard Deduction: $" << deduction;
    gtk_label_set_text(GTK_LABEL(d->deduction_label), oss.str().c_str());
    update_bracket_table(d->bracket_table, year, statusIdx);
}

struct CalcWidgets
{
    GtkWidget* year_spin;
    GtkWidget* status_combo;
    GtkWidget* income_entry;
    GtkWidget* result_label;
};
static void on_calc_btn_clicked(GtkButton*, gpointer user_data)
{
    CalcWidgets* w = (CalcWidgets*)user_data;
    int year = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(w->year_spin));
    int statusIdx = gtk_combo_box_get_active(GTK_COMBO_BOX(w->status_combo));
    double income = atof(gtk_entry_get_text(GTK_ENTRY(w->income_entry)));
    Tax::FilingStatus status = static_cast<Tax::FilingStatus>(statusIdx);
    Tax::TaxData data(income, year, status);
    Tax::TaxableIncomeCalculator taxableCalculator;
    Tax::BracketTaxCalculator taxCalculator;
    double taxable = taxableCalculator.calculateTax(data);
    double tax = taxCalculator.calculateTax(data);
    double effRate = (income > 0.0) ? (tax / income) * 100.0 : 0.0;
    char buf[256];
    snprintf(buf, sizeof(buf),
             "Taxable Income: $%.2f\nEstimated Tax: $%.2f\nEffective Rate: %.2f%%", taxable, tax,
             effRate);
    gtk_label_set_text(GTK_LABEL(w->result_label), buf);
}

struct RandWidgets
{
    GtkWidget* year_spin;
    GtkWidget* status_combo;
    GtkWidget* income_entry;
};
static void on_random_btn_clicked(GtkButton*, gpointer user_data)
{
    RandWidgets* w = (RandWidgets*)user_data;
    double incomes[] = {8000, 20000, 40000, 60000, 90000, 150000, 300000};
    int idx = rand() % 7;
    gtk_entry_set_text(GTK_ENTRY(w->income_entry), std::to_string(incomes[idx]).c_str());
    gtk_combo_box_set_active(GTK_COMBO_BOX(w->status_combo), rand() % NUM_STATUSES);
}

struct PieWidgets
{
    GtkWidget* pie_area;
    GtkWidget* line_label;
    GtkWidget* summary_label;
};
static void on_generate_profiles_clicked(GtkButton*, gpointer user_data)
{
    PieWidgets* w = (PieWidgets*)user_data;
    // Income ranges used for the fake profile sample.
    struct IncomeRange
    {
        const char* label;
        double minIncome, maxIncome;
        int weight;
    } ranges[] = {{"Under $10k", 0.0, 9999.0, 14},        {"$10k-$25k", 10000.0, 24999.0, 18},
                  {"$25k-$50k", 25000.0, 49999.0, 22},    {"$50k-$75k", 50000.0, 74999.0, 16},
                  {"$75k-$100k", 75000.0, 99999.0, 11},   {"$100k-$200k", 100000.0, 199999.0, 13},
                  {"$200k and up", 200000.0, 650000.0, 6}};
    int SAMPLE_SIZE = 1000;
    std::vector<int> counts(7, 0);
    std::vector<double> avg_income(51, 0.0), med_income(51, 0.0), avg_rate(51, 0.0);
    std::vector<std::vector<double>> all_incomes(51);
    std::vector<std::vector<double>> all_rates(51);
    std::mt19937 rng((unsigned)time(NULL));
    std::discrete_distribution<int> rangePicker{14, 18, 22, 16, 11, 13, 6};
    std::uniform_int_distribution<int> statusPicker(0, 3);
    Tax::BracketTaxCalculator taxCalculator;
    for (int i = 0; i < SAMPLE_SIZE; ++i)
    {
        int r = rangePicker(rng);
        double minI = ranges[r].minIncome, maxI = ranges[r].maxIncome;
        std::uniform_real_distribution<double> incomePicker(minI, maxI);
        double baseIncome2025 = incomePicker(rng);
        int status = statusPicker(rng);
        counts[r]++;
        // For each year 1975-2025
        for (int y = 0; y <= 50; ++y)
        {
            int year = 1975 + y;
            double scale = (year < 1980)   ? 0.28
                           : (year < 1990) ? 0.38
                           : (year < 2000) ? 0.55
                           : (year < 2010) ? 0.72
                           : (year < 2020) ? 0.88
                                           : 1.0;
            double income = baseIncome2025 * scale;
            Tax::TaxData data(income, year, static_cast<Tax::FilingStatus>(status));
            double tax = taxCalculator.calculateTax(data);
            double effectiveRate = income > 0.0 ? (tax / income) * 100.0 : 0.0;
            all_incomes[y].push_back(income);
            all_rates[y].push_back(effectiveRate);
        }
    }
    // Pie chart
    std::vector<double> percentages;
    for (int c : counts)
    {
        percentages.push_back(100.0 * c / SAMPLE_SIZE);
    }

    std::vector<std::string> labels;
    for (auto& r : ranges)
    {
        labels.push_back(r.label);
    }
    GtkWidget* chart = create_pie_chart(percentages, labels);
    // Remove old chart
    GList* children = gtk_container_get_children(GTK_CONTAINER(w->pie_area));
    for (GList* iter = children; iter != NULL; iter = iter->next)
    {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);
    gtk_box_pack_start(GTK_BOX(w->pie_area), chart, FALSE, FALSE, 0);
    gtk_widget_show_all(w->pie_area);

    // Calculate stats
    std::ostringstream oss;
    for (int y = 0; y <= 50; ++y)
    {
        auto& incomes = all_incomes[y];
        std::sort(incomes.begin(), incomes.end());
        double sum = 0.0;
        for (double v : incomes)
        {
            sum += v;
        }

        double rateSum = 0.0;
        for (double v : all_rates[y])
        {
            rateSum += v;
        }
        avg_income[y] = sum / incomes.size();
        med_income[y] = incomes[incomes.size() / 2];
        avg_rate[y] = rateSum / all_rates[y].size();
    }
    oss << "Year  Income (avg/med)  Avg rate\n";
    for (int y = 0; y <= 50; y += 5)
    {
        oss << (1975 + y) << ": $" << (int)avg_income[y] << "/$" << (int)med_income[y] << "  "
            << avg_rate[y] << "%\n";
    }
    gtk_label_set_text(GTK_LABEL(w->summary_label), oss.str().c_str());
    gtk_label_set_text(GTK_LABEL(w->line_label), "Line chart data generated in the summary below.");
}

GtkWidget* MainWindow::build_part_a(const std::string& username, const std::string& avatarPath)
{
    GtkWidget* page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 18);
    addClass(page, "page");

    GtkWidget* hero = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 24);
    addClass(hero, "hero");

    GtkWidget* copy = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    std::ostringstream title;
    title << "Welcome, " << username;

    gtk_box_pack_start(GTK_BOX(copy), makeLabel(title.str(), "title"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(copy),
                       makeLabel("Your tax workspace is ready. Use the calculator and "
                                 "simulation tabs to explore federal tax data.",
                                 "subtitle"),
                       FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(copy), makeLabel("Login complete", "metric"), FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(hero), copy, TRUE, TRUE, 0);
    gtk_box_pack_end(GTK_BOX(hero), makeAvatar(avatarPath), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(page), hero, FALSE, FALSE, 0);

    return page;
}

GtkWidget* MainWindow::build_part_b()
{
    GtkWidget* page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    addClass(page, "page");

    gtk_box_pack_start(GTK_BOX(page), makeLabel("Tax Calculator", "title"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(page),
                       makeLabel("Select a year, review the bracket table, then calculate "
                                 "an estimated federal tax.",
                                 "subtitle"),
                       FALSE, FALSE, 0);

    GtkWidget* box = makePanel(GTK_ORIENTATION_VERTICAL, 12);

    // Year selection
    GtkWidget* year_label = gtk_label_new("Tax Year:");
    GtkWidget* year_spin = gtk_spin_button_new_with_range(1975, 2025, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(year_spin), 2025);

    // Status selection
    GtkWidget* status_label = gtk_label_new("Filing Status:");
    GtkWidget* status_combo = gtk_combo_box_text_new();
    for (int i = 0; i < NUM_STATUSES; ++i)
    {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(status_combo), STATUS_NAMES[i]);
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(status_combo), 0);

    // Income entry
    GtkWidget* income_label = gtk_label_new("Income ($):");
    GtkWidget* income_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(income_entry), "50000");

    // Deduction display
    GtkWidget* deduction_label = gtk_label_new("");
    addClass(deduction_label, "metric");

    // Bracket table
    GtkWidget* bracket_table = gtk_grid_new();
    addClass(bracket_table, "data-table");
    gtk_grid_set_row_spacing(GTK_GRID(bracket_table), 8);
    gtk_grid_set_column_spacing(GTK_GRID(bracket_table), 24);
    GtkWidget* bracket_scroll = gtk_scrolled_window_new(nullptr, nullptr);
    gtk_widget_set_size_request(bracket_scroll, -1, 330);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(bracket_scroll), GTK_POLICY_AUTOMATIC,
                                   GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(bracket_scroll), bracket_table);

    // Calculate button
    GtkWidget* calc_btn = gtk_button_new_with_label("Calculate Tax");
    GtkWidget* random_btn = gtk_button_new_with_label("Random Profile");

    // Result area
    GtkWidget* result_label =
        gtk_label_new("Enter an income, then calculate or generate a random profile.");
    addClass(result_label, "result");
    gtk_label_set_xalign(GTK_LABEL(result_label), 0.0);

    // Layout
    GtkWidget* row1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_pack_start(GTK_BOX(row1), year_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(row1), year_spin, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(row1), status_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(row1), status_combo, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(row1), income_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(row1), income_entry, FALSE, FALSE, 0);

    GtkWidget* row2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_pack_start(GTK_BOX(row2), calc_btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(row2), random_btn, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(box), row1, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), deduction_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), bracket_scroll, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), row2, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), result_label, FALSE, FALSE, 0);

    UpdateUIData* update_data =
        new UpdateUIData{year_spin, status_combo, deduction_label, bracket_table};
    g_signal_connect(year_spin, "value-changed", G_CALLBACK(update_ui_cb), update_data);
    g_signal_connect(status_combo, "changed", G_CALLBACK(update_ui_cb), update_data);
    update_ui_cb(nullptr, update_data);

    CalcWidgets* calc_widgets =
        new CalcWidgets{year_spin, status_combo, income_entry, result_label};
    g_signal_connect(calc_btn, "clicked", G_CALLBACK(on_calc_btn_clicked), calc_widgets);

    RandWidgets* rand_widgets = new RandWidgets{year_spin, status_combo, income_entry};
    g_signal_connect(random_btn, "clicked", G_CALLBACK(on_random_btn_clicked), rand_widgets);

    gtk_box_pack_start(GTK_BOX(page), box, FALSE, FALSE, 0);
    return page;
}

GtkWidget* MainWindow::build_part_c()
{
    GtkWidget* page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    addClass(page, "page");

    gtk_box_pack_start(GTK_BOX(page), makeLabel("Simulation", "title"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(page),
                       makeLabel("Generate 1000 fake profiles and summarize income ranges, "
                                 "income averages, and effective tax rates.",
                                 "subtitle"),
                       FALSE, FALSE, 0);

    GtkWidget* box = makePanel(GTK_ORIENTATION_VERTICAL, 12);
    GtkWidget* btn = gtk_button_new_with_label("Generate 1000 Fake Profiles");
    GtkWidget* pie_area = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    GtkWidget* line_label = gtk_label_new("No simulation generated yet.");
    GtkWidget* summary_label = gtk_label_new("Click the button to create the sample and chart.");
    addClass(line_label, "metric");
    addClass(summary_label, "result");
    gtk_label_set_xalign(GTK_LABEL(summary_label), 0.0);

    gtk_box_pack_start(GTK_BOX(box), btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), pie_area, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), line_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), summary_label, FALSE, FALSE, 0);

    PieWidgets* pie_widgets = new PieWidgets{pie_area, line_label, summary_label};
    g_signal_connect(btn, "clicked", G_CALLBACK(on_generate_profiles_clicked), pie_widgets);

    gtk_box_pack_start(GTK_BOX(page), box, FALSE, FALSE, 0);
    return page;
}

MainWindow::MainWindow(const std::string& username, const std::string& avatarPath)
{
    notebook = gtk_notebook_new();
    addClass(notebook, "app-notebook");
    GtkWidget* tab_a = build_part_a(username, avatarPath);
    GtkWidget* tab_b = build_part_b();
    GtkWidget* tab_c = build_part_c();
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), tab_a, gtk_label_new("Login/Avatar"));
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), tab_b, gtk_label_new("Tax Calculator"));
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), tab_c, gtk_label_new("Simulation"));
}

GtkWidget* MainWindow::widget()
{
    return notebook;
}
