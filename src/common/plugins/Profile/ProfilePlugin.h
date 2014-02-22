/* $Id: ProfilePlugin.h 3756 2013-07-15 15:23:36Z IMPOMEZIA $
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

#ifndef PROFILEPLUGIN_H_
#define PROFILEPLUGIN_H_

#include "ChatApi.h"
#include "CoreApi.h"

class ProfilePlugin : public QObject, CoreApi, ChatApi
{
  Q_OBJECT
  Q_INTERFACES(CoreApi ChatApi)

# if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "me.schat.client.Profile" FILE "Profile.json")
# endif

public:
  QVariantMap header() const
  {
    QVariantMap out        = CoreApi::header();
    out[CORE_API_ID]       = "Profile";
    out[CORE_API_NAME]     = "Profile";
    out[CORE_API_VERSION]  = "0.2.6";
    out[CORE_API_SITE]     = "http://wiki.schat.me/Plugin/Profile";
    out[CORE_API_DESC]     = "Adds support for additional profile fields";
    out[CORE_API_DESC_RU]  = "Добавляет поддержку дополнительных полей профиля";

    return out;
  }

  ChatPlugin *create();
};

#endif /* PROFILEPLUGIN_H_ */
