/* $Id: StackOverflowAuthData.h 3481 2013-02-09 11:44:24Z IMPOMEZIA $
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

#ifndef STACKOVERFLOWAUTHDATA_H_
#define STACKOVERFLOWAUTHDATA_H_

#include "oauth2/OAuthData.h"

class StackOverflowAuthData : public OAuthData
{
public:
  StackOverflowAuthData();

protected:
  QByteArray toUrl(const QByteArray &state) const;
};

#endif /* STACKOVERFLOWAUTHDATA_H_ */
