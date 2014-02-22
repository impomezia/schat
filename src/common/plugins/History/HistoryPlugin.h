/* $Id: HistoryPlugin.h 3841 2013-10-27 00:22:25Z IMPOMEZIA $
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
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef HISTORYPLUGIN_H_
#define HISTORYPLUGIN_H_

#include "ChatApi.h"
#include "CoreApi.h"

class ChatPlugin;

class HistoryPlugin : public QObject, CoreApi, ChatApi
{
  Q_OBJECT
  Q_INTERFACES(CoreApi ChatApi)

# if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "me.schat.client.History" FILE "History.json")
# endif

public:
  QVariantMap header() const
  {
    QVariantMap out            = CoreApi::header();
    out[CORE_API_ID]           = "History";
    out[CORE_API_NAME]         = "History";
    out[CORE_API_VERSION]      = "0.5.1";
    out[CORE_API_SITE]         = "https://wiki.schat.me/Plugin/History";
    out[CORE_API_DESC]         = "Adds support for access to the message history";
    out[CORE_API_DESC_RU]      = "Добавляет поддержку доступа к истории сообщений";
    out[CORE_API_CONFIGURABLE] = true;

    return out;
  }

  ChatPlugin *create();
  QWidget *settings(QWidget *parent);
};

#endif /* HISTORYPLUGIN_H_ */
