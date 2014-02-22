/* $Id: SendFile.nsh 3515 2013-02-22 21:34:29Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2013 IMPOMEZIA <schat@impomezia.com>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

${Name} SendFile
${State} 1

${Body}
${Section}
  SetOutPath "$INSTDIR\plugins"
  File "${SCHAT_SOURCE}\plugins\SendFile.dll"

  SetOutPath "$INSTDIR\doc"
  File "${SCHAT_SOURCE}\doc\ChangeLog.SendFile.html"

  Delete "$INSTDIR\translations\sendfile_en.qm"
  Delete "$INSTDIR\translations\sendfile_ru.qm"
${SectionEnd}
${BodyEnd}

${Uninstall}
  Delete "$INSTDIR\doc\ChangeLog.SendFile.html"
  Delete "$INSTDIR\plugins\SendFile.dll"
  Delete "$INSTDIR\translations\sendfile_en.qm"
  Delete "$INSTDIR\translations\sendfile_ru.qm"

  RMDir "$INSTDIR\doc"
  RMDir "$INSTDIR\plugins"
  RMDir "$INSTDIR\translations"
${UninstallEnd}