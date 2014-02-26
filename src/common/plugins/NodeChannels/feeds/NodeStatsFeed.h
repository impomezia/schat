/* Simple Chat
 * Copyright (c) 2008-2014 Alexander Sedov <imp@schat.me>
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

#ifndef NODESTATSFEED_H_
#define NODESTATSFEED_H_

#include "feeds/Feed.h"
#include "feeds/FeedCreator.h"

/*!
 * Серверная реализация фида \b stats.
 */
class NodeStatsFeed : public Feed
{
public:
  NodeStatsFeed(const QString &name, const QVariantMap &data);
  NodeStatsFeed(const QString &name = FEED_NAME_STATS, qint64 date = 0);

  FeedReply del(const QString &path, Channel *user = 0, const QByteArray &blob = QByteArray());
  FeedReply post(const QString &path, const QVariantMap &json, Channel *user = 0, const QByteArray &blob = QByteArray());
  FeedReply put(const QString &path, const QVariantMap &json, Channel *user = 0, const QByteArray &blob = QByteArray());
  void setChannel(Channel *channel);

private:
  void init();
  qint64 rooms();
};


class NodeStatsFeedCreator : public FeedCreator
{
public:
  Feed* create(const QString &name) const;
  Feed* load(const QString &name, const QVariantMap &data) const;
  inline QString name() const { return FEED_NAME_STATS; }
};

#endif /* NODESTATSFEED_H_ */
