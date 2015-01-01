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

#include <QNetworkInterface>
#include <QHostInfo>

#include "Channel.h"
#include "DateTime.h"
#include "feeds/ServerFeed.h"
#include "net/PacketReader.h"
#include "net/packets/auth.h"
#include "net/PacketWriter.h"
#include "net/Protocol.h"
#include "net/SimpleID.h"
#include "sglobal.h"
#include "tools/OsInfo.h"
#include "tools/Ver.h"

AuthReply::AuthReply(PacketReader *reader)
  : policy(ServerFeed::MainChannelPolicy | ServerFeed::ForcedJoinPolicy)
{
  serverId = reader->sender();
  userId   = reader->dest();

  fields = reader->get<quint8>();
  status = reader->get<quint16>();
  id     = reader->id();

  if (status == Notice::OK) {
    cookie = reader->id();
    reader->get<quint32>();
    flags      = reader->get<quint8>();
    serverName = reader->text();
    provider   = reader->text();
  }

  if (fields & JSonField)
    json = reader->json().toMap();

  if (fields & HostField) {
    host   = reader->text();
    hostId = reader->id();
  }

  if (status != Notice::OK) {
    reader->get<quint32>();
    flags      = reader->get<quint8>();
    serverName = reader->text();
    provider   = reader->text();
  }

  if (fields & ChannelField) {
    policy  = reader->get<quint8>();
    channel = reader->id();
  }
}


QByteArray AuthReply::data(QDataStream *stream) const
{
  if (!json.isEmpty())
    fields |= JSonField;

  if (!host.isEmpty())
    fields |= HostField;

  if (!channel.isEmpty())
    fields |= ChannelField;

  PacketWriter writer(stream, Protocol::AuthReplyPacket, serverId, userId);
  writer.put(fields);
  writer.put(status);
  writer.putId(id);

  if (status == Notice::OK) {
    writer.putId(cookie);
    writer.put<quint32>(0);
    writer.put<quint8>(flags);
    writer.put(serverName);
    writer.put(provider);
  }

  if (fields & JSonField)
    writer.put(json);

  if (fields & HostField) {
    writer.put(host);
    writer.putId(hostId);
  }

  if (status != Notice::OK) {
    writer.put<quint32>(0);
    writer.put<quint8>(flags);
    writer.put(serverName);
    writer.put(provider);
  }

  if (fields & ChannelField) {
    writer.put<quint8>(policy);
    writer.putId(channel);
  }

  return writer.data();
}


AuthRequest::AuthRequest(PacketReader *reader)
  : os(0)
  , version(0)
  , tz(DateTime::tz())
{
  fields = reader->get<quint8>();
  authType = reader->get<quint8>();
  uniqueId = reader->id();
  id = reader->id();
  gender = reader->get<quint8>();
  setStatus(reader->get<quint8>());
  host = reader->text();
  nick = reader->text();
  userAgent = reader->text();

  if (authType == Cookie || authType == External)
    cookie = reader->id();

  if (fields & JSonField) {
    const QByteArray raw = reader->get<QByteArray>();

    if (raw.size() <= MaxJSONSize)
      json = JSON::parse(raw).toMap();
  }

  if (fields & ExtraInfoField) {
    os       = reader->get<quint8>();
    version  = reader->get<quint32>();
    tz       = reader->get<qint32>();
    hostName = reader->text();
  }
}


/*!
 * Проверка пакета на корректность.
 */
bool AuthRequest::isValid() const
{
  if (SimpleID::typeOf(uniqueId) != SimpleID::UniqueUserId)
    return false;

  if (SimpleID::typeOf(id) != SimpleID::MessageId)
    return false;

  if (userAgent.isEmpty())
    return false;

  if (!Channel::isValidName(nick))
    return false;

  if (authType == Cookie && SimpleID::typeOf(cookie) != SimpleID::CookieId)
    return false;

  return true;
}


QVariantMap AuthRequest::toJSON() const
{
  QVariantMap out;
  out.insert(LS("type"),     authType);
  out.insert(LS("uniqueId"), ChatId(uniqueId).toString());
  out.insert(LS("gender"),   gender);
  out.insert(LS("status"),   status);
  out.insert(LS("host"),     host);
  out.insert(LS("nick"),     nick);
  out.insert(LS("json"),     json);
  out.insert(LS("os"),       os);
  out.insert(LS("version"),  Ver(version).toString());
  out.insert(LS("tz"),       tz);
  out.insert(LS("hostName"), hostName);

  if (!cookie.isEmpty())
    out.insert(LS("cookie"), ChatId(cookie).toString());

  return out;
}


void AuthRequest::setStatus(quint8 status)
{
  if (status == Status::Offline)
    status = Status::Online;

  this->status = status;
}
