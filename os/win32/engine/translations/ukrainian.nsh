/* $Id: ukrainian.nsh 3274 2012-11-17 19:06:18Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2012 IMPOMEZIA <schat@impomezia.com>
 * Ukrainian translation — Motsyo Gennadi <drool@altlinux.ru>
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

!ifndef UKRAINIAN_NSH_
!define UKRAINIAN_NSH_

!insertmacro MUI_UNSET CURRENT_LANG
!define CURRENT_LANG "L_UK"

${L} STR100                 "Ярлики:"
${L} STR101                 "Робочий стіл"
${L} STR102                 "Швидкий запуск"
${L} STR103                 "Меню $\"Всі програми$\""
${L} STR104                 "Додати в автостарт"
${L} STR200                 "Виберіть додаткові завдання"
${L} STR201                 "Які додаткові завдання необхідно виконати?"
${L} STR300                 "Видалення програми"
${L} STR400                 "Знайдено копію, що працює ${SCHAT_NAME}. Завершіть роботу ${SCHAT_NAME} та спробуйте знову."
${L} STR1004                "Смайліки"
${L} STR1005                "Плагіни"
${L} sec_Core               "Основні компоненти"
${L} desc_Core              "Основні компоненти ${SCHAT_NAME}"
${L} sec_Qt                 "Qt Open Source ${SCHAT_QT_VERSION}"
${L} desc_Qt                "Бібліотека Qt Open Source ${SCHAT_QT_VERSION}"
${L} sec_Cache              "Cache"
${L} desc_Cache             "Плагін Cache"
${L} sec_History            "History"
${L} desc_History           "Плагін History"
${L} sec_Profile            "Profile"
${L} desc_Profile           "Плагін Profile"
${L} sec_Popup              "Popup"
${L} desc_Popup             "Плагін Popup"
${L} sec_Proxy              "Proxy"
${L} sec_Console            "Console"
${L} desc_Console           "Плагін Console"
${L} sec_Channels           "Channels"
${L} desc_Channels          "Плагін Channels"
${L} desc_Proxy             "Плагін Proxy"
${L} sec_Emoticons          "Emoticons"
${L} desc_Emoticons         "Плагін Emoticons"
${L} sec_SendFile           "Send File"
${L} desc_SendFile          "Плагін Send File"
${L} sec_SpellChecker       "Spell Checker"
${L} desc_SpellChecker      "Плагін Spell Checker"
${L} sec_Idle               "Idle"
${L} desc_Idle              "Плагін Idle"
${L} sec_Update             "Update"
${L} desc_Update            "Плагін Update"
${L} sec_RawFeeds           "Raw Feeds"
${L} desc_RawFeeds          "Плагін Raw Feeds"
${L} sec_YouTube            "YouTube"
${L} desc_YouTube           "Плагін YouTube"

!endif /* UKRAINIAN_NSH_ */
