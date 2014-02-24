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

#include "acl/AclValue.h"
#include "Ch.h"
#include "Channel.h"
#include "cores/Core.h"
#include "DateTime.h"
#include "feeds/AclFeed.h"
#include "feeds/FeedsCore.h"
#include "feeds/InfoFeed.h"
#include "feeds/NodeAclFeed.h"
#include "JSON.h"
#include "net/packets/MessageNotice.h"
#include "net/packets/Notice.h"
#include "net/SimpleID.h"
#include "sglobal.h"

NodeAclFeed::NodeAclFeed(const QString &name, const QVariantMap &data)
  : Feed(name, data)
{
  init();
}


NodeAclFeed::NodeAclFeed(const QString &name, qint64 date)
  : Feed(name, date)
{
  init();
}


/*!
 * Переопределение запросов "delete acl/owner/<id>" и "delete acl/other/<id>".
 */
FeedReply NodeAclFeed::del(const QString &path, Channel *channel, const QByteArray &blob)
{
  Q_UNUSED(blob)

  if (!path.startsWith(LS("head/")))
    return Notice::Forbidden;

  const int acl = AclValue::match(this, channel);
  FeedReply reply(Notice::Forbidden);

  if (acl & (Acl::Edit | Acl::SpecialEdit)) {
    reply = Feed::del(path, channel);
  }
  else if (acl & Acl::SpecialWrite) {
    const QByteArray id = SimpleID::decode(path.mid(11));
    if (SimpleID::typeOf(id) != SimpleID::UserId)
      return Notice::BadRequest;

    if (isGenericUser(id)) {
      reply.status = head().del(path.mid(5));
      if (reply.status != Notice::OK)
        return reply;
    }
  }

  if (reply.status == Notice::OK) {
    m_data.remove(path.mid(11));
    reply.date = DateTime::utc();
  }

  return reply;
}


/*!
 * Переопределение запросов "post acl/head/owner" или "post acl/head/other/<id>".
 */
FeedReply NodeAclFeed::post(const QString &path, const QVariantMap &json, Channel *channel, const QByteArray &blob)
{
  Q_UNUSED(blob)

  if (!path.startsWith(LS("head/"))) {
    if (path == ACL_FEED_INVITE_KEY)
      return invite(json, channel);
    else if (path == ACL_FEED_KICK_KEY)
      return kick(json, channel);

    return Notice::Forbidden;
  }

  const int acl = AclValue::match(this, channel);
  FeedReply reply(Notice::Forbidden);

  if (acl & (Acl::Edit | Acl::SpecialEdit) || path == LS("head/owner")) {
    reply = Feed::post(path, json, channel);
  }
  else if ((acl & Acl::Write) && path.startsWith(LS("head/other/"))) {
    reply.status = setAcl(path.mid(11), json.value(FEED_KEY_VALUE).toInt(), !(acl & Acl::SpecialWrite));
  }

  if (reply.status == Notice::OK) {
    if (path == LS("head/owner"))
      m_data[json.value(FEED_KEY_VALUE).toString()] = AclValue::toByteArray(head().acl().mask() >> 6 | Acl::SpecialEdit);
    else
      m_data[path.mid(11)] = AclValue::toByteArray(json.value(FEED_KEY_VALUE).toInt());

    reply.date = DateTime::utc();
  }

  return reply;
}


/*!
 * Переопределение запроса "put acl/head/mask" для установки прав доступа для пользователей
 * права которых не установлены явно.
 */
FeedReply NodeAclFeed::put(const QString &path, const QVariantMap &json, Channel *channel, const QByteArray &blob)
{
  Q_UNUSED(blob)

  if (path.startsWith(LS("head/"))) {
    const FeedReply reply = Feed::put(path, json, channel);
    if (reply.status == Notice::OK) {
      const int acl = (head().acl().mask() & ~0770);
      m_data[FEED_WILDCARD_ASTERISK] = AclValue::toByteArray(acl);

      head().channel()->gender().setColor(acl ? Gender::Default : Gender::Green);
    }

    return reply;
  }

  return Notice::Forbidden;
}


/*!
 * Переопределение установки канала для записи информации о владельце канала.
 */
void NodeAclFeed::setChannel(Channel *channel)
{
  Feed::setChannel(channel);

  if (!channel)
    return;

  const int type = channel->type();
  if (type == SimpleID::UserId)
    m_data[SimpleID::encode(channel->id())] = AclValue::toByteArray(head().acl().mask() >> 6 | Acl::SpecialEdit);

  if (type == SimpleID::ServerId)
    FeedsCore::sub(FEED_NAME_ACL);

  if (type == SimpleID::ChannelId)
    head().channel()->gender().setColor(head().acl().mask() & ~0770 ? Gender::Default : Gender::Green);
}


/*!
 * Возвращает \b true если пользователь не является владельцем или модератором.
 */
