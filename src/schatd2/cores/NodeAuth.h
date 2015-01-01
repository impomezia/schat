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

#ifndef NODEAUTH_H_
#define NODEAUTH_H_

#include "id/ChatId.h"
#include "SJMPPacket.h"

class AuthRequest;
class Core;

class AuthResult
{
public:
  enum Action {
    Reject,  ///< Пользователь не авторизирован.
    Accept,  ///< Пользователь авторизирован.
    Pending, ///< Авторизация отложена.
    Nothing, ///< Действие не требуется.
  };

  AuthResult(Action action = Nothing)
  : action(action)
  , packet(true)
  , option(0)
  , status(200)
  {}

  AuthResult(int status, const QByteArray &authId, int option = 2)
  : action(Reject)
  , packet(true)
  , option(option)
  , status(status)
  , authId(authId)
  {}

  AuthResult(const QByteArray &id, const QByteArray &authId, const SJMPPacket &packet)
  : action(Accept)
  , packet(true)
  , option(1)
  , status(200)
  , authId(authId)
  , id(id)
  , hostId(ChatId(packet.header(QLatin1String("session")).toByteArray(), ChatId::HostId))
  , cookie(packet.header(QLatin1String("cookie")).toByteArray())
  {}

  virtual ~AuthResult() {}

  Action action;     ///< Действие.
  bool packet;       ///< true если нужно отправить стандартный AuthReplyPacket.
  int option;        ///< Дополнительная опция для установки состояния сокета.
  int status;        ///< Статус.
  QByteArray authId; ///< Идентификатор авторизации.
  QByteArray id;     ///< Идентификатор пользователя.
  QVariantMap json;  ///< JSON данные.
  ChatId hostId;
  ChatId cookie;
};


/*!
 * Базовый класс модуля авторизации.
 */
class NodeAuth
{
public:
  NodeAuth(Core *core)
  : m_core(core)
  {}

  virtual ~NodeAuth() {}
  virtual AuthResult auth(const AuthRequest &data) = 0;
  virtual int type() const = 0;

protected:
  Core *m_core;
};

#endif /* NODEAUTH_H_ */
