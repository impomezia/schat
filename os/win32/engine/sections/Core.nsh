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

${Name} Core

${Body}
${Section}
  SectionIn RO
  !insertmacro KILL_ALL "schat2.exe"

  SetOutPath "$INSTDIR"
  File "${SCHAT_SOURCE}\schat2.exe"
  File "${SCHAT_SOURCE}\schat.dll"
  File "${SCHAT_SOURCE}\schat-client.dll"
  File "license.txt"

  SetOutPath "$INSTDIR\doc"
  File "${SCHAT_SOURCE}\doc\ChangeLog.html"

  SetOutPath "$INSTDIR\sounds"
  File "${SCHAT_SOURCE}\sounds\Received.wav"

  SetOutPath "$INSTDIR\translations"
  File "${SCHAT_SOURCE}\translations\schat2_en.qm"
  File "${SCHAT_SOURCE}\translations\schat2_ru.qm"
  File "${SCHAT_SOURCE}\translations\schat2_uk.qm"
  File "${SCHAT_SOURCE}\translations\qt_ru.qm"
  File "${SCHAT_SOURCE}\translations\qt_uk.qm"
  File "${SCHAT_SOURCE}\translations\en.png"
  File "${SCHAT_SOURCE}\translations\ru.png"
  File "${SCHAT_SOURCE}\translations\uk.png"

  SetOutPath "$INSTDIR\plugins"
  File "${SCHAT_SOURCE}\plugins\Http.dll"

  WriteRegStr HKCU "${SCHAT_REGKEY}" "" $INSTDIR
  WriteUninstaller "$INSTDIR\uninstall.exe"
  WriteINIStr "$INSTDIR\schat2.init" "General" "Portable" true

  ReadTagDLL::extract "$INSTDIR\default.conf"
  CopyFiles /SILENT /FILESONLY '$EXEDIR\default.conf' '$INSTDIR\default.conf'
${SectionEnd}
${BodyEnd}

${Uninstall}
  Delete "$INSTDIR\schat2.exe"
  Delete "$INSTDIR\schat.dll"
  Delete "$INSTDIR\schat-client.dll"
  Delete "$INSTDIR\license.txt"
  Delete "$INSTDIR\uninstall.exe"
  Delete "$INSTDIR\doc\ChangeLog.html"
  Delete "$INSTDIR\sounds\Received.wav"
  Delete "$INSTDIR\translations\schat2_en.qm"
  Delete "$INSTDIR\translations\schat2_ru.qm"
  Delete "$INSTDIR\translations\schat2_uk.qm"
  Delete "$INSTDIR\translations\qt_ru.qm"
  Delete "$INSTDIR\translations\qt_uk.qm"
  Delete "$INSTDIR\translations\en.png"
  Delete "$INSTDIR\translations\ru.png"
  Delete "$INSTDIR\translations\uk.png"
  Delete "$INSTDIR\plugins\Http.dll"

  RMDir "$INSTDIR\doc"
  RMDir "$INSTDIR\sounds"
  RMDir "$INSTDIR\translations"
  RMDir "$INSTDIR\plugins"

  DeleteRegKey HKCU "${SCHAT_REGKEY}"
${UninstallEnd}