bool NodeAclFeed::isGenericUser(const QByteArray &id) const
{
  ChatChannel user = Ch::channel(id, SimpleID::UserId);
  if (!user)
    return true;

  const int acl = AclValue::match(this, user.data());
  Ch::gc(user);

  if (acl & (Acl::Edit | Acl::SpecialEdit))
    return false;

  if (acl & (Acl::SpecialWrite))
    return false;

  return true;
}


/*!
 * Приглашение пользователя в канал.
 */
FeedReply NodeAclFeed::invite(const QVariantMap &json, Channel *channel)
{
  const QByteArray id = SimpleID::decode(json.value(FEED_KEY_VALUE).toString());
  if (!channel || SimpleID::typeOf(id) != SimpleID::UserId)
    return Notice::BadRequest;

  if (head().channel()->channels().contains(id))
    return Notice::NotModified;

  ChatChannel user = Ch::channel(id, SimpleID::UserId);
  if (!user)
    return Notice::NotFound;

  if (user->status() == Status::Offline)
    return Notice::ChannelOffline;

  const int acl = AclValue::match(this, user.data());
  if (!(acl & Acl::Write)) {
    const QString request = ACL_FEED_HEAD_OTHER_REQ + LC('/') + json.value(FEED_KEY_VALUE).toString();
    const int status      = FeedsCore::post(static_cast<ServerChannel*>(head().channel()), request, static_cast<ServerChannel*>(channel), Acl::Read | Acl::Write, Feed::Share | Feed::Broadcast).status;
    if (status != Notice::OK && !(AclValue::match(this, user.data()) & Acl::Read))
      return status;
  }

  QVariantMap data;
  data[LS("id")]   = SimpleID::encode(head().channel()->id());
  data[LS("name")] = head().channel()->name();

  /// \deprecated Этот способ приглашения в канал является устаревшим.
  MessagePacket packet(new MessageNotice(channel->id(), user->id(), JSON::generate(data), 0, ChatId(ChatId::MessageId).toByteArray()));
  packet->setCommand(LS("invite"));
  packet->setDirection(Notice::Internal);
  packet->setStatus(Notice::Found);

  Core::i()->send(user->sockets(), packet);
  return Notice::NotModified;
}


FeedReply NodeAclFeed::kick(const QVariantMap &json, Channel *user)
{
  const QByteArray id = SimpleID::decode(json.value(FEED_KEY_VALUE).toString());
  if (!user || SimpleID::typeOf(id) != SimpleID::UserId)
    return Notice::BadRequest;

  ServerChannel *channel = static_cast<ServerChannel*>(head().channel());
  if (!channel->channels().contains(id) && !channel->offline().contains(id))
    return Notice::NotFound;

  const int acl = AclValue::match(this, user);
  if ((acl & (Acl::Edit | Acl::SpecialEdit)) || (acl & Acl::SpecialWrite))
    return FeedReply(Notice::OK, DateTime::utc());

  return Notice::Forbidden;
}


/*!
 * Установка прав доступа.
 */
int NodeAclFeed::setAcl(const QString &encodedId, int acl, bool sudo)
{
  const QByteArray id = SimpleID::decode(encodedId);
  if (SimpleID::typeOf(id) != SimpleID::UserId)
    return Notice::BadRequest;

  if (sudo) {
    if (head().channel()->type() != SimpleID::ChannelId)
      return Notice::Forbidden;

    FeedPtr feed = head().channel()->feed(FEED_NAME_INFO, false);
    if (!feed || !feed->data().value(INFO_FEED_SUDO_KEY).toBool())
      return Notice::Forbidden;

    const QString asterisk = m_data.value(FEED_WILDCARD_ASTERISK).toString();
    if (!((asterisk == LS("---") || asterisk == LS("r--")) && !m_data.contains(encodedId)))
      return Notice::Forbidden;
  }

  if (isGenericUser(id))
    return head().post(LS("other/") + encodedId, acl, 0);

  return Notice::Forbidden;
}


/*!
 * Инициализация фида.
 */
void NodeAclFeed::init()
{
  if (!m_data.isEmpty())
    return;

  m_data[FEED_WILDCARD_ASTERISK] = AclValue::toByteArray(head().acl().mask() & ~0770);

  // Импортирование существующих прав доступа.
  const QVariantMap &data = head().data();
  const QVariantMap others = data.value(ACL_OTHERS).toMap();
  if (!others.isEmpty()) {
    QMapIterator<QString, QVariant> i(others);
    while (i.hasNext()) {
      i.next();
      m_data[i.key()] = AclValue::toByteArray(i.value().toInt());
    }
  }

  const QStringList owners = data.value(ACL_OWNERS).toStringList();
  if (!owners.isEmpty()) {
    const QString mask = AclValue::toByteArray(head().acl().mask() >> 6 | Acl::SpecialEdit);
    foreach (const QString &id, owners)
      m_data[id] = mask;
  }
}


Feed* NodeAclFeedCreator::create(const QString &name) const
{
  return new NodeAclFeed(name, DateTime::utc());
}


Feed* NodeAclFeedCreator::load(const QString &name, const QVariantMap &data) const
{
  return new NodeAclFeed(name, data);
}
