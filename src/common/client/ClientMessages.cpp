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

#include "client/ChatClient.h"
#include "client/ClientChannels.h"
#include "client/ClientCmd.h"
#include "client/ClientHooks.h"
#include "client/ClientMessages.h"
#include "client/SimpleClient.h"
#include "DateTime.h"
#include "id/ChatId.h"
#include "net/Channels.h"
#include "net/SimpleID.h"
#include "sglobal.h"
#include "text/PlainTextFilter.h"

ClientMessages::ClientMessages(QObject *parent)
  : QObject(parent)
  , m_version(Protocol::V4_0)
  , m_client(ChatClient::io())
{
  connect(m_client, SIGNAL(notice(int)), SLOT(notice(int)));
  connect(ChatClient::channels(), SIGNAL(channels(QList<QByteArray>)), SLOT(channels(QList<QByteArray>)));
}


/*!
 * Возвращает \b true если используется время клиента, вместо времени пакета
 * для определения времени сообщения.
 *
 * \p status Статус пакета.
 */
bool ClientMessages::isClientDate(int status)
{
  if (status == Notice::Found || status == Notice::Unread || status == Notice::Read || status >= Notice::UserStatus)
    return false;

  return true;
}


/*!
 * Отправка текстового сообщения, если в тексте будут команды, то они будут обработаны.
 */
bool ClientMessages::send(const QByteArray &dest, const QString &text)
{
  if (text.isEmpty())
    return false;

  m_destId = dest;
  QString plain = PlainTextFilter::filter(text);

  if (!plain.startsWith(LC('/')))
    return sendText(dest, text);

  if (command(dest, text, plain))
    return true;

  /// Для обработки обычных команд используется хук: Hooks::Messages::command(const QByteArray &dest, const ClientCmd &cmd).
  QStringList commands = (LS(" ") + plain).split(LS(" /"), QString::SkipEmptyParts);
  int matches = 0;
  for (int i = 0; i < commands.size(); ++i) {
    ClientCmd cmd(commands.at(i));
    if (cmd.isValid() && command(dest, cmd))
      matches++;
  }

  if (!matches)
    return sendText(dest, text);

  return true;
}


/*!
 * Отправка текстового сообщения, команды не обрабатываются.
 */
bool ClientMessages::sendText(const QByteArray &dest, const QString &text, const QString &command)
{
  if (SimpleID::typeOf(dest) == SimpleID::ServerId)
    return false;

  MessagePacket packet(new MessageNotice(ChatClient::id(), dest, text, DateTime::utc(), m_randomId.init(ObjectId::gen()).toByteArray()));
  if (!command.isEmpty())
    packet->setCommand(command);

  if (m_client->send(packet, true)) {
    sent(packet);
    return true;
  }
  else {
    foreach (Hooks::Messages *hook, m_hooks)
      hook->error(packet);

    return false;
  }
}


void ClientMessages::insert(MessageNotice *notice)
{
  readText(MessagePacket(notice));
}


void ClientMessages::channels(const QList<QByteArray> &channels)
{
  foreach (const QByteArray &id, channels) {
    if (m_pending.contains(id)) {
      const QList<MessagePacket> packets = m_pending.values(id);

      foreach (MessagePacket packet, packets) {
        if (SimpleID::typeOf(packet->dest()) == SimpleID::ChannelId) {
          if (ChatClient::channels()->get(packet->sender()) && ChatClient::channels()->get(packet->dest()))
            m_pending.remove(packet->dest(), packet);
          else
            continue;
        }

        read(packet);
        m_pending.remove(packet->sender(), packet);
      }
    }
  }
}


/*!
 * Чтение входящего сообщения.
 */
void ClientMessages::notice(int type)
{
  if (type != Notice::MessageType)
    return;

  readText(MessagePacket(new MessageNotice(type, ChatClient::io()->reader())));
}


