@echo off

setlocal enabledelayedexpansion
set current_dir=%cd%
echo Starting directory: %current_dir%

set green=
set red=
set nc=

for /r "%current_dir%\examples" %%f in (*.olang) do (
    "%current_dir%\compilulki.exe" "%%f" > nul
    if !ERRORLEVEL! == 0 (echo Successfully processed: %%f) else (echo Error processing: %%f)
    echo =============
)
endlocal
