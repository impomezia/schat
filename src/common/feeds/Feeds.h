/* $Id: Feeds.h 2623 2012-05-08 02:37:23Z IMPOMEZIA $
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

#ifndef FEEDS_H_
#define FEEDS_H_

#include "feeds/Feed.h"

class Channel;

/*!
 * Хранилище фидов.
 */
class SCHAT_EXPORT Feeds
{
public:
  Feeds()
  : m_channel(0)
  {}

  bool add(FeedPtr feed, bool save = true);
  inline bool add(Feed *feed, bool save = true)    { return add(FeedPtr(feed), save); }
  inline const QMap<QString, FeedPtr>& all() const { return m_feeds; }
  inline void clear()                              { m_feeds.clear(); }
  inline void remove(const QString &name)          { m_feeds.remove(name); }
  inline void setChannel(Channel *channel)         { m_channel = channel; }
  QVariantMap f(Channel *channel) const;
  QVariantMap headers(Channel *channel) const;

private:
  Channel *m_channel;             ///< Канал фидов.
  QMap<QString, FeedPtr> m_feeds; ///< Таблица фидов.
};

#endif /* FEEDS_H_ */
