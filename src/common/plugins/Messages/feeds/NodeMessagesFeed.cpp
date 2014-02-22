/* $Id: NodeMessagesFeed.cpp 3755 2013-07-14 23:11:47Z IMPOMEZIA $
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

#include "cores/Core.h"
#include "DateTime.h"
#include "feeds/MessagesFeed.h"
#include "feeds/NodeMessagesFeed.h"
#include "net/packets/Notice.h"
#include "net/SimpleID.h"
#include "NodeMessagesDB.h"
#include "ServerChannel.h"
#include "sglobal.h"
#include "Storage.h"

const QString NodeMessagesFeed::kEditable = QLatin1String("Messages/Editable");
const QString NodeMessagesFeed::kTimeOut  = QLatin1String("Messages/TimeOut");

NodeMessagesFeed::NodeMessagesFeed(const QString &name, const QVariantMap &data)
  : Feed(name, data)
{
  m_header.acl().setMask(0666);
}


NodeMessagesFeed::NodeMessagesFeed(const QString &name, qint64 date)
  : Feed(name, date)
{
  m_header.acl().setMask(0666);
}


FeedReply NodeMessagesFeed::del(const QString &path, Channel *channel, const QByteArray &blob)
{
  Q_UNUSED(blob)

  if (head().channel()->type() == ChatId::ServerId)
    return Notice::BadRequest;

  if (!channel || feed(channel).value(MESSAGES_FEED_EDITABLE_KEY).toInt() == NoEdit)
    return Notice::BadRequest;

  ChatId id(path);
  if (id.type() == ChatId::MessageId) {
    int status = Notice::OK;
    MessageRecordV2 record = fetch(id, channel, status);
    if (status != Notice::OK)
      return status;

    if (!(permissions(record, channel) & Remove))
      return Notice::Forbidden;

    FeedReply reply(Notice::OK, DateTime::utc());
    if (id.hasOid()) {
      record.mdate  = reply.date;
      record.status = MessageNotice::Removed;
      record.text.clear();
      record.data.clear();
      record.blob.clear();
      NodeMessagesDB::update(record);

      return reply;
    }
    else
      NodeMessagesDB::remove(record.id);

    return reply;
  }

  return Notice::NotImplemented;
}


FeedReply NodeMessagesFeed::get(const QString &path, const QVariantMap &json, Channel *channel, const QByteArray &blob) const
{
  Q_UNUSED(blob);

  if (head().channel()->type() == ChatId::ServerId)
    return Notice::BadRequest;

  if (path == MESSAGES_FEED_FETCH_KEY)
    return fetch(json, channel);
  else if (path == MESSAGES_FEED_LAST_KEY)
    return last(json, channel);
  else if (path == MESSAGES_FEED_OFFLINE_KEY)
    return offline(channel);
  else if (path == MESSAGES_FEED_SINCE_KEY)
    return since(json, channel);
  else if (path == MESSAGES_FEED_LOGGING_KEY)
    return logging();

  return Notice::NotImplemented;
}


FeedReply NodeMessagesFeed::post(const QString &path, const QVariantMap &json, Channel *channel, const QByteArray &blob)
{
  Q_UNUSED(path)
  Q_UNUSED(json)
  Q_UNUSED(channel)
  Q_UNUSED(blob)

  return Notice::NotImplemented;
}


FeedReply NodeMessagesFeed::put(const QString &path, const QVariantMap &json, Channel *channel, const QByteArray &blob)
{
  Q_UNUSED(path)
  Q_UNUSED(json)
  Q_UNUSED(channel)
  Q_UNUSED(blob)

  return Notice::NotImplemented;
}


QVariantMap NodeMessagesFeed::feed(Channel *channel) const
{
  Q_UNUSED(channel);
  QVariantMap out;
  out.insert(MESSAGES_FEED_EDITABLE_KEY, Storage::value(kEditable, DefaultEditFlags));
  out.insert(MESSAGES_FEED_TIMEOUT_KEY,  Storage::value(kTimeOut,  DefaultTimeOut));

  return out;
}


bool NodeMessagesFeed::isTimeOut(qint64 date) const
{
  return (qAbs(DateTime::utc() - date) / 1000) > feed().value(MESSAGES_FEED_TIMEOUT_KEY).toInt();
}


/*!
 * Загрузка сообщений по идентификаторам.
 */
FeedReply NodeMessagesFeed::fetch(const QVariantMap &json, Channel *user) const
{
  if (!user)
    return Notice::BadRequest;

  QList<ChatId> ids;

  if (json.contains(MESSAGES_FEED_MESSAGES_KEY)) {
    ChatId messageId;
    const QStringList messages = json.value(MESSAGES_FEED_MESSAGES_KEY).toStringList();

    foreach (const QString &id, messages) {
      messageId.init(id.toLatin1());
      if (messageId.type() == ChatId::MessageId)
        ids.append(messageId);
    }
  }

  if (ids.isEmpty())
    return Notice::BadRequest;

  const QList<MessageRecordV2> records = NodeMessagesDB::get(ids, head().channel()->type() == ChatId::UserId ? user->id() : ChatId());
  if (records.isEmpty())
    return FeedReply(Notice::NotFound);

  FeedReply reply(Notice::OK);
  toPackets(reply.packets, records, json.value(MESSAGES_FEED_V_KEY).toInt());

  reply.json[MESSAGES_FEED_COUNT_KEY] = reply.packets.size();
  return reply;
}


/*!
 * Запрос последних сообщений в канале.
 *
 * \param json JSON данные запроса, могут содержать значение "count" для определения количества сообщений, по умолчанию 20.
 * \param user Пользователь совершивший запрос.
 */
