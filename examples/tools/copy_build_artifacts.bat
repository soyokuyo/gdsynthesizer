@echo off
REM Copy build artifacts from project/bin to examples/*/godot_project/bin
REM Only copies files if destination is older or missing

setlocal enabledelayedexpansion

REM Get the script directory (tools directory)
set "SCRIPT_DIR=%~dp0"
REM Get the repository root (parent of tools directory)
set "REPO_ROOT=%SCRIPT_DIR%..\.."

REM Source directory
set "SOURCE_DIR=%REPO_ROOT%\project\bin"

REM Check if source directory exists
if not exist "%SOURCE_DIR%" (
    echo Error: Source directory not found: %SOURCE_DIR%
    exit /b 1
)

echo Copying build artifacts from %SOURCE_DIR%
echo.

REM Find all example project bin directories
for /d %%E in ("%REPO_ROOT%\examples\*") do (
    set "BIN_DIR=%%E\godot_project\bin"
    
    REM Check if bin directory exists
    if exist "!BIN_DIR!" (
        echo Processing: !BIN_DIR!
        
        REM Copy .gdextension file
        if exist "%SOURCE_DIR%\gdsynthesizer.gdextension" (
            robocopy "%SOURCE_DIR%" "!BIN_DIR!" gdsynthesizer.gdextension /IS /IT /NP /NFL /NDL >nul
            if !errorlevel! leq 1 (
                echo   Copied: gdsynthesizer.gdextension
            )
        )
        
        REM Copy DLL files
        for %%F in ("%SOURCE_DIR%\*.dll") do (
            set "FILENAME=%%~nxF"
            robocopy "%SOURCE_DIR%" "!BIN_DIR!" "!FILENAME!" /IS /IT /NP /NFL /NDL >nul
            if !errorlevel! leq 1 (
                echo   Copied: !FILENAME!
            )
        )
        
        REM Copy WASM files
        for %%F in ("%SOURCE_DIR%\*.wasm") do (
            set "FILENAME=%%~nxF"
            robocopy "%SOURCE_DIR%" "!BIN_DIR!" "!FILENAME!" /IS /IT /NP /NFL /NDL >nul
            if !errorlevel! leq 1 (
                echo   Copied: !FILENAME!
            )
        )
        
        echo.
    )
)

echo Done.
endlocal

