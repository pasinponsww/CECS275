# format.ps1
# Checks or applies clang-format to C++ source files.

param(
    [switch] $Fix
)

$ErrorActionPreference = "Stop"

$files = @(
    Get-ChildItem -Path "common", "gui/src" -Recurse -Include *.h, *.cpp -File
)

if ($files.Count -eq 0) {
    Write-Host "No C++ files found."
    exit 0
}

if ($Fix) {
    foreach ($file in $files) {
        clang-format -i $file.FullName
    }

    Write-Host "Formatting applied."
    exit 0
}

foreach ($file in $files) {
    clang-format --dry-run --Werror $file.FullName
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Format check failed: $($file.FullName)"
        exit $LASTEXITCODE
    }
}

Write-Host "Format check passed."
