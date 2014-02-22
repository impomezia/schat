/* $Id: NodeAclFeed.h 3735 2013-07-07 23:38:33Z IMPOMEZIA $
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

#ifndef NODEACLFEED_H_
#define NODEACLFEED_H_

#include "feeds/Feed.h"
#include "feeds/FeedCreator.h"

/*!
 * Серверная реализация фида \b acl.
 */
class SCHAT_EXPORT NodeAclFeed : public Feed
{
public:
  NodeAclFeed(const QString &name, const QVariantMap &data);
  NodeAclFeed(const QString &name = FEED_NAME_ACL, qint64 date = 0);

  FeedReply del(const QString &path, Channel *channel = 0, const QByteArray &blob = QByteArray());
  FeedReply post(const QString &path, const QVariantMap &json, Channel *channel = 0, const QByteArray &blob = QByteArray());
  FeedReply put(const QString &path, const QVariantMap &json, Channel *channel = 0, const QByteArray &blob = QByteArray());
  void setChannel(Channel *channel);

private:
  bool isGenericUser(const QByteArray &id) const;
  FeedReply invite(const QVariantMap &json, Channel *channel);
  FeedReply kick(const QVariantMap &json, Channel *user);
  int setAcl(const QString &encodedId, int acl, bool sudo = false);
  void init();
};


class SCHAT_EXPORT NodeAclFeedCreator : public FeedCreator
{
public:
  Feed* create(const QString &name) const;
  Feed* load(const QString &name, const QVariantMap &data) const;
  inline QString name() const { return FEED_NAME_ACL; }
};

#endif /* NODEACLFEED_H_ */
