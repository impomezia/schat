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

#include "acl/AclValue.h"
#include "Ch.h"
#include "cores/Core.h"
#include "DateTime.h"
#include "events.h"
#include "JSON.h"
#include "net/DataCreator.h"
#include "net/Net.h"
#include "net/NetContext.h"
#include "net/NetReply.h"
#include "net/NetRequest.h"
#include "net/PacketWriter.h"
#include "NodeLog.h"
#include "sglobal.h"

#define LOG_N9010 LOG_TRACE("N9010", "Core/Net", "s:" << context.socket() << ". data:" << context.req()->toJSON())
#define LOG_N9011 LOG_ERROR("N9011", "Core/Net", "s:" << context.socket() << ". Unknown sender \"" << ChatId(Core::i()->packetsEvent()->channelId()).toString() << "\"")
#define LOG_N9012 LOG_ERROR("N9012", "Core/Net", "s:" << context.socket() << ". channel \"" << destId << "\" not found")

Net::Net(QObject *parent)
  : QObject(parent)
{
}


void Net::add(DataCreator *creator)
{
  m_creators.add(creator);
}


/*!
 * Внутренняя функция для инициирования публикации изменений на основе состояния сервера.
 */
void Net::pub(ChatChannel channel, const QString &path)
{
  if (!channel)
    return;

  DataCreator *creator = m_creators.get(path);
  if (!creator)
    return;

  NetRecord record;
  if (!creator->create(channel, path, record))
    return;

  if (!record.date)
    record.date = DateTime::utc();

  pub(channel, path, record);
}


/*!
 * Внутренняя функция публикации изменений.
 */
void Net::pub(ChatChannel channel, const QString &path, const NetRecord &record)
{
  const ChatId id(channel->id());

  NetRecordMap &map = m_data[id];
  if (map.value(path).date == record.date)
    return;

  map.insert(path, record);
  broadcast(channel, path, record);
}


/*!
 * Обработка запроса от клиента.
 *
 * \param context Контекст запроса.
 * \param reply   Ответ на запрос.
 */
void Net::req(const NetContext &context, NetReply &reply)
{
  if (!prepare(context, reply))
    return;

  if (context.req()->method == NetRequest::GET)
    get(context, reply);
}


Net::Creators::~Creators()
{
  qDeleteAll(m_list);
}


void Net::Creators::add(DataCreator *creator)
{
  if (m_list.contains(creator))
    return;

  m_list.append(creator);
  const QStringList paths = creator->paths();
  foreach (const QString &path, paths)
    m_map.insert(path, creator);
}


/*!
 * Обработка GET запроса.
 *
 * В случае необходимости пользователь будет автоматически подписан на уведомления об изменении данных.
 */
bool Net::get(const NetContext &context, NetReply &reply) const
{
  const QString path = context.req()->request.section(LC('/'), 1);
  if (!subscribe(path)) {
    reply.status = NetReply::FORBIDDEN;
    return false;
  }

  const NetRecordMap &map = m_data[m_dest->id()];
  if (map.contains(path)) {
    const NetRecord &record = map[path];
    if (context.req()->date && context.req()->date == record.date) {
      reply.status = NetReply::NOT_MODIFIED;
      return true;
    }

    reply.date   = record.date;
    reply.status = NetReply::OK;
    reply.data   = record.data;
    return true;
  }

  return false;
}


/*!
 * FIXME: L4SGc3F
 *
 * Подготовка запроса к чтению.
 */
bool Net::prepare(const NetContext &context, NetReply &reply)
{
  LOG_N9010

  m_sender = Ch::channel(Core::i()->packetsEvent()->channelId(), ChatId::UserId, false);
  if (!m_sender) {
    reply.status = NetReply::BAD_REQUEST;
    LOG_N9011
    return false;
  }

  if (context.req()->headers.contains(LS("user"))) {
    if (m_sender->nativeId() != LS("L4SGc3F")) {
      reply.status = NetReply::BAD_REQUEST;
      return false;
    }

    const ChatId id(context.req()->headers.value(LS("user")).toString());
    m_user = id.isNull() ? ChatChannel() : Ch::channel(id.toByteArray(), ChatId::UserId);
  }
  else
    m_user = m_sender;

  const QString destId = context.req()->request.section(LC('/'), 0, 0);
  if (destId == LS("server")) {
    m_dest = Ch::server();
  }
  else {
    ChatId id(destId);
    if (!id.isNull())
      m_dest = Ch::channel(id.toByteArray(), id.type());
  }

  if (!m_dest) {
    LOG_N9012
    return false;
  }

  return true;
}


/*!
 * Проверка прав доступа на чтение и автоматическая подписка в случае необходимости.
 *
 * \param path Имя ресурса.
 * \return \true если у пользователя есть права на чтение, и он подписан на изменения этого ресурса.
 */
bool Net::subscribe(const QString &path) const
{
  int changed = 0;
  ChatId id;

  if (m_user)
    id.init(m_user->id());

  if (!m_dest->subscribers().contains(path, id)) {
    if (!(matchAcl(path) & Acl::Read))
      return false;

    m_dest->subscribers().add(path, id);
    ++changed;
  }

  if (m_sender != m_user) {
    id.init(m_sender->id());

    if (!m_dest->subscribers().contains(path, id)) {
      m_dest->subscribers().add(path, id);
      ++changed;
    }
  }

  if (changed)
    emit subscriptionChanged(m_dest->id(), path);

  return true;
}


/*!
 * \todo Добавить проверку прав доступа.
 */
int Net::matchAcl(const QString &path) const
{
  Q_UNUSED(path)

  return Acl::Read;
}


/*!
 * Рассылка уведомления всем подписчикам при изменении ресурса.
 */
void Net::broadcast(ChatChannel channel, const QString &path, const NetRecord &record)
{
  const QList<quint64> sockets = channel->subscribers().sockets(path);
  if (sockets.isEmpty())
    return;

  QVariantList list;
  list.append(LS("RES"));
  list.append(ChatId(channel->id()).toString() + (path.size() ? LS("/") : QString()) + path);
  list.append(record.date);
  list.append(QVariantMap());

  if (record.data.canConvert(QVariant::List)) {
    const QVariantList d = record.data.toList();
    if (!d.isEmpty())
      list.append(record.data.toList());
    else
      list.append(QVariant());
  }
  else
    list.append(record.data);

  PacketWriter writer(Core::stream(), Protocol::JSONPacket);
  writer.put(JSON::generate(list));
  Core::i()->send(sockets, writer.data());
}
