/* $Id: PopupPlugin.h 3644 2013-04-17 13:17:34Z IMPOMEZIA $
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

#ifndef POPUPPLUGIN_H_
#define POPUPPLUGIN_H_

#include "ChatApi.h"
#include "CoreApi.h"

class PopupPlugin : public QObject, CoreApi, ChatApi
{
  Q_OBJECT
  Q_INTERFACES(CoreApi ChatApi)

# if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "me.schat.client.Popup" FILE "Popup.json")
# endif

public:
  QVariantMap header() const
  {
    QVariantMap out        = CoreApi::header();
    out[CORE_API_ID]       = "Popup";
    out[CORE_API_NAME]     = "Popup";
    out[CORE_API_VERSION]  = "0.1.5";
    out[CORE_API_SITE]     = "http://wiki.schat.me/Plugin/Popup";
    out[CORE_API_DESC]     = "Adds support for popup notifications";
    out[CORE_API_DESC_RU]  = "Добавляет поддержку всплывающих уведомлений";

    return out;
  }

  ChatPlugin *create();
};

#endif /* POPUPPLUGIN_H_ */
