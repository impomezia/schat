@echo off
cd ../../out
for %%I in (*.exe *.dll) do SignTool.exe sign /f %SCHAT_SIGN_FILE% /p %SCHAT_SIGN_PASSWORD% /t http://timestamp.verisign.com/scripts/timstamp.dll %%~fI

cd plugins
for %%I in (*.dll) do SignTool.exe sign /f %SCHAT_SIGN_FILE% /p %SCHAT_SIGN_PASSWORD% /t http://timestamp.verisign.com/scripts/timstamp.dll %%~fI

cd ../..
nmake install /NOLOGO
if ERRORLEVEL 1 exit 1

cd os/win32
makensis setup.nsi
if ERRORLEVEL 1 exit 1

makensis server.nsi
if ERRORLEVEL 1 exit 1

cd wix
candle -nologo -dProductVersion=%SCHAT_VERSION%.%SCHAT_REVISION% -ext WixUIExtension Files.wxs Product.wxs Features.wxs Shortcuts.wxs
if ERRORLEVEL 1 exit 1

light -nologo -out ..\out\schat2-%SCHAT_VERSION%.msi -cultures:en-US -ext WixUIExtension Files.wixobj Product.wixobj Features.wixobj Shortcuts.wixobj
if ERRORLEVEL 1 exit 1

cd ..\out
for %%I in (*.exe) do SignTool.exe sign /f %SCHAT_SIGN_FILE% /p %SCHAT_SIGN_PASSWORD% /t http://timestamp.verisign.com/scripts/timstamp.dll %%~fI
for %%I in (*.msi) do SignTool.exe sign /f %SCHAT_SIGN_FILE% /p %SCHAT_SIGN_PASSWORD% /t http://timestamp.verisign.com/scripts/timstamp.dll %%~fI

cd ..
exit 0