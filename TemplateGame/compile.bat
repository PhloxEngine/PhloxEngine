@echo off
echo Compiling game to .phlox format...
PhloxCompiler.exe GameInfo.json source\*.phs -o Data.phlox
echo Done!
pause 