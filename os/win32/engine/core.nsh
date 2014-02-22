/* $Id: core.nsh 3339 2012-12-22 00:13:57Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2012 IMPOMEZIA <schat@impomezia.com>
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

!include "MUI2.nsh"
!include "FileFunc.nsh"
!include "WordFunc.nsh"
!include "engine\default.nsh"
!include "engine\schat.nsh"
!include "engine\page-options.nsh"
!include "engine\update.nsh"
!include "engine\revision.nsh"

!define SCHAT_META
!include "engine\sections.nsh"

BrandingText "${SCHAT_COPYRIGHT}"
Caption "${SCHAT_NAME} ${SCHAT_VERSION}"
InstallDir "${SCHAT_INSTALLDIR}"
InstallDirRegKey HKCU "${SCHAT_REGKEY}" ""
Name "${SCHAT_NAME}"
OutFile "${SCHAT_OUTDIR}schat2-${SCHAT_PREFIX}${SCHAT_VERSION}${SCHAT_SUFFIX}.exe"
RequestExecutionLevel user

SetCompressor /SOLID lzma
SetCompressorDictSize 10

VIProductVersion "${SCHAT_VERSION}.${SCHAT_REVISION}"
VIAddVersionKey  "CompanyName"      "Alexander Sedov"
VIAddVersionKey  "FileDescription"  "${SCHAT_NAME} Installer"
VIAddVersionKey  "FileVersion"      "${SCHAT_VERSION}"
VIAddVersionKey  "LegalCopyright"   "${SCHAT_COPYRIGHT}"
VIAddVersionKey  "OriginalFilename" "schat2-${SCHAT_VERSION}.exe"
VIAddVersionKey  "ProductName"      "${SCHAT_NAME}"
VIAddVersionKey  "ProductVersion"   "${SCHAT_VERSION}"

!if ${SCHAT_CHECK_RUN} == 1
 ReserveFile "contrib\plugins\FindProcDLL.dll"
 ReserveFile "contrib\plugins\KillProcDLL.dll"
!endif

!if ${SCHAT_FINISH_RUN} == 1
  !define MUI_FINISHPAGE_RUN "$INSTDIR\schat2.exe"
!endif

!define MUI_ABORTWARNING
!define MUI_COMPONENTSPAGE_SMALLDESC
!define MUI_FINISHPAGE_LINK            "${SCHAT_WEB_SITE}"
!define MUI_FINISHPAGE_LINK_LOCATION   "${SCHAT_WEB_SITE}"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP         "contrib\header.bmp"
!define MUI_HEADERIMAGE_RIGHT
!define MUI_ICON                       "contrib\install.ico"
!define MUI_UNICON                     "contrib\install.ico"
!define MUI_WELCOMEFINISHPAGE_BITMAP   "contrib\wizard.bmp"
!define MUI_LANGDLL_REGISTRY_ROOT      "HKCU"
!define MUI_LANGDLL_REGISTRY_KEY       "${SCHAT_REGKEY}"
!define MUI_LANGDLL_REGISTRY_VALUENAME "language"
!define MUI_LANGDLL_ALWAYSSHOW
!define MUI_WELCOMEPAGE_TITLE_3LINES
!define MUI_FINISHPAGE_TITLE_3LINES

!insertmacro MUI_RESERVEFILE_LANGDLL

!define MUI_PAGE_CUSTOMFUNCTION_PRE updatePre
!insertmacro MUI_PAGE_WELCOME
!define MUI_PAGE_CUSTOMFUNCTION_PRE updatePre
!insertmacro MUI_PAGE_LICENSE "license.txt"
!define MUI_PAGE_CUSTOMFUNCTION_PRE updatePre
!insertmacro MUI_PAGE_COMPONENTS
!define MUI_PAGE_CUSTOMFUNCTION_PRE updatePre
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro OPTIONS_PAGE
!insertmacro MUI_PAGE_INSTFILES
!define MUI_PAGE_CUSTOMFUNCTION_PRE updatePre
!insertmacro MUI_PAGE_FINISH

!ifdef Core
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
!endif

!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "Russian"
!insertmacro MUI_LANGUAGE "Ukrainian"


/*
 * Основные секции.
 */
