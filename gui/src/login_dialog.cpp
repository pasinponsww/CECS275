/**
 * @file login_dialog.cpp
 * @brief Implements the GTK login and avatar picker dialog.
 */

#include "login_dialog.h"

#include <string>

void LoginDialog::on_avatar_clicked(GtkButton* button, gpointer user_data)
{
    (void)button;

    LoginDialog* dialog = static_cast<LoginDialog*>(user_data);
    GtkWidget* chooser = gtk_file_chooser_dialog_new(
        "Select Avatar", nullptr, GTK_FILE_CHOOSER_ACTION_OPEN, "_Cancel", GTK_RESPONSE_CANCEL,
        "_Open", GTK_RESPONSE_ACCEPT, nullptr);

    if (gtk_dialog_run(GTK_DIALOG(chooser)) == GTK_RESPONSE_ACCEPT)
    {
        char* filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(chooser));
        dialog->avatar_path = filename;
        gtk_label_set_text(GTK_LABEL(dialog->avatar_label), filename);
        g_free(filename);
    }

    gtk_widget_destroy(chooser);
}

LoginDialog::LoginDialog(GtkWindow* parent)
    : dialog(nullptr), entry_user(nullptr), entry_pass(nullptr), avatar_label(nullptr),
      avatar_button(nullptr), success(false)
{
    dialog = gtk_dialog_new_with_buttons("Login", parent, GTK_DIALOG_MODAL, "_OK", GTK_RESPONSE_OK,
                                         "_Cancel", GTK_RESPONSE_CANCEL, nullptr);

    GtkWidget* content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget* grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 8);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 8);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);

    GtkWidget* label_user = gtk_label_new("Username:");
    entry_user = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), label_user, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_user, 1, 0, 2, 1);

    GtkWidget* label_pass = gtk_label_new("Password:");
    entry_pass = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(entry_pass), FALSE);
    gtk_grid_attach(GTK_GRID(grid), label_pass, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_pass, 1, 1, 2, 1);

    avatar_button = gtk_button_new_with_label("Upload Avatar");
    avatar_label = gtk_label_new("No file selected");
    gtk_grid_attach(GTK_GRID(grid), avatar_button, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), avatar_label, 1, 2, 2, 1);
    g_signal_connect(avatar_button, "clicked", G_CALLBACK(on_avatar_clicked), this);

    gtk_container_add(GTK_CONTAINER(content), grid);
    gtk_widget_show_all(dialog);
}

bool LoginDialog::run(std::string& username, std::string& avatarPath)
{
    int result = gtk_dialog_run(GTK_DIALOG(dialog));

    if (result == GTK_RESPONSE_OK)
    {
        const char* user = gtk_entry_get_text(GTK_ENTRY(entry_user));
        const char* pass = gtk_entry_get_text(GTK_ENTRY(entry_pass));

        if (std::string(user) == "student" && std::string(pass) == "tax2026")
        {
            username = user;
            avatarPath = avatar_path;
            success = true;
        }
        else
        {
            GtkWidget* message =
                gtk_message_dialog_new(GTK_WINDOW(dialog), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR,
                                       GTK_BUTTONS_OK, "Login failed.");

            gtk_dialog_run(GTK_DIALOG(message));
            gtk_widget_destroy(message);
            success = false;
        }
    }
    else
    {
        success = false;
    }

    gtk_widget_hide(dialog);
    return success;
}
