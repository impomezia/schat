/* $Id: LiveAuthData.cpp 2954 2012-07-31 23:07:24Z IMPOMEZIA $
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

#include <QUrl>

#include "oauth2/live/LiveAuthData.h"

LiveAuthData::LiveAuthData()
  : OAuthData("live")
{
  name = "Windows Live";
  htmlName = name;
}


QByteArray LiveAuthData::toUrl(const QByteArray &state) const
{
  return "https://login.live.com/oauth20_authorize.srf?client_id=" + id + "&scope=wl.basic,wl.birthday,wl.emails&response_type=code&redirect_uri=" + QUrl::toPercentEncoding(redirect) + "&state=" + state;
}
