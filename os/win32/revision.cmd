@echo off
echo !define SCHAT_REVISION %SCHAT_REVISION% >  "engine\revision.nsh"
echo #define SCHAT_REVISION %SCHAT_REVISION% >  "..\..\src\common\revision.h"
echo #define SCHAT_PLATFORM "win32"          >> "..\..\src\common\revision.h"