!undef SCHAT_META
!define SCHAT_SECTIONS
!include "engine\sections.nsh"
!insertmacro SECTION_OPTIONS


/*
 * Завершающие действия.
 */
!undef SCHAT_SECTIONS
!define SCHAT_POST
Section
  WriteINIStr "$INSTDIR\uninstall.ini" "${SCHAT_NAME}" "Version" "${SCHAT_VERSION}"

  !include "engine\sections.nsh"

  WriteRegStr HKLM "${SCHAT_UNINST_KEY}" "DisplayName"     "${SCHAT_NAME}"
  WriteRegStr HKLM "${SCHAT_UNINST_KEY}" "UnInstallString" "$INSTDIR\uninstall.exe"
  WriteRegStr HKLM "${SCHAT_UNINST_KEY}" "DisplayIcon"     "$INSTDIR\schat2.exe"
  WriteRegStr HKLM "${SCHAT_UNINST_KEY}" "Publisher"       "Alexander Sedov"
  WriteRegStr HKLM "${SCHAT_UNINST_KEY}" "URLInfoAbout"    "${SCHAT_WEB_SITE}"
  WriteRegStr HKLM "${SCHAT_UNINST_KEY}" "HelpLink"        "http://wiki.schat.me/"
  WriteRegStr HKLM "${SCHAT_UNINST_KEY}" "URLUpdateInfo"   "https://schat.me/download"
  WriteRegStr HKLM "${SCHAT_UNINST_KEY}" "DisplayVersion"  "${SCHAT_VERSION}"
  WriteRegStr HKLM "${SCHAT_UNINST_KEY}" "InstallLocation" "$INSTDIR"
  WriteRegStr HKLM "${SCHAT_UNINST_KEY}" "Contact"         "support@impomezia.com"
  WriteRegDWORD HKLM "${SCHAT_UNINST_KEY}" "VersionMajor"  1
  WriteRegDWORD HKLM "${SCHAT_UNINST_KEY}" "VersionMinor"  99

  ${If} $update == true
    Exec '"$INSTDIR\schat2.exe"'
  ${EndIf}
SectionEnd


/*
 * Удаление.
 */
!undef SCHAT_POST
!define SCHAT_UNINSTALL
!ifdef Core
Section "Uninstall"
  Delete "$DESKTOP\${SCHAT_NAME}.lnk"
  Delete "$QUICKLAUNCH\${SCHAT_NAME}.lnk"
  Delete "${SCHAT_PROGRAMGROUP}\*.lnk"
  RMDir  "${SCHAT_PROGRAMGROUP}"

  !include "engine\sections.nsh"

  Delete "$INSTDIR\uninstall.ini"
  DeleteRegKey HKLM "${SCHAT_UNINST_KEY}"
  RMDir "$INSTDIR"
SectionEnd
!endif


/*
 * Описания секций.
 */
!undef SCHAT_UNINSTALL
!define SCHAT_DESC
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !if ${SCHAT_PLUGINS_GRP} == 1
    !insertmacro MUI_DESCRIPTION_TEXT ${GroupPlugins} $(STR1005)
  !endif

  !include "engine\sections.nsh"
!insertmacro MUI_FUNCTION_DESCRIPTION_END


/*
 * Инициализация.
 */
!undef SCHAT_DESC
!define SCHAT_INIT
!insertmacro UPDATE_ENGINE_FUNCTIONS
!insertmacro GetParameters
!insertmacro GetOptionsS
!insertmacro GetParent

Function .onInit
  !insertmacro UPDATE_CMD
  ${If} $update == false
  !insertmacro MUI_LANGDLL_DISPLAY
  ${EndIf}

  !insertmacro OPTIONS_PAGE_INIT
  !include "engine\sections.nsh"
FunctionEnd


/*
 * Инициализация деинсталлятора.
 */
!ifdef Core
Function un.onInit
  !insertmacro MUI_UNGETLANGUAGE
  !insertmacro UPDATE_ENGINE_INIT
FunctionEnd
!endif

!insertmacro OPTIONS_PAGE_FUNC
!insertmacro INSERT_TRANSLATIONS
!insertmacro FIND_RUNNING
!insertmacro UN_FIND_RUNNING

Function updatePre
  ${If} $update == true
    Abort
  ${EndIf}
FunctionEnd
