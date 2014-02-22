/* $Id: OdnoklassnikiAuthData.cpp 3761 2013-08-01 08:16:32Z IMPOMEZIA $
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

#include <QUrl>

#include "oauth2/odnoklassniki/OdnoklassnikiAuthData.h"
#include "sglobal.h"

OdnoklassnikiAuthData::OdnoklassnikiAuthData()
  : OAuthData("odnoklassniki")
{
  name = "Одноклассники";
  htmlName = name;
}


QByteArray OdnoklassnikiAuthData::toUrl(const QByteArray &state) const
{
  QByteArray url = "http://www.odnoklassniki.ru/oauth/authorize?client_id=" + id + "&response_type=code&redirect_uri=" + QUrl::toPercentEncoding(redirect + LC('/') + state);
  return url.replace("%24%7BSTATE%7D", "${STATE}");
}
