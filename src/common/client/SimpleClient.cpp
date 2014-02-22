/* $Id: SimpleClient.cpp 3705 2013-06-23 02:24:46Z IMPOMEZIA $
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

#include "client/SimpleClient.h"
#include "client/SimpleClient_p.h"
#include "debugstream.h"
#include "net/Channels.h"
#include "net/PacketReader.h"
#include "net/packets/auth.h"
#include "net/packets/ChannelNotice.h"
#include "net/packets/Notice.h"
#include "sglobal.h"

SimpleClientPrivate::SimpleClientPrivate()
  : authType(AuthRequest::Discovery)
{
}


SimpleClientPrivate::~SimpleClientPrivate()
{
}


SimpleClientPrivate::AuthReplyAction SimpleClientPrivate::authReply(const AuthReply &reply)
{
  if (clientState == AbstractClient::ClientOnline)
    return Nothing;

  AuthReplyAction action = AbstractClientPrivate::authReply(reply);
  json[CLIENT_PROP_ID]      = ChatId(reply.serverId).toBase32();
  json[CLIENT_PROP_HOST]    = reply.host;
  json[CLIENT_PROP_HOST_ID] = reply.hostId;
  json[CLIENT_PROP_CHANNEL] = reply.channel;
  json[CLIENT_PROP_POLICY]  = reply.policy;

  if (reply.status == Notice::OK) {
    authType = AuthRequest::Cookie;
    json.remove(CLIENT_PROP_ERROR);
    return action;
  }

  QVariantMap error;
  error[CLIENT_PROP_ERROR_TYPE]   = LS("auth");
  error[CLIENT_PROP_ERROR_AUTH]   = authType;
  error[CLIENT_PROP_ERROR_STATUS] = reply.status;
  error[CLIENT_PROP_ERROR_DATA]   = reply.json;
  json[CLIENT_PROP_ERROR]         = error;

  if (reply.status == Notice::NickAlreadyUse)
    return action;

  // Выбранный способ авторизации не реализован.
  if (reply.status == Notice::NotImplemented) {
    if (authType == AuthRequest::Discovery || authType == AuthRequest::External)
      authType = AuthRequest::Anonymous;

    return action;
  }

  if (reply.status == Notice::Found || reply.status == Notice::Forbidden) {
    json[CLIENT_PROP_AUTH_SERVER] = reply.provider;
    json[CLIENT_PROP_ANONYMOUS]   = (bool) reply.flags & 1;

    if (reply.provider.isEmpty() && !(reply.flags & 1))
      return ErrorState;

    return WaitAuthState;
  }

  return action;
}


/*!
 * Чтение пакетов типа Protocol::NoticePacket.
 * \sa Notice.
 */
bool SimpleClientPrivate::notice()
{
  Q_Q(SimpleClient);
  quint16 type = reader->get<quint16>();
  emit(q->notice(type));

  return true;
}


SimpleClient::SimpleClient(QObject *parent)
  : AbstractClient(*new SimpleClientPrivate(), parent)
{
}


SimpleClient::SimpleClient(SimpleClientPrivate &dd, QObject *parent)
  : AbstractClient(dd, parent)
{
}


SimpleClient::~SimpleClient()
{
}


void SimpleClient::leave()
{
  send(ChannelNotice::request(channelId(), channelId(), CHANNELS_QUIT_CMD));

  AbstractClient::leave();
}


void SimpleClient::setAuthType(int authType)
{
  d_func()->authType = authType;
}


/*!
 * Формирование пакета запроса авторизации.
 */
void SimpleClient::requestAuth()
{
  Q_D(SimpleClient);

  if (d->authType == AuthRequest::Cookie && d->cookie.type() != ChatId::CookieId)
    d->authType = AuthRequest::Discovery;

  if (d->uniqueId.isNull())
    d->uniqueId.init(ChatId::UniqueUserId, Channel::defaultName().toUtf8());

  if (d->authId.isNull())
    d->authId.init(ChatId::MessageId);

  AuthRequest data(d->authType, d->url.toString(), d->channel.data());
  data.uniqueId = d->uniqueId.byteArray();
  data.cookie   = d->cookie.byteArray();
  data.id       = d->authId.byteArray();
  send(data.data(d->sendStream));
}


/*!
 * Обработка пакетов.
 */
void SimpleClient::newPacketsImpl()
{
  Q_D(SimpleClient);
# if defined(SCHAT_DEBUG)
  SCHAT_DEBUG_STREAM(this << "newPacketsImpl() BEGIN")
  int size = 0;
  for (int i = 0; i < d->readQueue.size(); ++i)
    size += d->readQueue.at(i).size();

  qDebug() << "             count:" << d->readQueue.size();
  qDebug() << "             size: " << size << "bytes";
  QTime t;
  t.start();
# endif

  while (!d->readQueue.isEmpty()) {
    d->readBuffer = d->readQueue.takeFirst();
    PacketReader reader(d->readStream);
    d->reader = &reader;

    SCHAT_DEBUG_STREAM("CURRENT PACKET SIZE:" << d->readBuffer.size())

    switch (reader.type()) {
      case Protocol::AuthReplyPacket:
        d->doneAuth(d->authReply(AuthReply(d->reader)));
        break;

      case Protocol::NoticePacket:
        d->notice();
        break;

      default:
        break;
    }
  }

  SCHAT_DEBUG_STREAM(this << "newPacketsImpl() END elapsed:" << t.elapsed() << "ms")
  emit idle();
}
