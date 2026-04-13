/**
 * @file tax_app_gui.cpp
 * @brief Entry point for the GTK tax study application.
 */

#include "gui_data.h"
#include "login_dialog.h"
#include "main_window.h"

#include <gtk/gtk.h>

#include <string>

int main(int argc, char* argv[])
{
    gtk_init(&argc, &argv);

    GuiData::load_all();
    if (!GuiData::is_loaded())
    {
        GtkWidget* message = gtk_message_dialog_new(
            nullptr, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
            "Failed to load tax data files.\n"
            "Please check data/tax_deduction.txt and data/tax_rate_and_bracket.txt");

        gtk_dialog_run(GTK_DIALOG(message));
        gtk_widget_destroy(message);
        return 1;
    }

    std::string username;
    std::string avatarPath;
    LoginDialog login(nullptr);

    if (!login.run(username, avatarPath))
    {
        return 0;
    }

    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    std::string title = "US Tax Study App - User: " + username;
    gtk_window_set_title(GTK_WINDOW(window), title.c_str());
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 700);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), nullptr);

    MainWindow mainWindow(username, avatarPath);
    gtk_container_add(GTK_CONTAINER(window), mainWindow.widget());

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
