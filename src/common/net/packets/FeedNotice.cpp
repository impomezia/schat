/* $Id: FeedNotice.cpp 3206 2012-10-21 03:21:54Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2012 IMPOMEZIA <schat@impomezia.com>
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
#include "feeds/Feed.h"
#include "net/packets/FeedNotice.h"
#include "sglobal.h"

FeedNotice::FeedNotice()
  : Notice(QByteArray(), QByteArray(), QString())
{
  m_type = FeedType;
}


FeedNotice::FeedNotice(const QByteArray &sender, const QByteArray &dest, const QString &command, const QByteArray &id)
  : Notice(sender, dest, command, DateTime::utc(), id)
{
  m_type = FeedType;
}


FeedNotice::FeedNotice(quint16 type, PacketReader *reader)
  : Notice(type, reader)
{
}


FeedPacket FeedNotice::reply(const FeedNotice &source, const QVariantMap &json)
{
  FeedPacket packet(new FeedNotice(source.dest(), source.sender(), source.command()));
  packet->setDirection(Server2Client);
  packet->setText(source.text());
  packet->setData(json);
  return packet;
}


/*!
 * Базовая функция формирования ответа за запрос клиента.
 *
 * \param source  Исходный пакет, полученный от клиента.
 * \param status  Код ответа на запрос.
 */
FeedPacket FeedNotice::reply(const FeedNotice &source, int status)
{
  FeedPacket packet(new FeedNotice(source.dest(), source.sender(), source.command()));
  packet->setDirection(Server2Client);
  packet->setText(source.text());
  packet->setStatus(status);
  return packet;
}


/*!
 * Универсальная функция запроса клиента к серверу.
 *
 * \param user    Идентификатор пользователя.
 * \param channel Идентификатор канала, к которому предназначен запрос.
 * \param command Команда.
 * \param name    Имя фида.
 * \param json    JSON данные запроса.
 */
FeedPacket FeedNotice::request(const QByteArray &user, const QByteArray &channel, const QString &command, const QString &name, const QVariantMap &json)
{
  FeedPacket packet(new FeedNotice(user, channel, command));
  packet->setText(name);
  packet->setData(json);

  return packet;
}


/*!
 * Разделение строки на имя фида и запрос.
 *
 * Например, строка "server/uptime" будет разбита на "server" и "uptime".
 * Запрос может быть пустым, если нужно получить тело фида.
 */
QPair<QString, QString> FeedNotice::split(const QString &text)
{
  QPair<QString, QString> pair;
  int index = text.indexOf(LC('/'));
  if (index != -1) {
    pair.first  = text.mid(0, index);
    pair.second = text.mid(index + 1);
  }
  else
    pair.first = text;

  return pair;
}

