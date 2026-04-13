cmake_minimum_required(VERSION 3.10)
project(CECS275_TaxStudy_GUI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(
    data
    common/core
    common/math
    common/virtual
    ${GTK3_INCLUDE_DIRS}
)

link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

set(GUI_SRC_FILES
    gui/src/tax_app_gui.cpp
    gui/src/login_dialog.cpp
    gui/src/main_window.cpp
    gui/src/gui_data.cpp
    gui/src/pie_chart.cpp
    gui/src/line_chart.cpp
    common/core/utils.cpp
    common/math/taxmath.cpp
)

add_executable(gui_tax_app ${GUI_SRC_FILES})
set_target_properties(gui_tax_app PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/gui)
target_link_libraries(gui_tax_app ${GTK3_LIBRARIES})
