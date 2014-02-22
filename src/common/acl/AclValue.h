/* $Id: AclValue.h 3430 2013-01-23 00:17:08Z IMPOMEZIA $
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

#ifndef ACLVALUE_H_
#define ACLVALUE_H_

#include <QVariant>

#include "schat.h"

class Channel;
class Feed;

class SCHAT_EXPORT AclValue
{
public:
  AclValue(const QByteArray &mask);
  AclValue(const QVariant &mask);
  AclValue(int mask = 0);
  QByteArray toByteArray() const;

  static int toInt(const QByteArray &mask);
  static int match(const Feed *feed, Channel *channel);
  static QByteArray toByteArray(int mask);

private:
  int m_mask;
};

#endif /* ACLVALUE_H_ */
