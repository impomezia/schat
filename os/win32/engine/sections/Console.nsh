/* $Id: Console.nsh 3181 2012-10-16 12:45:36Z IMPOMEZIA $
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
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

${Name} Console
${State} 1

${Body}
${Section}
  SetOutPath "$INSTDIR\plugins"
  File "${SCHAT_SOURCE}\plugins\Console.dll"

  SetOutPath "$INSTDIR\doc"
  File "${SCHAT_SOURCE}\doc\ChangeLog.Console.html"
${SectionEnd}
${BodyEnd}

${Uninstall}
  Delete "$INSTDIR\doc\ChangeLog.Console.html"
  Delete "$INSTDIR\plugins\Console.dll"

  RMDir "$INSTDIR\doc"
  RMDir "$INSTDIR\plugins"
${UninstallEnd}