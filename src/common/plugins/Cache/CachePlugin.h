/* $Id: CachePlugin.h 3692 2013-06-14 18:14:42Z IMPOMEZIA $
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

#ifndef CACHEPLUGIN_H_
#define CACHEPLUGIN_H_

#include "ChatApi.h"
#include "CoreApi.h"

class CachePlugin : public QObject, CoreApi, ChatApi
{
  Q_OBJECT
  Q_INTERFACES(CoreApi ChatApi)

# if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "me.schat.client.Cache" FILE "Cache.json")
# endif

public:
  QVariantMap header() const
  {
    QVariantMap out        = CoreApi::header();
    out[CORE_API_ID]       = "Cache";
    out[CORE_API_NAME]     = "Cache";
    out[CORE_API_VERSION]  = "0.4.4";
    out[CORE_API_SITE]     = "http://wiki.schat.me/Plugin/Cache";
    out[CORE_API_DESC]     = "Improves performance and reduces network traffic";
    out[CORE_API_DESC_RU]  = "Повышает производительность и уменьшает сетевой трафик";

    return out;
  }

  ChatPlugin *create();
};

#endif /* CACHEPLUGIN_H_ */
