# build.ps1
# Run this from the project root.

$ErrorActionPreference = "Stop"

$mingwRoot = "C:\msys64\ucrt64"
$gcc = Join-Path $mingwRoot "bin\gcc.exe"
$gxx = Join-Path $mingwRoot "bin\g++.exe"
$pkgConfigPath = Join-Path $mingwRoot "lib\pkgconfig"

if (!(Test-Path $gcc) -or !(Test-Path $gxx)) {
    Write-Host "Could not find MSYS2 UCRT64 gcc/g++."
    Write-Host "Expected:"
    Write-Host "  $gcc"
    Write-Host "  $gxx"
    Write-Host ""
    Write-Host "Install MSYS2 UCRT64 or update `$mingwRoot in build.ps1."
    exit 1
}

$env:PATH = (Join-Path $mingwRoot "bin") + ";" + $env:PATH
$env:PKG_CONFIG_PATH = $pkgConfigPath

if (Test-Path "build") {
    Remove-Item -Recurse -Force build
}

cmake `
    -G "MinGW Makefiles" `
    -S . `
    -B build `
    -DCMAKE_C_COMPILER="$gcc" `
    -DCMAKE_CXX_COMPILER="$gxx"

if ($LASTEXITCODE -ne 0) {
    Write-Host "CMake configure failed."
    exit $LASTEXITCODE
}

cmake --build build

if ($LASTEXITCODE -ne 0) {
    Write-Host "Build failed."
    exit $LASTEXITCODE
}

Write-Host ""
Write-Host "Build complete."
Write-Host "GUI app: gui\gui_tax_app.exe"
