/* $Id: ProxyPlugin.h 3408 2013-01-16 12:40:01Z IMPOMEZIA $
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
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PROXYPLUGIN_H_
#define PROXYPLUGIN_H_

#include "ChatApi.h"
#include "CoreApi.h"

class ProxyPlugin : public QObject, CoreApi, ChatApi
{
  Q_OBJECT
  Q_INTERFACES(CoreApi ChatApi)

# if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "me.schat.client.Proxy" FILE "Proxy.json")
# endif

public:
  QVariantMap header() const
  {
    QVariantMap out        = CoreApi::header();
    out[CORE_API_ID]       = "Proxy";
    out[CORE_API_NAME]     = "Proxy";
    out[CORE_API_VERSION]  = "0.1.0";
    out[CORE_API_SITE]     = "http://wiki.schat.me/Plugin/Proxy";
    out[CORE_API_DESC]     = "Adds support for connections via HTTP and SOCKS5 proxy";
    out[CORE_API_DESC_RU]  = "Добавляет поддержку подключения через HTTP и SOCKS5 прокси";
    out[CORE_API_ENABLED]  = false;

    return out;
  }

  ChatPlugin *create();
};

#endif /* PROXYPLUGIN_H_ */
