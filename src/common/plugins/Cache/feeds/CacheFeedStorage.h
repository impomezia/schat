/* $Id: CacheFeedStorage.h 3201 2012-10-20 04:22:05Z IMPOMEZIA $
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

#ifndef CACHEFEEDSTORAGE_H_
#define CACHEFEEDSTORAGE_H_

#include "feeds/FeedStorage.h"

class CacheFeedStorage : public FeedStorage
{
  Q_OBJECT

public:
  CacheFeedStorage(QObject *parent = 0);
  int saveImpl(FeedPtr feed, qint64 date);
  void loadImpl(Channel *channel);
  void removeImpl(FeedPtr feed);
};

#endif /* CACHEFEEDSTORAGE_H_ */
