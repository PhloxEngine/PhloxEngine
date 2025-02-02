@echo off
setlocal EnableDelayedExpansion

echo Compiling game to .phlox format...

if not exist "Lettuce.exe" (
    echo Error: Lettuce.exe not found in current directory!
    dir
    pause
    exit /b 1
)

if not exist "GameInfo.json" (
    echo Error: GameInfo.json not found!
    dir
    pause
    exit /b 1
)

if not exist "build" mkdir build

if not exist "source\*.phs" (
    echo Error: No .phs files found in source directory!
    dir source
    pause
    exit /b 1
)

set "command=Lettuce.exe GameInfo.json"
for %%f in (source\*.phs) do (
    set "command=!command! "%%f""
)
set "command=!command! -o build\Data.phlox"

echo Executing: !command!
!command!

if errorlevel 1 (
    echo Compilation failed!
    pause
    exit /b 1
)

if not exist "build\Data.phlox" (
    echo Error: build\Data.phlox was not created!
    pause
    exit /b 1
)

for %%I in (build\Data.phlox) do if %%~zI==0 (
    echo Error: build\Data.phlox is empty!
    pause
    exit /b 1
)

echo Build successful!
pause
