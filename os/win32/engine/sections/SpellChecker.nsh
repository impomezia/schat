/* $Id: SpellChecker.nsh 2972 2012-08-04 23:11:07Z IMPOMEZIA $
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

${Name} SpellChecker
${State} 1

${Body}
${Section}
  SetOutPath "$INSTDIR\plugins"
  File "${SCHAT_SOURCE}\plugins\SpellChecker.dll"

  SetOutPath "$INSTDIR\doc"
  File "${SCHAT_SOURCE}\doc\ChangeLog.SpellChecker.html"
  
  SetOutPath "$INSTDIR\spelling"
  File "${SCHAT_SOURCE}\spelling\en_US.aff"
  File "${SCHAT_SOURCE}\spelling\ru_RU.aff"
  File "${SCHAT_SOURCE}\spelling\en_US.dic"
  File "${SCHAT_SOURCE}\spelling\ru_RU.dic"
${SectionEnd}
${BodyEnd}

${Uninstall}
  Delete "$INSTDIR\doc\ChangeLog.SpellChecker.html"
  Delete "$INSTDIR\plugins\SpellChecker.dll"
  Delete "$INSTDIR\spelling\en_US.aff"
  Delete "$INSTDIR\spelling\ru_RU.aff"
  Delete "$INSTDIR\spelling\en_US.dic"
  Delete "$INSTDIR\spelling\ru_RU.dic"

  RMDir "$INSTDIR\doc"
  RMDir "$INSTDIR\plugins"
  RMDir "$INSTDIR\spelling"
${UninstallEnd}