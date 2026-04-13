/**
 * @file main_window.h
 * @brief Main GTK notebook window for Parts A, B, and C.
 */

#pragma once

#include <gtk/gtk.h>
#include <string>

/**
 * @class MainWindow
 * @brief Builds the GUI tabs after successful login.
 *
 * @details The class keeps the top-level GTK notebook together and delegates
 * each requirement section to a focused private builder function.
 */
class MainWindow
{
  public:
    /**
     * @brief Creates the main tabbed window content.
     * @param username Logged-in username.
     * @param avatarPath Avatar image path selected during login.
     */
    MainWindow(const std::string& username, const std::string& avatarPath);

    /**
     * @brief Returns the GTK widget owned by this window wrapper.
     * @return Notebook widget containing all app tabs.
     */
    GtkWidget* widget();

  private:
    GtkWidget* notebook;

    /**
     * @brief Builds the login/avatar summary tab.
     */
    GtkWidget* build_part_a(const std::string& username, const std::string& avatarPath);

    /**
     * @brief Builds the tax year table and calculator tab.
     */
    GtkWidget* build_part_b();

    /**
     * @brief Builds the fake profile simulation tab.
     */
    GtkWidget* build_part_c();
};
