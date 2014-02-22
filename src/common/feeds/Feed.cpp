/* $Id: Feed.cpp 3735 2013-07-07 23:38:33Z IMPOMEZIA $
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

#include "Channel.h"
#include "acl/AclValue.h"
#include "DateTime.h"
#include "feeds/Feed.h"
#include "net/packets/Notice.h"
#include "net/SimpleID.h"
#include "sglobal.h"

/*!
 * Создание фида на основе JSON данных.
 *
 * \param name Имя фида.
 * \param data JSON данные.
 */
Feed::Feed(const QString &name, const QVariantMap &data)
{
  m_header.setName(name);
  m_header.setData(data.value(LS("head")).toMap());
  m_data = data;
  m_data.remove(LS("head"));
}


/*!
 * Создание пустого именованного фида.
 */
Feed::Feed(const QString &name, qint64 date)
{
  m_header.setName(name);
  m_header.data()[LS("date")] = date;
}


/*!
 * Возвращает \b true если данные фида корректны.
 */
bool Feed::isValid() const
{
  return m_header.isValid();
}


/*!
 * Обработка \b delete запроса к фиду.
 */
FeedReply Feed::del(const QString &path, Channel *channel, const QByteArray &blob)
{
  Q_UNUSED(channel)
  Q_UNUSED(blob)

  if (path.isEmpty())
    return Notice::BadRequest;

  if (path == FEED_WILDCARD_ASTERISK) {
    if (m_data.isEmpty())
      return Notice::NotModified;

    m_data.clear();
    return FeedReply(Notice::OK, DateTime::utc());
  }

  if (path.startsWith(LS("head/"))) {
    if (!can(channel, Acl::Edit | Acl::SpecialEdit))
      return Notice::Forbidden;

    const int status = head().del(path.mid(5));
    if (status == Notice::OK)
      return FeedReply(status, DateTime::utc());

    return status;
  }

  if (!m_data.contains(path))
    return Notice::NotFound;

  m_data.remove(path);
  return FeedReply(Notice::OK, DateTime::utc());
}


/*!
 * Обработка \b get запроса к фиду.
 *
 * \param path Путь запроса.
 * \param json Опциональные JSON данные запроса.
 * \param channel Пользователь совершивший запрос, если такой есть, функция не проверяет правда доступа.
 */
FeedReply Feed::get(const QString &path, const QVariantMap &json, Channel *channel, const QByteArray &blob) const
{
  Q_UNUSED(json)
  Q_UNUSED(blob)

  if (path.isEmpty())
    return Notice::BadRequest;

  else if (path == LS("head"))
    return FeedReply(Notice::OK, head().get(channel), head().date());

  else if (!m_data.contains(path))
    return Notice::NotFound;

  QVariantMap data;
  data[FEED_KEY_VALUE] = m_data.value(path);

  return FeedReply(Notice::OK, data, head().date());
}


/*!
 * Обработка \b post запроса к фиду.
 */
FeedReply Feed::post(const QString &path, const QVariantMap &json, Channel *channel, const QByteArray &blob)
{
  Q_UNUSED(channel)
  Q_UNUSED(blob)

  if (path.isEmpty() || path.contains(FEED_WILDCARD_ASTERISK) || !json.contains(FEED_KEY_VALUE))
    return Notice::BadRequest;

  const QVariant& value = json[FEED_KEY_VALUE];

  if (path.startsWith(LS("head/"))) {
    if (!can(channel, Acl::Edit | Acl::SpecialEdit))
      return Notice::Forbidden;

    const int status = head().post(path.mid(5), value);
    if (status == Notice::OK)
      return FeedReply(status, DateTime::utc());

    return status;
  }

  if (!m_data.contains(path) || m_data.value(path) != value) {
    m_data[path] = value;
    return FeedReply(Notice::OK, DateTime::utc());
  }

  return Notice::NotModified;
}


/*!
 * Обработка \b put запроса к фиду.
 */
FeedReply Feed::put(const QString &path, const QVariantMap &json, Channel *channel, const QByteArray &blob)
{
  Q_UNUSED(channel)
  Q_UNUSED(blob)

  if (path.isEmpty() || !json.contains(FEED_KEY_VALUE))
    return Notice::BadRequest;

  const QVariant& value = json[FEED_KEY_VALUE];

  if (path.startsWith(LS("head/"))) {
    if (!can(channel, Acl::Edit | Acl::SpecialEdit))
      return Notice::Forbidden;

    const int status = head().put(path.mid(5), value);
    if (status == Notice::OK)
      return FeedReply(status, DateTime::utc());

    return status;
  }

  if (!m_data.contains(path))
    return Notice::NotFound;

  if (m_data.value(path) != value) {
    m_data[path] = value;
    return FeedReply(Notice::OK, DateTime::utc());
  }

  return Notice::NotModified;
}


/*!
 * Получение тела фида.
 *
 * \param channel пользователь запросивший фид.
 */
QVariantMap Feed::feed(Channel *channel) const
{
  Q_UNUSED(channel)

  return m_data;
}


/*!
 * Получение полных JSON данных фида для сохранения в хранилище.
 */
QVariantMap Feed::save() const
{
  QVariantMap data = m_data;
  data[LS("head")] = m_header.save();
  return data;
}


bool Feed::can(Channel *channel, int acl) const
{
  return AclValue::match(this, channel) & acl;
}


void Feed::setChannel(Channel *channel)
{
  m_header.setChannel(channel);

  if (channel && channel->type() == SimpleID::UserId)
    m_header.acl().add(channel->id());
}


bool Feed::merge(const QString &key, QVariantMap &out, const QVariantMap &in)
{
  if (in.isEmpty())
    return false;

  out[key] = in;
  return true;
}


QVariantMap Feed::merge(const QString &key, const QVariantMap &in)
{
  QVariantMap out;
  if (!in.isEmpty())
    out[key] = in;

  return out;
}


/*!
 * Слияние данных фидов.
 */
void Feed::merge(QVariantMap &out, const QVariantMap &in)
{
  QMapIterator<QString, QVariant> i(in);
  while (i.hasNext()) {
    i.next();
    out[i.key()] = i.value();
  }
}
