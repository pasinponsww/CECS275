### Final Project


## Toolchain and Setup Requirements

To build and run this project on Windows, you need the following tools:

### Required Tools

1. **CMake**
	- Download from: https://cmake.org/download/
	- Add CMake to your system PATH during installation.

2. **MinGW-w64 (GCC for Windows)**
	- Download from: https://www.mingw-w64.org/downloads/
	- Make sure to add the `bin` directory (e.g., `C:\msys64\ucrt64\bin` or your MinGW install path) to your system PATH.
	- Verify installation by running `g++ --version` in PowerShell.

3. **PowerShell**
	- Comes pre-installed on Windows 10/11.

4. **GTK 3 (for GUI)**
	- Download the GTK 3 bundle for Windows from: https://www.gtk.org/download/windows.php
	- See [gui/GTK_SETUP.md](gui/GTK_SETUP.md) for detailed setup instructions (including PATH and PKG_CONFIG_PATH).
	- Make sure `pkg-config.exe` is in your PATH (comes with MSYS2 or can be downloaded separately).

---

## GTK GUI Application

This project is GUI-only and uses GTK 3. The GUI executable is built as `gui/gui_tax_app.exe`.

**GTK 3 Setup:**
See [gui/GTK_SETUP.md](gui/GTK_SETUP.md) for step-by-step instructions to set up GTK 3 on Windows and integrate with CMake/MinGW.

**To build and run the GUI:**
1. Follow the GTK setup instructions.
2. Run the build script or CMake commands as above.
3. The GUI executable will be in the `gui/` folder as `gui_tax_app.exe`.
4. Run it with:
   ```powershell
   ./gui/gui_tax_app.exe
   ```

---

### Build Steps

1. Open PowerShell in the project root directory.
2. Run the following command to build the project:

	```powershell
	./build.ps1
	```

	This script will configure and build the project using CMake and MinGW.

3. The GUI executable will be generated in the `gui` directory.

### Troubleshooting

- If you change your compiler or generator, delete the `build` folder before rebuilding:

  ```powershell
  Remove-Item -Recurse -Force build
  ```

- If you see errors about missing compilers or libraries, ensure MinGW-w64 is installed and in your PATH, and that CMake is using the correct generator (`MinGW Makefiles`).

---


## Project Structure

- The `gui/` folder contains the GTK application, including login, avatar upload, calculator, simulation, and chart helpers.
- The `common/core` folder loads and stores shared tax data.
- The `common/math` folder contains pure parsing, validation, and calculation helpers.
- The `common/virtual` folder contains the calculator interface that demonstrates virtual functions and overloads.
- The `data/` folder contains the tax deduction and bracket source files.

---

## Project Description

This project is a 50-year US Tax Study application written in C++. It allows users to:
- Log in and upload an avatar
- Select a tax year (1975-2025) and view tax brackets and deductions
- Calculate taxes for manual or random profiles
- Simulate 1000 fake tax profiles and view summary statistics

See below for more details on features and usage.
