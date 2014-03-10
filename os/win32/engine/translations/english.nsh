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

!ifndef ENGLISH_NSH_
!define ENGLISH_NSH_

!insertmacro MUI_UNSET CURRENT_LANG
!define CURRENT_LANG "L_EN"

${L} STR100                 "Shortcuts:"
${L} STR101                 "Desktop"
${L} STR102                 "Quick Launch"
${L} STR103                 "Start Menu"
${L} STR104                 "Add to autostart"
${L} STR200                 "Select additional tasks"
${L} STR300                 "Uninstall"
${L} STR400                 "An instance of ${SCHAT_NAME} is currently running. Exit ${SCHAT_NAME} and then try again."
${L} STR201                 "What additional tasks needed for executing?"
${L} STR1004                "Emoticons"
${L} STR1005                "Plugins"
${L} sec_Core               "Core Components"
${L} desc_Core              "Core Components"
${L} sec_Qt                 "Qt Open Source Edition ${SCHAT_QT_VERSION}"
${L} desc_Qt                "Qt Open Source Edition ${SCHAT_QT_VERSION}"
${L} sec_Cache              "Cache"
${L} desc_Cache             "Improves performance and reduces network traffic"
${L} sec_History            "History"
${L} desc_History           "Adds support for access to the message history"
${L} sec_Profile            "Profile"
${L} desc_Profile           "Adds support for additional profile fields"
${L} sec_Popup              "Popup"
${L} desc_Popup             "Adds support for popup notifications"
${L} sec_Preview            "Preview"
${L} desc_Preview           "Image Preview"
${L} sec_Proxy              "Proxy"
${L} desc_Proxy             "Adds support for connections via HTTP and SOCKS5 proxy"
${L} sec_Console            "Console"
${L} desc_Console           "Server Management console"
${L} sec_Channels           "Channels"
${L} desc_Channels          "Advanced channels support"
${L} sec_Emoticons          "Emoticons"
${L} desc_Emoticons         "Adds support for graphical emoticons"
${L} sec_SendFile           "Send File"
${L} desc_SendFile          "Adds support for file transfer between users"
${L} sec_SpellChecker       "Spell Checker"
${L} desc_SpellChecker      "Automatic spell checking"
${L} sec_Idle               "Idle"
${L} desc_Idle              "Detect idle time to automatically set Away status"
${L} sec_Update             "Update"
${L} desc_Update            "Automatically downloads and installs updates"
${L} sec_RawFeeds           "Raw Feeds"
${L} desc_RawFeeds          "Debug plugin to access the raw data feeds"
${L} sec_YouTube            "YouTube"
${L} desc_YouTube           "Adds support for embedding YouTube videos in messages"

!endif /* ENGLISH_NSH_ */
