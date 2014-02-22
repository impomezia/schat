/* $Id: events.h 2133 2012-01-12 05:55:51Z IMPOMEZIA $
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

#ifndef EVENTS_H_
#define EVENTS_H_

#include <QEvent>
#include <QHostAddress>
#include <QList>

#include "schat.h"

/*!
 * Базовый класс для событий сервера.
 */
class SCHAT_EXPORT ServerEvent : public QEvent
{
public:
  enum ServerEvents {
    NewPackets = 7666,
    SocketRelease = 7667,
    NewConnection = 0x4E43
  };

  ServerEvent(ServerEvents type, const QList<quint64> &sockets, const QByteArray &m_channelId = QByteArray());
  ServerEvent(ServerEvents type, quint64 socket, const QByteArray &m_channelId = QByteArray());
  inline const QByteArray& channelId() const   { return m_channelId; }
  inline const QList<quint64>& sockets() const { return m_sockets; }
  quint64 socket() const;

protected:
  QByteArray m_channelId;      ///< Идентификатор пользователя.
  QList<quint64> m_sockets;    ///< Идентификаторы сокетов.
};


/*!
 * Отправка пакетов между одиночным сокетом и ядром чата и наоборот.
 */
class SCHAT_EXPORT NewPacketsEvent : public ServerEvent
{
public:
  enum Option {
    NoSocketOption,
    AuthorizeSocketOption,
    KillSocketOption
  };

  NewPacketsEvent(const QList<quint64> &sockets, const QByteArray &packet, const QByteArray &userId = QByteArray());
  NewPacketsEvent(const QList<quint64> &sockets, const QList<QByteArray> &packets, const QByteArray &userId = QByteArray());

  const QList<QByteArray> packets; ///< Тела виртуальных пакетов.
  int option;                      ///< Опция /sa Option.
  QHostAddress address;            ///< Адрес сокета.
  qint64 timestamp;                ///< Отметка времени.
};


class SCHAT_EXPORT SocketReleaseEvent : public ServerEvent
{
public:
  SocketReleaseEvent(quint64 socket, const QString &errorString, const QByteArray &userId = QByteArray());

  const QString errorString;
};


class SCHAT_EXPORT NewConnectionEvent : public ServerEvent
{
public:
  NewConnectionEvent(int socketDescriptor, quint64 socket);

  int socketDescriptor;
};

#endif /* EVENTS_H_ */
