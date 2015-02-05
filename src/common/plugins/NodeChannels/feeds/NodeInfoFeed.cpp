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

#include "Ch.h"
#include "Channel.h"
#include "DateTime.h"
#include "feeds/InfoFeed.h"
#include "feeds/NodeInfoFeed.h"
#include "net/packets/Notice.h"
#include "net/SimpleID.h"
#include "sglobal.h"
#include "Storage.h"

NodeInfoFeed::NodeInfoFeed(const QString &name, const QVariantMap &data)
  : Feed(name, data)
{
  m_header.acl().setMask(0444);
}


NodeInfoFeed::NodeInfoFeed(const QString &name, qint64 date)
  : Feed(name, date)
{
  m_header.acl().setMask(0444);
}


/*!
 * Переопределение проверки прав доступа.
 *
 * Этот фид использует права доступа фида FEED_ACL.
 */
bool NodeInfoFeed::can(Channel *channel, int acl) const
{
  if (!channel && acl != Acl::Read)
    return false;

  FeedPtr feed = m_header.channel()->feed(FEED_NAME_ACL, false, false);
  if (feed)
    return feed->can(channel, acl);

  return Feed::can(channel, acl);
}


/*!
 * Переопределение запроса \b delete.
 *
 * Для этой операции требуются права на редактирование.
 */
FeedReply NodeInfoFeed::del(const QString &path, Channel *channel, const QByteArray &blob)
{
  Q_UNUSED(blob)

  const bool owner     = can(channel, Acl::Edit);
  const bool moderator = can(channel, Acl::SpecialWrite);
  if (!owner && !moderator)
    return Notice::Forbidden;

  if (path.startsWith(INFO_FEED_MOTD_KEY + LS("/"))) {
    ChatId id(path.mid(5));
    if (id.type() != ChatId::MessageId)
      return Notice::BadRequest;

    QVariantList list = m_data.value(INFO_FEED_MOTD_KEY).toList();
    list.removeAll(id.toString());

    m_data.insert(INFO_FEED_MOTD_KEY, list);
    return FeedReply(Notice::OK, DateTime::utc());
  }

  if (!owner)
    return Notice::Forbidden;

  return Feed::del(path, channel);
}


FeedReply NodeInfoFeed::post(const QString &path, const QVariantMap &json, Channel *channel, const QByteArray &blob)
{
  Q_UNUSED(blob)

  const bool owner     = can(channel, Acl::Edit);
  const bool moderator = can(channel, Acl::SpecialWrite);
  if (!owner && !moderator)
    return Notice::Forbidden;

  if (!channel || path.isEmpty() || path.contains(FEED_WILDCARD_ASTERISK) || !json.contains(FEED_KEY_VALUE))
    return Notice::BadRequest;

  const QVariant& value = json[FEED_KEY_VALUE];
  const qint64 date = DateTime::utc();

  if (path == INFO_FEED_MOTD_KEY) {
    ChatId id(value.toString());
    if (id.type() != ChatId::MessageId)
      return Notice::BadRequest;

    QVariantList list = m_data.value(path).toList();
    list.prepend(value.toString());

    m_data.insert(path, list);
    return FeedReply(Notice::OK, date);
  }

  if (!owner)
    return Notice::Forbidden;

  // Установка текстового заголовка канала.
  if (path == INFO_FEED_TITLE_KEY) {
    QVariantMap data;
    data[INFO_FEED_TEXT_KEY]   = value.toString().left(200);
    data[INFO_FEED_AUTHOR_KEY] = SimpleID::encode(channel->id());
    data[INFO_FEED_DATE_KEY]   = date;

    m_data[path] = data;
    return FeedReply(Notice::OK, date);
  }

  if (path == INFO_FEED_VISIBILITY_KEY) {
    visibility(value.toInt());
    return FeedReply(Notice::OK, date);
  }

  if (path == INFO_FEED_PINNED_KEY) {
    if (!Ch::server()->feed(FEED_NAME_ACL)->can(channel, Acl::Edit))
      return Notice::Forbidden;

    m_data[path] = value.toBool();
    return FeedReply(Notice::OK, date);
  }

  if (!m_data.contains(path) || m_data.value(path) != value) {
    m_data[path] = value;
    return FeedReply(Notice::OK, date);
  }

  return Notice::NotModified;
}


/*!
 * Переопределение запроса \b put.
 *
 * Этот фид не поддерживает такой запрос.
 */
FeedReply NodeInfoFeed::put(const QString &path, const QVariantMap &json, Channel *channel, const QByteArray &blob)
{
  Q_UNUSED(path)
  Q_UNUSED(json)
  Q_UNUSED(channel)
  Q_UNUSED(blob);
  return Notice::Forbidden;
}


/*!
 * Обновление настроек видимости канала.
 *
 * При необходимости обновляется опция "PermanentChannels" в хранилище, для обновления списка постоянных каналов.
 */
void NodeInfoFeed::visibility(int value)
{
  const QString key     = LS("PermanentChannels");
  const QString id      = SimpleID::encode(m_header.channel()->id());
  QStringList permanent = Storage::value(key).toStringList();
  bool contains         = permanent.contains(id);

  if (value > 0) {
    if (!contains) {
      permanent.append(id);
      Storage::setValue(key, permanent);
    }
  }
  else if (contains) {
    permanent.removeAll(id);
    Storage::setValue(key, permanent);
  }

  static_cast<ServerChannel *>(m_header.channel())->setPermanent(value > 0);
  m_data[INFO_FEED_VISIBILITY_KEY] = value;
}



Feed* NodeInfoFeedCreator::create(const QString &name) const
{
  return new NodeInfoFeed(name, DateTime::utc());
}


Feed* NodeInfoFeedCreator::load(const QString &name, const QVariantMap &data) const
{
  return new NodeInfoFeed(name, data);
}