bool ClientMessages::command(const QByteArray &dest, const ClientCmd &cmd)
{
  for (int i = 0; i < m_hooks.size(); ++i) {
    if (m_hooks.at(i)->command(dest, cmd))
      return true;
  }

  const QString command = cmd.command().toLower();
  const QString& body   = cmd.body();

  if (command == CHANNELS_JOIN_CMD) {
    if (cmd.isBody()) {
      if (body.size() == 34)
        ChatClient::channels()->join(SimpleID::decode(body));
      else
        ChatClient::channels()->join(body);
    }
    else
      ChatClient::channels()->join(dest);

    return true;
  }

  if (command == LS("talk")) {
    if (cmd.isBody())
      ChatClient::channels()->join(body, ChatClient::id());

    return true;
  }

  if (command == LS("nick")) {
    ChatClient::channels()->nick(body);
    return true;
  }

  if (command == LS("name")) {
    ChatClient::channels()->name(dest, body);
    return true;
  }

  if (command == LS("part")) {
    ChatClient::channels()->part(dest);
    return true;
  }

  return false;
}


bool ClientMessages::command(const QByteArray &dest, const QString &text, const QString &plain)
{
  for (int i = 0; i < m_hooks.size(); ++i) {
    if (m_hooks.at(i)->command(dest, text, plain))
      return true;
  }

  if (plain.startsWith(LS("/me "), Qt::CaseInsensitive)) {
    sendText(dest, Hooks::Messages::remove(LS("/me "), text), LS("me"));
    return true;
  }

  if (plain.startsWith(LS("/say "), Qt::CaseInsensitive)) {
    sendText(dest, Hooks::Messages::remove(LS("/say "), text), LS("say"));
    return true;
  }

  return false;
}


void ClientMessages::prepare(MessagePacket packet)
{
  m_version = m_client->version();
  ChatId id = packet->id();

  if (isClientDate(packet->status())) {
    packet->setDate(ChatClient::date());

    if (packet->direction() != Notice::Internal) {
      packet->setInternalId(id.toByteArray());

      if (m_version >= Protocol::V4_1 && id.hasOid())
        id.setDate(packet->date());
      else
        id.init(packet->toId());

      packet->setId(id.toByteArray());
    }
  }

  if (m_version >= Protocol::V4_1 && id.hasOid()) {
    packet->oid   = id.oid();
    packet->mdate = id.date();
  }
}


void ClientMessages::read(MessagePacket packet)
{
  int matches = 0;
  foreach (Hooks::Messages *hook, m_hooks) {
    matches += hook->read(packet);
  }

  if (matches)
    return;

  foreach (Hooks::Messages *hook, m_hooks) {
    hook->unhandled(packet);
  }
}


/*!
 * Чтение текстового сообщения.
 */
void ClientMessages::readText(MessagePacket packet)
{
  if (!packet->isValid())
    return;

  prepare(packet);

  ClientChannels *channels = ChatClient::channels();
  ClientChannel sender     = channels->get(packet->sender());
  ClientChannel dest       = channels->get(packet->dest());
  const int destType       = SimpleID::typeOf(packet->dest());

  if (ChatClient::state() == ChatClient::Online) {
    // Если информация о каналах отправителя и получателя имеется и синхронизирована, сообщение обрабатывается без задержки.
    // В случае если получатель обычный канал, но вход в него не выполнен, производится вход в этот канал.
    if (sender && sender->isSynced() && dest && dest->isSynced()) {
      if (destType == SimpleID::ChannelId && !channels->joined().contains(dest->id()))
        channels->join(dest->id());

      return read(packet);
    }

    if (!m_pending.contains(packet->sender()))
      channels->join(packet->sender());

    m_pending.insert(packet->sender(), packet);

    if (destType == SimpleID::ChannelId) {
      if (!m_pending.contains(packet->dest()))
        channels->join(packet->dest());

      m_pending.insert(packet->dest(), packet);
    }
  }
  else if (sender && dest)
    read(packet);
}


void ClientMessages::sent(MessagePacket packet)
{
  foreach (Hooks::Messages *hook, m_hooks) {
    hook->sent(packet);
  }
}
