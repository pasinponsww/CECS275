# docs.ps1
# Generates Doxygen HTML documentation.

$ErrorActionPreference = "Stop"

if (!(Get-Command doxygen -ErrorAction SilentlyContinue)) {
    Write-Host "Doxygen was not found on PATH."
    Write-Host "Install it, then rerun: .\docs.ps1"
    exit 1
}

New-Item -ItemType Directory -Force -Path "docs" | Out-Null

doxygen Doxyfile

if ($LASTEXITCODE -ne 0) {
    Write-Host "Doxygen generation failed."
    exit $LASTEXITCODE
}

Write-Host ""
Write-Host "Doxygen HTML generated:"
Write-Host "docs\doxygen\html\index.html"
