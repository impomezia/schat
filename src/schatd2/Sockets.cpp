/* $Id: Sockets.cpp 3638 2013-04-15 11:09:48Z IMPOMEZIA $
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

#include "Ch.h"
#include "Sockets.h"
#include "Storage.h"


QList<quint64> Sockets::all(ChatChannel channel, ChatChannel user, bool echo)
{
  QList<quint64> sockets;
  if (channel->type() == SimpleID::UserId)
    sockets = all(channel);
  else
    sockets = Sockets::channel(channel);

  echoFilter(user, sockets, echo);
  return sockets;
}


/*!
 * Получение списка всех сокетов для пользователя.
 *
 * \param user пользователь.
 * \param echo \b true если необходимо включить в список сокеты пользователя.
 */
QList<quint64> Sockets::all(ChatChannel user, bool echo)
{
  QList<quint64> out;
  if (!user)
    return out;

  if (echo)
    out = user->sockets();

  QList<QByteArray> channels = user->channels().all();
  for (int i = 0; i < channels.size(); ++i) {
    ChatChannel channel = Ch::channel(channels.at(i), SimpleID::typeOf(channels.at(i)));
    if (!channel)
      continue;

    if (channel->type() == SimpleID::UserId)
      merge(out, channel->sockets());
    else
      merge(out, Sockets::channel(channel));
  }

  return out;
}


/*!
 * Получение списка всех сокетов пользователей в обычном канале.
 */
QList<quint64> Sockets::channel(ChatChannel channel)
{
  if (!channel)
    return QList<quint64>();

  return Sockets::channel(channel.data());
}


QList<quint64> Sockets::channel(ServerChannel *channel)
{
  QList<quint64> out;
  if (!channel)
    return out;

  QList<QByteArray> channels = channel->channels().all();
  channels.append(channel->offline().all());

  for (int i = 0; i < channels.size(); ++i) {
    ChatChannel channel = Ch::channel(channels.at(i), SimpleID::UserId, false);
    if (!channel)
      continue;

    merge(out, channel->sockets());
  }

  return out;
}


/*!
 * Фильтрация списка сокетов.
 */
void Sockets::echoFilter(ChatChannel channel, QList<quint64> &sockets, bool echo)
{
  if (!channel)
    return;

  if (echo) {
    merge(sockets, channel->sockets());
    return;
  }

  foreach (quint64 socket, channel->sockets()) {
    sockets.removeAll(socket);
  }
}


/*!
 * Слияние двух списков недопуская появления дубликатов.
 */
void Sockets::merge(QList<quint64> &out, const QList<quint64> &sockets)
{
  foreach (quint64 socket, sockets) {
    if (!out.contains(socket))
      out.append(socket);
  }
}
