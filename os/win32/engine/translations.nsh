/* $Id: translations.nsh 2217 2012-01-27 00:01:34Z IMPOMEZIA $
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

!ifndef TRANSLATIONS_NSH_
!define TRANSLATIONS_NSH_

!define L "!insertmacro ${CURRENT_LANG}"

!macro L_EN _NAME _TEXT
  LangString ${_NAME} ${LANG_ENGLISH} "${_TEXT}"
!macroend

!macro L_RU _NAME _TEXT
  LangString ${_NAME} ${LANG_RUSSIAN} "${_TEXT}"
!macroend

!macro L_UK _NAME _TEXT
  LangString ${_NAME} ${LANG_UKRAINIAN} "${_TEXT}"
!macroend

!macro INSERT_TRANSLATIONS
  !include "engine\translations\english.nsh"
  !include "engine\translations\russian.nsh"
  !include "engine\translations\ukrainian.nsh"
!macroend

!endif /* TRANSLATIONS_NSH_ */
