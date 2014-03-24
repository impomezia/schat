/* Simple Chat
 * Copyright (c) 2008-2014 Alexander Sedov <imp@schat.me>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

!define SCHAT_NAME "Simple Chat Server"
!define SCHAT_NAME_SHORT "${SCHAT_NAME}"

!include "MUI2.nsh"
!include "engine\default.nsh"
!include "engine\revision.nsh"

!AddPluginDir "contrib\plugins"

!undef SCHAT_SOURCE
!define SCHAT_SOURCE "schatd2"

BrandingText "${SCHAT_COPYRIGHT}"
Caption "${SCHAT_NAME} ${SCHAT_VERSION}"
InstallDir "${SCHAT_INSTALLDIR}"
InstallDirRegKey HKCU "${SCHAT_REGKEY}" ""
Name "${SCHAT_NAME}"
OutFile "${SCHAT_OUTDIR}schat2-server-${SCHAT_PREFIX}${SCHAT_VERSION}${SCHAT_SUFFIX}.exe"
RequestExecutionLevel user

SetCompressor /SOLID lzma
SetCompressorDictSize 10

VIProductVersion "${SCHAT_VERSION}.${SCHAT_REVISION}"
VIAddVersionKey  "CompanyName"      "Alexander Sedov"
VIAddVersionKey  "FileDescription"  "${SCHAT_NAME} Installer"
VIAddVersionKey  "FileVersion"      "${SCHAT_VERSION}"
VIAddVersionKey  "LegalCopyright"   "${SCHAT_COPYRIGHT}"
VIAddVersionKey  "OriginalFilename" "schat2-server-${SCHAT_VERSION}.exe"
VIAddVersionKey  "ProductName"      "${SCHAT_NAME}"
VIAddVersionKey  "ProductVersion"   "${SCHAT_VERSION}"

ReserveFile "contrib\plugins\FindProcDLL.dll"

!define MUI_ABORTWARNING
!define MUI_COMPONENTSPAGE_SMALLDESC
!define MUI_FINISHPAGE_LINK            "${SCHAT_WEB_SITE}"
!define MUI_FINISHPAGE_LINK_LOCATION   "${SCHAT_WEB_SITE}"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP         "contrib\header.bmp"
!define MUI_HEADERIMAGE_RIGHT
!define MUI_ICON                       "contrib\install.ico"
!define MUI_UNICON                     "contrib\install.ico"

!insertmacro MUI_PAGE_LICENSE "license.txt"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_LANGUAGE "English"

Function .onInit
  newcheck:
  FindProcDLL::FindProc "$INSTDIR\schatd2.exe"
  Pop $R0
  ${If} $R0 == 1 
    MessageBox MB_RETRYCANCEL|MB_ICONEXCLAMATION "An instance of ${SCHAT_NAME} is currently running. Exit ${SCHAT_NAME} and then try again." IDRETRY newcheck
    Quit
  ${EndIf}

  FindProcDLL::FindProc "$INSTDIR\schatd2-srv.exe"
  Pop $R0
  ${If} $R0 == 1 
    MessageBox MB_RETRYCANCEL|MB_ICONEXCLAMATION "An instance of ${SCHAT_NAME} is currently running. Exit ${SCHAT_NAME} and then try again." IDRETRY newcheck
    Quit
  ${EndIf}     
FunctionEnd


Section

  SetOutPath "$INSTDIR"
  File "${SCHAT_SOURCE}\schatd2.exe"
  File "${SCHAT_SOURCE}\schatd2-srv.exe"
  File "${SCHAT_SOURCE}\..\schat2\crashreport.exe"
  File "${SCHAT_SOURCE}\schatd.dll"
  File "${SCHAT_SOURCE}\schat-authd.exe"
  File "${SCHAT_SOURCE}\schat-tufao.dll"
  File "${SCHAT_SOURCE}\schat-rest.dll"
  File "license.txt"

  File "${SCHAT_SOURCE}\libeay32.dll"
  File "${SCHAT_SOURCE}\QtCore4.dll"
  File "${SCHAT_SOURCE}\QtNetwork4.dll"
  File "${SCHAT_SOURCE}\QtSql4.dll"
  File "${SCHAT_SOURCE}\ssleay32.dll"
  File "${SCHAT_SOURCE}\zlib.dll"

  !if ${SCHAT_VC100} == 1
    File "${SCHAT_SOURCE}\msvcp100.dll"
    File "${SCHAT_SOURCE}\msvcr100.dll"
  !endif
  
  SetOutPath "$INSTDIR\plugins"
  File "${SCHAT_SOURCE}\plugins\GenericNode.dll"
  File "${SCHAT_SOURCE}\plugins\Messages.dll"
  File "${SCHAT_SOURCE}\plugins\NodeChannels.dll"
  File "${SCHAT_SOURCE}\plugins\NodeConsole.dll"
  File "${SCHAT_SOURCE}\plugins\GeoIP.dll"
  File "${SCHAT_SOURCE}\plugins\RestApi.dll"
  File "${SCHAT_SOURCE}\plugins\NodeFeedLog.dll"

  SetOutPath "$INSTDIR\plugins\qt\sqldrivers"
  File "${SCHAT_SOURCE}\plugins\qt\sqldrivers\qsqlite4.dll"

  SetOutPath "$INSTDIR\doc"
  File "${SCHAT_SOURCE}\doc\ChangeLog.html"
  File "${SCHAT_SOURCE}\doc\ChangeLog.GenericNode.html"
  File "${SCHAT_SOURCE}\doc\ChangeLog.Messages.html"
  File "${SCHAT_SOURCE}\doc\ChangeLog.NodeChannels.html"
  File "${SCHAT_SOURCE}\doc\ChangeLog.NodeConsole.html"
  File "${SCHAT_SOURCE}\doc\ChangeLog.GeoIP.html"
  File "${SCHAT_SOURCE}\doc\ChangeLog.RestApi.html"
  File "${SCHAT_SOURCE}\doc\ChangeLog.NodeFeedLog.html"

  SetOutPath "$INSTDIR\www"
  File "${SCHAT_SOURCE}\www\receiver.html"
  File "${SCHAT_SOURCE}\www\favicon.ico"

  SetOutPath "$INSTDIR\www\css"
  File "${SCHAT_SOURCE}\www\css\bootstrap.css"
  File "${SCHAT_SOURCE}\www\css\main.css"
  File "${SCHAT_SOURCE}\www\css\result.css"

  SetOutPath "$INSTDIR\www\img"
  File "${SCHAT_SOURCE}\www\img\spinner.gif"
  File "${SCHAT_SOURCE}\www\img\spinner-big.gif"
  File "${SCHAT_SOURCE}\www\img\logo.png"
  File "${SCHAT_SOURCE}\www\img\providers.png"

  SetOutPath "$INSTDIR\www\js"
  File "${SCHAT_SOURCE}\www\js\html5.js"
  File "${SCHAT_SOURCE}\www\js\jquery.min.js"
  File "${SCHAT_SOURCE}\www\js\main.js"
  File "${SCHAT_SOURCE}\www\js\result.js"

  WriteRegStr HKLM "${SCHAT_UNINST_KEY}" "DisplayName"     "${SCHAT_NAME}"
  WriteRegStr HKLM "${SCHAT_UNINST_KEY}" "UnInstallString" "$INSTDIR\uninstall.exe"
  WriteRegStr HKLM "${SCHAT_UNINST_KEY}" "DisplayIcon"     "$INSTDIR\schatd2.exe"
  WriteRegStr HKLM "${SCHAT_UNINST_KEY}" "Publisher"       "Alexander Sedov"
  WriteRegStr HKLM "${SCHAT_UNINST_KEY}" "URLInfoAbout"    "${SCHAT_WEB_SITE}"
  WriteRegStr HKLM "${SCHAT_UNINST_KEY}" "HelpLink"        "${SCHAT_WEB_SITE}"
  WriteRegStr HKLM "${SCHAT_UNINST_KEY}" "URLUpdateInfo"   "${SCHAT_WEB_SITE}"
  WriteRegStr HKLM "${SCHAT_UNINST_KEY}" "DisplayVersion"  "${SCHAT_VERSION}"
  
  WriteRegStr HKCU "${SCHAT_REGKEY}" "" $INSTDIR
  WriteUninstaller "$INSTDIR\uninstall.exe"
  WriteINIStr "$INSTDIR\schatd2.init" "General" "Portable" true
  WriteINIStr "$INSTDIR\schat-authd.init" "General" "Portable" true
SectionEnd

Section "Uninstall"

  Delete "$INSTDIR\msvcp100.dll"
  Delete "$INSTDIR\msvcr100.dll"

  Delete "$INSTDIR\libeay32.dll"
  Delete "$INSTDIR\QtCore4.dll"
  Delete "$INSTDIR\QtNetwork4.dll"
  Delete "$INSTDIR\QtSql4.dll"
  Delete "$INSTDIR\ssleay32.dll"
  Delete "$INSTDIR\zlib1.dll"
  Delete "$INSTDIR\zlib.dll"

  Delete "$INSTDIR\server.crt"
  Delete "$INSTDIR\server.key"

  Delete "$INSTDIR\schatd2.exe"
  Delete "$INSTDIR\schatd2-srv.exe"
  Delete "$INSTDIR\schat-authd.exe"
  Delete "$INSTDIR\schatd.dll"
  Delete "$INSTDIR\schat-tufao.dll"
  Delete "$INSTDIR\schat-rest.dll"
  Delete "$INSTDIR\license.txt"
  Delete "$INSTDIR\schat-authd.init"
  Delete "$INSTDIR\schatd2.init"

  Delete "$INSTDIR\plugins\qt\sqldrivers\qsqlite4.dll"
  Delete "$INSTDIR\plugins\GenericNode.dll"
  Delete "$INSTDIR\plugins\Messages.dll"
  Delete "$INSTDIR\plugins\NodeChannels.dll"
  Delete "$INSTDIR\plugins\NodeConsole.dll"
  Delete "$INSTDIR\plugins\GeoIP.dll"
  Delete "$INSTDIR\plugins\RestApi.dll"
  Delete "$INSTDIR\plugins\NodeFeedLog.dll"

  Delete "$INSTDIR\doc\ChangeLog.html"
  Delete "$INSTDIR\doc\ChangeLog.GenericNode.html"
  Delete "$INSTDIR\doc\ChangeLog.Messages.html"
  Delete "$INSTDIR\doc\ChangeLog.NodeChannels.html"
  Delete "$INSTDIR\doc\ChangeLog.NodeConsole.html"
  Delete "$INSTDIR\doc\ChangeLog.GeoIP.html"
  Delete "$INSTDIR\doc\ChangeLog.RestApi.html"
  Delete "$INSTDIR\doc\ChangeLog.NodeFeedLog.html"

  Delete "$INSTDIR\www\css\bootstrap.css"
  Delete "$INSTDIR\www\css\main.css"
  Delete "$INSTDIR\www\css\result.css"
  Delete "$INSTDIR\www\img\spinner.gif"
  Delete "$INSTDIR\www\img\spinner-big.gif"
  Delete "$INSTDIR\www\img\logo.png"
  Delete "$INSTDIR\www\img\providers.png"
  Delete "$INSTDIR\www\js\html5.js"
  Delete "$INSTDIR\www\js\jquery-1.8.2.min.js"
  Delete "$INSTDIR\www\js\jquery-1.9.1.min.js"
  Delete "$INSTDIR\www\js\main.js"
  Delete "$INSTDIR\www\js\result.js"
  Delete "$INSTDIR\www\receiver.html"
  Delete "$INSTDIR\www\favicon.ico"

  RMDir "$INSTDIR\plugins\qt\sqldrivers"
  RMDir "$INSTDIR\plugins\qt"
  RMDir "$INSTDIR\doc"
  RMDir "$INSTDIR\plugins"
  RMDir "$INSTDIR\www\css"
  RMDir "$INSTDIR\www\img"
  RMDir "$INSTDIR\www\js"
  RMDir "$INSTDIR\www"

  Delete "$INSTDIR\uninstall.exe"
  DeleteRegKey HKLM "${SCHAT_UNINST_KEY}"
  RMDir "$INSTDIR"
SectionEnd