FeedReply NodeMessagesFeed::last(const QVariantMap &json, Channel *user) const
{
  const int count = json.value(MESSAGES_FEED_COUNT_KEY, 20).toInt();
  if (count <= 0)
    return Notice::BadRequest;

  qint64 before = json.value(MESSAGES_FEED_BEFORE_KEY, 0).toLongLong();
  QStringList messages;
  Channel *channel = head().channel();

  if (channel->type() == ChatId::ChannelId) {
    messages = MessageNotice::encode(NodeMessagesDB::last(channel->id(), count, before));
  }
  else if (channel->type() == ChatId::UserId) {
    if (!user)
      return Notice::BadRequest;

    messages = MessageNotice::encode(NodeMessagesDB::last(channel->id(), user->id(), count, before));
  }

  if (messages.isEmpty())
    return Notice::NotFound;

  if (json.contains(MESSAGES_FEED_TAG_KEY) && json.value(MESSAGES_FEED_TAG_KEY) == MessageNotice::toTag(messages))
    return Notice::NotModified;

  FeedReply reply(Notice::OK);
  reply.json = json;
  reply.json[MESSAGES_FEED_COUNT_KEY]    = messages.size();
  reply.json[MESSAGES_FEED_MESSAGES_KEY] = messages;
  return reply;
}


/*!
 * Возвращает Notice::OK если журналирование разрешено или Notice::Forbidden если запрещено.
 *
 * Журналирование всегда разрешено для каналов пользователей.
 */
FeedReply NodeMessagesFeed::logging() const
{
  Channel *channel = head().channel();
  if (channel->type() != SimpleID::ChannelId)
    return Notice::OK;

  FeedPtr feed = channel->feed(FEED_NAME_INFO, false, false);
  if (feed && !feed->data().value(MESSAGES_FEED_LOGGING_KEY, true).toBool())
    return Notice::Forbidden;

  return Notice::OK;
}


FeedReply NodeMessagesFeed::offline(Channel *user) const
{
  if (!user)
    return Notice::BadRequest;

  if (head().channel()->id() != user->id())
    return FeedReply(Notice::Forbidden);

  QList<MessageRecordV2> records = NodeMessagesDB::offline(user->id());
  if (records.isEmpty())
    return Notice::NotFound;

  NodeMessagesDB::markAsRead(records);

  FeedReply reply(Notice::OK);
  toPackets(reply.packets, records);

  reply.json[MESSAGES_FEED_COUNT_KEY] = reply.packets.size();
  return reply;
}


FeedReply NodeMessagesFeed::since(const QVariantMap &json, Channel *user) const
{
  const qint64 date = json.value(MESSAGES_FEED_DATE_KEY, 0).toLongLong();
  if (date <= 0)
    return Notice::BadRequest;

  qint64 end = json.value(MESSAGES_FEED_END_KEY, DateTime::utc()).toLongLong();
  QList<ChatId> messages;
  Channel *channel = head().channel();

  if (channel->type() == SimpleID::ChannelId) {
    messages = NodeMessagesDB::since(channel->id(), date, end);
  }
  else if (channel->type() == SimpleID::UserId) {
    if (!user)
      return Notice::BadRequest;

    messages = NodeMessagesDB::since(channel->id(), user->id(), date, end);
  }

  if (messages.isEmpty())
    return Notice::NotFound;

  FeedReply reply(Notice::OK);
  reply.json = json;
  reply.json[MESSAGES_FEED_COUNT_KEY]    = messages.size();
  reply.json[MESSAGES_FEED_MESSAGES_KEY] = MessageNotice::encode(messages);

  return reply;
}


int NodeMessagesFeed::permissions(const MessageRecordV2 &record, Channel *user) const
{
  const int flags    = feed(user).value(MESSAGES_FEED_EDITABLE_KEY).toInt();
  const bool timeout = isTimeOut(record.date);

  if (record.sender == ChatId(user->id()) && (flags & SelfEdit) && !timeout)
    return Remove | Modify;

  if (head().channel()->type() != ChatId::ChannelId || !head().channel()->feed(FEED_NAME_ACL)->can(user, Acl::SpecialWrite))
    return NoPermissions;

  int out = 0;
  if (flags & ModeratorRemove) out |= Remove;
  if (flags & ModeratorEdit)   out |= Modify;

  return out;
}


MessageRecordV2 NodeMessagesFeed::fetch(const ChatId &id, Channel *user, int &status) const
{
  const QList<MessageRecordV2> records = NodeMessagesDB::get(QList<ChatId>() << id, head().channel()->type() == ChatId::UserId ? user->id() : ChatId());
  if (records.isEmpty()) {
    status = Notice::NotFound; // Сообщение не найдено.
    return MessageRecordV2();
  }

  const MessageRecordV2 record = records.first();
  if (record.dest != ChatId(head().channel()->id()))
    status = Notice::BadRequest; // Сообщение не принадлежит каналу фида.

  return record;
}


void NodeMessagesFeed::toPackets(QList<QByteArray> &out, const QList<MessageRecordV2> &records, int version) const
{
  for (int i = 0; i < records.size(); ++i) {
    const MessageRecordV2& record = records.at(i);
    if (!record.id)
      continue;

    MessageNotice packet(record);
    if (!version && packet.status() >= Notice::UserStatus)
      packet.setStatus(Notice::Found);

    out.append(packet.data(Core::stream()));
  }
}


Feed* NodeMessagesFeedCreator::create(const QString &name) const
{
  return new NodeMessagesFeed(name, DateTime::utc());
}


Feed* NodeMessagesFeedCreator::load(const QString &name, const QVariantMap &data) const
{
  return new NodeMessagesFeed(name, data);
}
