/* Simple Chat
 * Copyright (c) 2008-2015 Alexander Sedov <imp@schat.me>
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

#ifndef NODEPROFILEFEED_H_
#define NODEPROFILEFEED_H_

#include "feeds/Feed.h"
#include "feeds/FeedCreator.h"

/*!
 * Серверная реализация фида \b profile.
 */
class NodeProfileFeed : public Feed
{
public:
  NodeProfileFeed(const QString &name, const QVariantMap &data);
  NodeProfileFeed(const QString &name = FEED_NAME_PROFILE, qint64 date = 0);

  FeedReply post(const QString &path, const QVariantMap &json, Channel *channel = 0, const QByteArray &blob = QByteArray()) Q_DECL_OVERRIDE;
  FeedReply put(const QString &path, const QVariantMap &json, Channel *channel = 0, const QByteArray &blob = QByteArray()) Q_DECL_OVERRIDE;
  QVariantMap feed(Channel *channel = 0) const Q_DECL_OVERRIDE;
};


class NodeProfileFeedCreator : public FeedCreator
{
public:
  Feed* create(const QString &name) const Q_DECL_OVERRIDE;
  Feed* load(const QString &name, const QVariantMap &data) const Q_DECL_OVERRIDE;
  QString name() const Q_DECL_OVERRIDE { return FEED_NAME_PROFILE; }
};

#endif /* NODEPROFILEFEED_H_ */
