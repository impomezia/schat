/* $Id: LiveAuthData.h 2954 2012-07-31 23:07:24Z IMPOMEZIA $
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

#ifndef LIVEAUTHDATA_H_
#define LIVEAUTHDATA_H_

#include "oauth2/OAuthData.h"

class LiveAuthData : public OAuthData
{
public:
  LiveAuthData();

protected:
  QByteArray toUrl(const QByteArray &state) const;
};

#endif /* LIVEAUTHDATA_H_ */
