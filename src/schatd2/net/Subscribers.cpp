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

#include "Ch.h"
#include "DateTime.h"
#include "net/Subscribers.h"
#include "Sockets.h"

Subscribers::Subscribers()
{
}


qint64 Subscribers::contains(const QString &path, const ChatId &id) const
{
  if (!m_map.contains(path))
    return 0;

  const Ids& ids = m_map[path];
  if (!ids.contains(id))
    return 0;

  return ids.value(id);
}


QList<quint64> Subscribers::sockets(const QString &path) const
{
  QList<quint64> out;
  if (!m_map.contains(path))
    return out;

  const Ids& ids = m_map[path];
  ChatChannel channel;

  QMapIterator<ChatId, qint64> i(ids);
  while (i.hasNext()) {
    i.next();
    channel = Ch::channel(i.key().toByteArray(), ChatId::UserId, false);
    if (channel)
      Sockets::merge(out, channel->sockets());
  }

  return out;
}


QVariantMap Subscribers::toData() const
{
  QVariantMap out;
  QMapIterator<QString, Ids> i(m_map);
  while (i.hasNext()) {
    i.next();

    QVariantMap item;
    QMapIterator<ChatId, qint64> k(i.value());
    while (k.hasNext()) {
      k.next();
      item.insert(k.key().toString(), k.value());
    }

    out.insert(i.key(), item);
  }

  return out;
}


void Subscribers::add(const QString &path, const ChatId &id)
{
  m_map[path].insert(id, DateTime::utc());
}
