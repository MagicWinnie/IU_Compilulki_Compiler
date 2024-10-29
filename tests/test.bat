@echo off

setlocal enabledelayedexpansion
set current_dir=%cd%
echo Starting directory: %current_dir%

set green=[32m
set red=[31m
set nc=[0m
for /r "%current_dir%\examples" %%f in (*.olang) do (
    echo.%%f| find /I "\errors\" > nul && (
      "%current_dir%\compilulki.exe" -g "%%f" > nul
      if !ERRORLEVEL! == 0 (
          echo !red!Successfully processed: %%f!nc!
      ) else (
          echo !green!Error processing: %%f!nc!
      )
    ) || (
      "%current_dir%\compilulki.exe" -g "%%f" > nul
      if !ERRORLEVEL! == 0 (
          echo !green!Successfully processed: %%f!nc!
      ) else (
          echo !red!Error processing: %%f!nc!
      )
    )
    echo =============
)
endlocal