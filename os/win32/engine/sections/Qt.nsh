/* $Id: Qt.nsh 3679 2013-06-08 16:38:22Z IMPOMEZIA $
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

${Name} Qt

${Body}
${Section}
  SectionIn RO

  SetOutPath "$INSTDIR"
  File "${SCHAT_SOURCE}\libeay32.dll"
  File "${SCHAT_SOURCE}\libpng15.dll"
  File "${SCHAT_SOURCE}\QtCore4.dll"
  File "${SCHAT_SOURCE}\QtGui4.dll"
  File "${SCHAT_SOURCE}\QtNetwork4.dll"
  File "${SCHAT_SOURCE}\QtSql4.dll"
  File "${SCHAT_SOURCE}\QtWebKit4.dll"
  File "${SCHAT_SOURCE}\ssleay32.dll"
  File "${SCHAT_SOURCE}\zlib.dll"
  File "${SCHAT_SOURCE}\qt.conf"

  !if ${SCHAT_VC100} == 1
    File "${SCHAT_SOURCE}\msvcp100.dll"
    File "${SCHAT_SOURCE}\msvcr100.dll"
  !endif

  SetOutPath "$INSTDIR\plugins\qt\imageformats"
  File "${SCHAT_SOURCE}\plugins\qt\imageformats\qgif4.dll"
  File "${SCHAT_SOURCE}\plugins\qt\imageformats\qico4.dll"
  File "${SCHAT_SOURCE}\plugins\qt\imageformats\qjpeg4.dll"

  SetOutPath "$INSTDIR\plugins\qt\sqldrivers"
  File "${SCHAT_SOURCE}\plugins\qt\sqldrivers\qsqlite4.dll"

  SetOutPath "$INSTDIR\plugins\qt\bearer"
  File "${SCHAT_SOURCE}\plugins\qt\bearer\qgenericbearer4.dll"
${SectionEnd}
${BodyEnd}


${Uninstall}
  Delete "$INSTDIR\libeay32.dll"
  Delete "$INSTDIR\libpng15.dll"
  Delete "$INSTDIR\QtCore4.dll"
  Delete "$INSTDIR\QtGui4.dll"
  Delete "$INSTDIR\QtNetwork4.dll"
  Delete "$INSTDIR\QtSql4.dll"
  Delete "$INSTDIR\QtWebKit4.dll"
  Delete "$INSTDIR\ssleay32.dll"
  Delete "$INSTDIR\zlib1.dll"
  Delete "$INSTDIR\zlib.dll"
  Delete "$INSTDIR\qt.conf"
  Delete "$INSTDIR\msvcp100.dll"
  Delete "$INSTDIR\msvcr100.dll"
  Delete "$INSTDIR\plugins\qt\imageformats\qgif4.dll"
  Delete "$INSTDIR\plugins\qt\imageformats\qico4.dll"
  Delete "$INSTDIR\plugins\qt\imageformats\qjpeg4.dll"
  Delete "$INSTDIR\plugins\qt\sqldrivers\qsqlite4.dll"
  Delete "$INSTDIR\plugins\qt\bearer\qgenericbearer4.dll"

  RMDir "$INSTDIR\plugins\qt\imageformats"
  RMDir "$INSTDIR\plugins\qt\sqldrivers"
  RMDir "$INSTDIR\plugins\qt\bearer"
  RMDir "$INSTDIR\plugins\qt"
  RMDir "$INSTDIR\plugins"
${UninstallEnd}
