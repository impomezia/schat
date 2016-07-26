/* Simple Chat
 * Copyright © 2008-2016 Alexander Sedov <imp@schat.me>
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

!ifndef DEFAULT_NSH_
!define DEFAULT_NSH_

!define def "!insertmacro DEFAULT "
!macro DEFAULT _PARAM _VALUE
  !ifndef ${_PARAM}
    !define ${_PARAM} "${_VALUE}"
  !endif
!macroend

${def} SCHAT_VERSION       "2.4.0"
${def} SCHAT_QT_VERSION    "4.8.7"
${def} SCHAT_NAME          "Simple Chat 2"
${def} SCHAT_NAME_SHORT    "Simple Chat 2"
${def} SCHAT_PREFIX        ""
${def} SCHAT_SUFFIX        ""
${def} SCHAT_OUTDIR        "out/"
${def} SCHAT_FINISH_RUN    1
${def} SCHAT_REGKEY        "Software\IMPOMEZIA\${SCHAT_NAME_SHORT}"
${def} SCHAT_WEB_SITE      "https://schat.me/"
${def} SCHAT_COPYRIGHT     "Copyright © 2008-2016 Alexander Sedov"
${def} SCHAT_UNINST_KEY    "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SCHAT_NAME_SHORT}"
${def} SCHAT_INSTALLDIR    "$LOCALAPPDATA\IMPOMEZIA\${SCHAT_NAME_SHORT}"
${def} SCHAT_PAGE_OPTIONS  1
${def} SCHAT_PLUGINS_GRP   1
${def} SCHAT_CHECK_RUN     1
${def} SCHAT_VC100         1
${def} SCHAT_DEFAULT_CONF  0

${def} OPT_DESKTOP         0
${def} OPT_QUICKLAUNCH     1
${def} OPT_ALLPROGRAMS     1
${def} OPT_AUTOSTART       1
${def} OPT_AUTODAEMONSTART 0

${def} SCHAT_SOURCE        "schat2"

!endif /* DEFAULT_NSH_ */
