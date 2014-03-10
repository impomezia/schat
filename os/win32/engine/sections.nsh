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

!ifndef mod
  !define mod "!insertmacro SCHAT_MOD "
!endif
!ifmacrondef SCHAT_MOD
  !macro SCHAT_MOD _NAME
    !ifdef ${_NAME} | SCHAT_UNINSTALL
      !include "engine\sections\${_NAME}.nsh"
    !endif
  !macroend
!endif

${mod} Core
${mod} Qt

!ifdef SCHAT_SECTIONS
  !if ${SCHAT_PLUGINS_GRP} == 1
    SectionGroup "$(STR1005)" GroupPlugins
  !endif
!endif
${mod} Cache
${mod} History
${mod} Profile
${mod} Popup
${mod} Preview
${mod} Proxy
${mod} Emoticons
${mod} SendFile
${mod} SpellChecker
${mod} Idle
${mod} Update
${mod} Console
${mod} Channels
${mod} YouTube
${mod} RawFeeds
!ifdef SCHAT_SECTIONS
  !if ${SCHAT_PLUGINS_GRP} == 1
    SectionGroupEnd
  !endif
!endif
