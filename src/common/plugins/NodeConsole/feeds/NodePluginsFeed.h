/* $Id: NodePluginsFeed.h 3420 2013-01-19 05:23:36Z IMPOMEZIA $
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

#ifndef NODEPLUGINSFEED_H_
#define NODEPLUGINSFEED_H_

#include "feeds/Feed.h"
#include "feeds/FeedCreator.h"

/*!
 * Серверная реализация фида \b plugins.
 */
class NodePluginsFeed : public Feed
{
public:
  NodePluginsFeed(const QString &name, const QVariantMap &data);
  NodePluginsFeed(const QString &name = FEED_NAME_PLUGINS, qint64 date = 0);

private:
  void init();
};


class NodePluginsFeedCreator : public FeedCreator
{
public:
  Feed* create(const QString &name) const;
  Feed* load(const QString &name, const QVariantMap &data) const;
  inline QString name() const { return FEED_NAME_PLUGINS; }
};

#endif /* NODEPLUGINSFEED_H_ */
