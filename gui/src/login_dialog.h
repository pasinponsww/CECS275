/**
 * @file login_dialog.h
 * @brief GTK login dialog with avatar file selection.
 */

#pragma once

#include <gtk/gtk.h>
#include <string>

/**
 * @class LoginDialog
 * @brief Handles Part A: username/password login and avatar upload.
 *
 * @details The dialog validates the simple project credentials and stores the
 * selected avatar path so the main window can display it after login.
 */
class LoginDialog
{
  public:
    /**
     * @brief Creates the login dialog.
     * @param parent Optional parent GTK window.
     */
    explicit LoginDialog(GtkWindow* parent);

    /**
     * @brief Runs the modal login dialog.
     * @param username Output username after successful login.
     * @param avatarPath Output avatar image path selected by the user.
     * @return true when login succeeds; false when login fails or is cancelled.
     */
    bool run(std::string& username, std::string& avatarPath);

    /**
     * @brief Label updated after the user selects an avatar image.
     */
    GtkWidget* avatar_label;

    /**
     * @brief File path selected by the avatar picker.
     */
    std::string avatar_path;

  private:
    GtkWidget* dialog;
    GtkWidget* entry_user;
    GtkWidget* entry_pass;
    GtkWidget* avatar_button;
    bool success;

    /**
     * @brief GTK callback for the avatar upload button.
     * @param button Button that emitted the signal.
     * @param user_data LoginDialog instance.
     */
    static void on_avatar_clicked(GtkButton* button, gpointer user_data);
};
