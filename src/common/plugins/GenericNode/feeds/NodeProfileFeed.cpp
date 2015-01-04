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

#include "cores/Core.h"
#include "DataBase.h"
#include "DateTime.h"
#include "feeds/NodeProfileFeed.h"
#include "net/packets/Notice.h"
#include "net/SimpleID.h"
#include "ServerChannel.h"
#include "sglobal.h"
#include "SJMPPacket.h"

NodeProfileFeed::NodeProfileFeed(const QString &name, const QVariantMap &data)
  : Feed(name, data)
{
  m_header.acl().setMask(0744);
}


NodeProfileFeed::NodeProfileFeed(const QString &name, qint64 date)
  : Feed(name, date)
{
  m_header.acl().setMask(0744);
}


FeedReply NodeProfileFeed::post(const QString &path, const QVariantMap &json, Channel *channel, const QByteArray &blob)
{
  Q_UNUSED(channel);
  Q_UNUSED(blob)

  if (path.isEmpty() || path.contains(LC('*')) || !json.contains(LS("value")))
    return Notice::BadRequest;

  QVariantMap body;
  body.insert(path, json[LS("value")]);

  SJMPPacket packet;
  packet.setMethod(LS("put"));
  packet.setResource(LS("v4/user/") + head().channel()->nativeId());
  packet.setBody(body);

  SJMPPacket reply = Core::sendSync(packet);

  if (reply.status() == 200) {
    head().channel()->setData(LS("profile"), reply.body());
    return FeedReply(Notice::OK, reply.date());
  }

  return Notice::NotModified;
}


FeedReply NodeProfileFeed::put(const QString &path, const QVariantMap &json, Channel *channel, const QByteArray &blob)
{
  Q_UNUSED(path)
  Q_UNUSED(json)
  Q_UNUSED(channel)
  Q_UNUSED(blob)
  return Notice::Forbidden;
}


QVariantMap NodeProfileFeed::feed(Channel *channel) const
{
  if (head().channel()->type() != SimpleID::UserId || !can(channel, Acl::Read))
    return QVariantMap();

  QVariantMap out = head().channel()->data().value(LS("profile")).toMap();
  out[LS("provider")] = LS("simpleid");

  return out;
}


Feed* NodeProfileFeedCreator::create(const QString &name) const
{
  return new NodeProfileFeed(name, DateTime::utc());
}


Feed* NodeProfileFeedCreator::load(const QString &name, const QVariantMap &data) const
{
  return new NodeProfileFeed(name, data);
}
