# GTK 3 Setup for Windows (MinGW/CMake)

## 1. Download GTK 3
- Download the GTK 3 bundle for Windows from: https://www.gtk.org/download/windows.php
- Extract the contents to a folder, e.g., `C:/gtk` or `C:/msys64/mingw64` (if using MSYS2).

## 2. Add GTK to PATH
- Add the `bin` directory of your GTK installation to your system PATH.
  - Example: `C:/gtk/bin` or `C:/msys64/mingw64/bin`

## 3. CMake Configuration
- The CMake build will look for GTK using `pkg-config`.
- Make sure `pkg-config.exe` is in your PATH (comes with MSYS2 or can be downloaded separately).
- Set the `PKG_CONFIG_PATH` environment variable to the GTK `pkgconfig` directory:
  - Example: `C:/gtk/lib/pkgconfig` or `C:/msys64/mingw64/lib/pkgconfig`

## 4. Build and Run
- From the project root, run:
  ```powershell
  Remove-Item -Recurse -Force build
  cmake -G "MinGW Makefiles" -S . -B build
  cmake --build build
  ```
- The GUI executable will be in the `gui/` folder as `gui_tax_app.exe`.
- Run it from PowerShell or Explorer:
  ```powershell
  ./gui/gui_tax_app.exe
  ```

## 5. Troubleshooting
- If CMake cannot find GTK, check your PATH and `PKG_CONFIG_PATH`.
- If DLLs are missing at runtime, copy them from your GTK `bin` folder to the output folder, or ensure your PATH is set.

---

## Minimal Example for CMakeLists.txt

```
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

add_executable(gui_tax_app ...)
target_link_libraries(gui_tax_app ${GTK3_LIBRARIES})
```

---

For more details, see: https://www.gtk.org/docs/installations/windows/
