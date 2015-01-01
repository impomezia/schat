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

#ifndef HOST_H_
#define HOST_H_

#include <QSharedPointer>
#include <QVariant>

#include "id/ChatId.h"

class AuthRequest;
class SJMPPacket;

/*!
 * Информация о подключении пользователя.
 */
class SCHAT_EXPORT Host
{
public:
  Host()
  : online(false)
  , channel(0)
  , version(0)
  , os(0)
  , tz(0)
  , date(0)
  , socket(0)
  {}

  Host(const AuthRequest &data, const SJMPPacket &packet, const QString &ip, const QString &uuid, quint64 socket);

  bool online;            ///< \b true если подключение активно.
  qint64 channel;         ///< Номер канала в базе данных.
  ChatId hostId;         ///< Публичный идентификатор хоста.
  QString name;           ///< Имя хоста.
  QString ip;             ///< IP адрес пользователя.
  quint32 version;        ///< Версия клиента.
  quint8 os;              ///< Базовый тип операционной системы.
  QString osName;         ///< Имя операционной системы.
  qint32 tz;              ///< Смещение времени относительно UTC.
  qint64 date;            ///< Дата последнего подключения или отключения.
  QVariantMap geo;        ///< GEO информация о пользователе.
  QVariantMap data;       ///< Дополнительные данные.
  QList<quint64> sockets; ///< Список сокетов.
  QByteArray uniqueId;    ///< Уникальный идентификатор пользователя.
  quint64 socket;         ///< Сокет пользователя.
  QString uuid;           ///< UUID подключения.
};

typedef QSharedPointer<Host> HostInfo;

#endif /* HOST_H_ */
