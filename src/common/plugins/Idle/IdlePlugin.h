/* $Id: IdlePlugin.h 3455 2013-02-01 22:06:52Z IMPOMEZIA $
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

#ifndef IDLEPLUGIN_H_
#define IDLEPLUGIN_H_

#include "ChatApi.h"
#include "CoreApi.h"

class IdlePlugin : public QObject, CoreApi, ChatApi
{
  Q_OBJECT
  Q_INTERFACES(CoreApi ChatApi)

# if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "me.schat.client.Idle" FILE "Idle.json")
# endif

public:
  QVariantMap header() const
  {
    QVariantMap out            = CoreApi::header();
    out[CORE_API_ID]           = "Idle";
    out[CORE_API_NAME]         = "Idle";
    out[CORE_API_VERSION]      = "0.2.7";
    out[CORE_API_SITE]         = "http://wiki.schat.me/Plugin/Idle";
    out[CORE_API_DESC]         = "Detect idle time to automatically set Away status";
    out[CORE_API_DESC_RU]      = "Обнаружение времени простоя для автоматической установки статуса Отсутствую";
    out[CORE_API_CONFIGURABLE] = true;

    return out;
  }

  ChatPlugin *create();
  QWidget *settings(QWidget *parent);
};

#endif /* IDLEPLUGIN_H_ */
