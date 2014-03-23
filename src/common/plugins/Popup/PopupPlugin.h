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

#ifndef POPUPPLUGIN_H_
#define POPUPPLUGIN_H_

#include "ChatApi.h"
#include "CoreApi.h"

class PopupPlugin : public QObject, CoreApi, ChatApi
{
  Q_OBJECT
  Q_INTERFACES(CoreApi ChatApi)
  Q_PLUGIN_METADATA(IID "me.schat.client.Popup")

public:
  QVariantMap header() const Q_DECL_OVERRIDE
  {
    QVariantMap out        = CoreApi::header();
    out[CORE_API_ID]       = "Popup";
    out[CORE_API_NAME]     = "Popup";
    out[CORE_API_VERSION]  = "2.3.0";
    out[CORE_API_SITE]     = "https://wiki.schat.me/Plugin/Popup";
    out[CORE_API_DESC]     = "Adds support for popup notifications";
    out[CORE_API_DESC_RU]  = "Добавляет поддержку всплывающих уведомлений";

    return out;
  }

  ChatPlugin *create() Q_DECL_OVERRIDE;
};

#endif /* POPUPPLUGIN_H_ */
