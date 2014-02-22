/* $Id: NodeFeedStorage.h 2918 2012-07-26 01:00:59Z IMPOMEZIA $
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

#ifndef NODEFEEDSTORAGE_H_
#define NODEFEEDSTORAGE_H_

#include "feeds/FeedStorage.h"

class NodeFeedStorage : public FeedStorage
{
  Q_OBJECT

public:
  NodeFeedStorage(QObject *parent = 0);

protected:
  int revertImpl(FeedPtr feed, const QVariantMap &data);
  int saveImpl(FeedPtr feed, qint64 date);
  void cloneImpl(FeedPtr feed);
  void loadImpl(Channel *channel);
  void removeImpl(FeedPtr feed);

private:
  qint64 rev(FeedPtr feed);
  qint64 save(FeedPtr feed, const QByteArray &json);
  qint64 update(FeedPtr feed, const QByteArray &json);
  void load(Channel *channel, const QString &name, qint64 id);
  void start();
  void updateKey(FeedPtr feed);
};

#endif /* NODEFEEDSTORAGE_H_ */
