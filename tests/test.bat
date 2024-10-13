@echo off

setlocal
set current_dir=%cd%
echo Starting directory: %current_dir%

set expected_base_directory=IU_Compilulki_Compiler

:find_base_directory
for %%i in ("%current_dir%") do (
    if "%%~nxi" neq "%expected_base_directory%" (
        cd ..
        set current_dir=%cd%
        if "%current_dir%" neq "C:\" goto find_base_directory
    )
)

set green=
set red=
set nc=

if "%current_dir%"=="C:\" (
    echo The base directory '%expected_base_directory%' was not found in the directory structure.
    exit /b 1
)

if "%expected_base_directory%"=="%~nx0" goto base_directory_found

:base_directory_found
for /r "%current_dir%\examples" %%f in (*.olang) do (
    "%current_dir%\compilulki.exe" "%%f" > nul
    if %errorlevel% equ 0 (
        echo Successfully processed: %%f
    ) else (
        echo Error processing: %%f (Exit Code: %errorlevel%)
    )
    echo =============
)

endlocal
