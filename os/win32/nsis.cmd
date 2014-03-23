@echo off
cd ../../out
for %%I in (*.exe *.dll) do SignTool.exe sign /f %SCHAT_SIGN_FILE% /p %SCHAT_SIGN_PASSWORD% /t http://timestamp.verisign.com/scripts/timstamp.dll %%~fI

cd plugins
for %%I in (*.dll) do SignTool.exe sign /f %SCHAT_SIGN_FILE% /p %SCHAT_SIGN_PASSWORD% /t http://timestamp.verisign.com/scripts/timstamp.dll %%~fI

cd ../..
jom install /NOLOGO
if ERRORLEVEL 1 exit 1

cd os/win32
makensis setup.nsi
if ERRORLEVEL 1 exit 1

makensis server.nsi
if ERRORLEVEL 1 exit 1

cd wix
candle -nologo -dProductVersion=%SCHAT_VERSION% -ext WixUIExtension Files.wxs Product.wxs Features.wxs Shortcuts.wxs
if ERRORLEVEL 1 exit 1

light -nologo -out ..\out\schat2-%SCHAT_VERSION%.msi -cultures:en-US -ext WixUIExtension Files.wixobj Product.wixobj Features.wixobj Shortcuts.wixobj
if ERRORLEVEL 1 exit 1

cd ..\out
for %%I in (*.exe) do SignTool.exe sign /f %SCHAT_SIGN_FILE% /p %SCHAT_SIGN_PASSWORD% /t http://timestamp.verisign.com/scripts/timstamp.dll %%~fI
for %%I in (*.msi) do SignTool.exe sign /f %SCHAT_SIGN_FILE% /p %SCHAT_SIGN_PASSWORD% /t http://timestamp.verisign.com/scripts/timstamp.dll %%~fI

cd ..

cd schat2
copy /Y %QTDIR%\bin\QtCore4.pdb .
copy /Y %QTDIR%\bin\QtGui4.pdb .
copy /Y %QTDIR%\bin\QtNetwork4.pdb .
copy /Y %QTDIR%\bin\QtSql4.pdb .
copy /Y %QTDIR%\bin\QtWebKit4.pdb .

copy /Y %QTDIR%\plugins\bearer\qgenericbearer4.pdb plugins\qt\bearer
copy /Y %QTDIR%\plugins\imageformats\qgif4.pdb plugins\qt\imageformats
copy /Y %QTDIR%\plugins\imageformats\qico4.pdb plugins\qt\imageformats
copy /Y %QTDIR%\plugins\imageformats\qjpeg4.pdb plugins\qt\imageformats
copy /Y %QTDIR%\plugins\sqldrivers\qsqlite4.pdb plugins\qt\sqldrivers

copy /Y ..\..\..\out\*.pdb .
copy /Y ..\..\..\out\plugins\*.pdb plugins

7z a -r0 -mx=7 ../out/schat2-%SCHAT_VERSION%-pdb.7z *.pdb

cd ..
exit 0