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

#include "ChatCore.h"
#include "ChatHooks.h"
#include "ChatSettings.h"
#include "client/ChatClient.h"
#include "net/SimpleID.h"
#include "NetworkManager.h"

namespace Hooks
{

Networks::Networks(QObject *parent)
  : Client(parent)
{
  ChatClient::i()->hooks()->add(this);
}


bool Networks::openId(const QByteArray &id, bool &matched)
{
  matched = true;

  if (id.isEmpty())
    return ChatCore::networks()->open();

  return ChatCore::networks()->open(id);
}


/*!
 * Получение идентификатора пользователя из менеджера сетей
 * и установка базовых данных из настроек.
 */
QByteArray Networks::id() const
{
  ClientChannel channel = ChatClient::channel();
  channel->setName(ChatCore::settings()->value(ChatSettings::kProfileNick).toString());
  channel->gender().setRaw(ChatCore::settings()->value(ChatSettings::kProfileGender).toInt());
  channel->status().set(ChatCore::settings()->value(ChatSettings::kProfileStatus).toInt());

  Network item = ChatCore::networks()->item(ChatCore::networks()->selected());
  if (!item->isValid())
    return QByteArray();

  channel->setId(item->userId());

  return channel->id();
}


/*!
 * Получение идентификатора сервера из менеджера сетей.
 */
QByteArray Networks::serverId() const
{
  return SimpleID::decode(ChatCore::networks()->selected());
}

} // namespace Hooks
