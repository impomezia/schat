/* $Id: NodeStorageFeed.cpp 3735 2013-07-07 23:38:33Z IMPOMEZIA $
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

#include "DateTime.h"
#include "feeds/NodeStorageFeed.h"
#include "net/packets/Notice.h"
#include "Storage.h"
#include "sglobal.h"

NodeStorageFeed::NodeStorageFeed(const QString &name, const QVariantMap &data)
  : Feed(name, data)
{
  m_header.acl().setMask(0);
}


NodeStorageFeed::NodeStorageFeed(const QString &name, qint64 date)
  : Feed(name, date)
{
  m_header.acl().setMask(0);
}


FeedReply NodeStorageFeed::get(const QString &path, const QVariantMap &json, Channel *channel, const QByteArray &blob) const
{
  Q_UNUSED(json)
  Q_UNUSED(blob)

  if (path.isEmpty())
    return Notice::BadRequest;

  else if (path == LS("head"))
    return FeedReply(Notice::OK, head().get(channel), head().date());

  else if (!Storage::contains(path))
    return Notice::NotFound;

  QVariantMap data;
  data[LS("value")] = Storage::value(path);

  return FeedReply(Notice::OK, data, head().date());
}


/*!
 * Обработка \b post запроса к фиду.
 */
FeedReply NodeStorageFeed::post(const QString &path, const QVariantMap &json, Channel *channel, const QByteArray &blob)
{
  Q_UNUSED(channel)
  Q_UNUSED(blob)

  if (path.isEmpty() || path.contains(LC('*')) || !json.contains(LS("value")))
    return Notice::BadRequest;

  const QVariant& value = json[LS("value")];

  if (path.startsWith(LS("head/")))
    return Feed::post(path, json, channel);

  int status = Storage::setValue(path, value);
  if (status == Notice::OK)
    return FeedReply(status, DateTime::utc());

  return status;
}


/*!
 * Обработка \b put запроса к фиду.
 */
FeedReply NodeStorageFeed::put(const QString &path, const QVariantMap &json, Channel *channel, const QByteArray &blob)
{
  Q_UNUSED(channel)
  Q_UNUSED(blob)

  if (path.isEmpty() || path.contains(LC('*')) || !json.contains(LS("value")))
    return Notice::BadRequest;

  const QVariant& value = json[LS("value")];

  if (path.startsWith(LS("head/")))
    return Feed::put(path, json, channel);

  if (!Storage::contains(path))
    return Notice::NotFound;

  int status = Storage::setValue(path, value);
  if (status == Notice::OK)
    return FeedReply(status, DateTime::utc());

  return status;
}


Feed* NodeStorageFeedCreator::create(const QString &name) const
{
  return new NodeStorageFeed(name, DateTime::utc());
}


Feed* NodeStorageFeedCreator::load(const QString &name, const QVariantMap &data) const
{
  return new NodeStorageFeed(name, data);
}
