/* $Id: russian.nsh 3274 2012-11-17 19:06:18Z IMPOMEZIA $
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

!ifndef RUSSIAN_NSH_
!define RUSSIAN_NSH_

!insertmacro MUI_UNSET CURRENT_LANG
!define CURRENT_LANG "L_RU"

${L} STR100                 "Ярлыки:"
${L} STR101                 "Рабочий стол"
${L} STR102                 "Быстрый запуск"
${L} STR103                 "Меню $\"Все программы$\""
${L} STR104                 "Добавить в автозагрузку"
${L} STR200                 "Выберите дополнительные задачи"
${L} STR201                 "Какие дополнительные задачи необходимо выполнить?"
${L} STR300                 "Удаление программы"
${L} STR400                 "Обнаружена работающая копия ${SCHAT_NAME}. Завершите работу ${SCHAT_NAME} и попробуйте снова."
${L} STR1004                "Смайлики"
${L} STR1005                "Плагины"
${L} sec_Core               "Основные компоненты"
${L} desc_Core              "Основные компоненты ${SCHAT_NAME}"
${L} sec_Qt                 "Qt Open Source ${SCHAT_QT_VERSION}"
${L} desc_Qt                "Библиотека Qt Open Source ${SCHAT_QT_VERSION}"
${L} sec_Cache              "Cache"
${L} desc_Cache             "Повышает производительность и уменьшает сетевой трафик"
${L} sec_History            "History"
${L} desc_History           "Добавляет поддержку доступа к истории сообщений"
${L} sec_Profile            "Profile"
${L} desc_Profile           "Добавляет поддержку дополнительных полей профиля"
${L} sec_Popup              "Popup"
${L} desc_Popup             "Добавляет поддержку всплывающих уведомлений"
${L} sec_Proxy              "Proxy"
${L} desc_Proxy             "Добавляет поддержку подключения через HTTP и SOCKS5 прокси"
${L} sec_Console            "Console"
${L} desc_Console           "Консоль управления сервером"
${L} sec_Channels           "Channels"
${L} desc_Channels          "Расширенная поддержка каналов"
${L} sec_Emoticons          "Emoticons"
${L} desc_Emoticons         "Добавляет поддержку графических смайликов"
${L} sec_SendFile           "Send File"
${L} desc_SendFile          "Добавляет поддержку передачи файлов между пользователями"
${L} sec_SpellChecker       "Spell Checker"
${L} desc_SpellChecker      "Автоматическая проверка орфографии"
${L} sec_Idle               "Idle"
${L} desc_Idle              "Обнаружение времени простоя для автоматической установки статуса Отсутствую"
${L} sec_Update             "Update"
${L} desc_Update            "Автоматически загружает и устанавливает обновления"
${L} sec_RawFeeds           "Raw Feeds"
${L} desc_RawFeeds          "Отладочный плагин для доступа к данным фидов"
${L} sec_YouTube            "YouTube"
${L} desc_YouTube           "Добавляет поддержку встраивания видео c YouTube в сообщения"

!endif /* RUSSIAN_NSH_ */
