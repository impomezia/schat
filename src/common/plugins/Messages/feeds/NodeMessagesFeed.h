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

#ifndef NODEMESSAGESFEED_H_
#define NODEMESSAGESFEED_H_

#include "feeds/Feed.h"
#include "feeds/FeedCreator.h"
#include "net/packets/MessageNotice.h"

/*!
 * Серверная реализация фида \b messages.
 */
class NodeMessagesFeed : public Feed
{
public:
  /// Опции редактирования сообщений.
  enum EditFlags {
    NoEdit          = 0, ///< Редактирование запрещено.
    SelfEdit        = 1, ///< Пользователь может редактировать и удалять свои сообщения.
    ModeratorRemove = 2, ///< Модераторы канала могут удалять сообщения.
    ModeratorEdit   = 4  ///< Модераторы канала могут редактировать сообщения.
  };

  /// Опции по умолчанию.
  enum Defaults {
    DefaultEditFlags = SelfEdit | ModeratorRemove, ///< Пользователь может редактировать и удалять свои сообщения, модераторы только удалять.
    DefaultTimeOut   = 3600                        ///< По прошествии 1 часа (3600 секунд) пользователь больше не может редактировать свои сообщения.
  };

  /// Права доступа для конкретного пользователя.
  enum Permissions {
    NoPermissions = 0,
    Remove        = 1,
    Modify        = 2
  };

  NodeMessagesFeed(const QString &name, const QVariantMap &data);
  NodeMessagesFeed(const QString &name = FEED_NAME_MESSAGES, qint64 date = 0);

  FeedReply del(const QString &path, Channel *channel = 0, const QByteArray &blob = QByteArray());
  FeedReply get(const QString &path, const QVariantMap &json = QVariantMap(), Channel *channel = 0, const QByteArray &blob = QByteArray()) const;
  FeedReply post(const QString &path, const QVariantMap &json, Channel *channel = 0, const QByteArray &blob = QByteArray());
  FeedReply put(const QString &path, const QVariantMap &json, Channel *channel = 0, const QByteArray &blob = QByteArray());
  QVariantMap feed(Channel *channel = 0) const;

private:
  static const QString kEditable;
  static const QString kTimeOut;

  bool isTimeOut(qint64 date) const;
  FeedReply fetch(const QVariantMap &json, Channel *user) const;
  FeedReply last(const QVariantMap &json, Channel *user) const;
  FeedReply logging() const;
  FeedReply offline(Channel *user) const;
  FeedReply since(const QVariantMap &json, Channel *user) const;
  int permissions(const MessageRecordV2 &record, Channel *user) const;
  MessageRecordV2 fetch(const ChatId &id, Channel *user, int &status) const;
  void toPackets(QList<QByteArray> &out, const QList<MessageRecordV2> &records, int version = 0) const;
};


class NodeMessagesFeedCreator : public FeedCreator
{
public:
  Feed* create(const QString &name) const;
  Feed* load(const QString &name, const QVariantMap &data) const;
  inline QString name() const { return FEED_NAME_MESSAGES; }
};

#endif /* NODEMESSAGESFEED_H_ */
