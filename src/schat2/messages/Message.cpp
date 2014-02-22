/* $Id: Message.cpp 3749 2013-07-12 20:50:37Z IMPOMEZIA $
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

#include "client/ChatClient.h"
#include "DateTime.h"
#include "messages/Message.h"
#include "net/SimpleID.h"
#include "sglobal.h"
#include "WebBridge.h"

const QString Message::kAuthor = QLatin1String("Author");
const QString Message::kDate   = QLatin1String("Date");
const QString Message::kDay    = QLatin1String("Day");
const QString Message::kExtra  = QLatin1String("Extra");
const QString Message::kFunc   = QLatin1String("Func");
const QString Message::kHint   = QLatin1String("Hint");
const QString Message::kId     = QLatin1String("Id");
const QString Message::kText   = QLatin1String("Text");
const QString Message::kType   = QLatin1String("Type");

Message::Message()
{
}


Message::Message(const QByteArray &id, const QByteArray &tab, const QString &type, const QString &func)
  : m_tab(tab)
{
  setId(id);

  if (!type.isEmpty())
    m_data.insert(kType, type);

  if (!func.isEmpty())
    m_data.insert(kFunc, func);
}


/*!
 * Определение идентификатора вкладки в зависимости от отправителя и получателя сообщения.
 */
QByteArray Message::detectTab(const QByteArray &sender, const QByteArray &dest)
{
  if (ChatId(dest).type() == ChatId::ChannelId || sender == ChatClient::id())
    return dest;

  return sender;
}


/*!
 * Установка автора сообщения.
 */
void Message::setAuthor(const QByteArray &id)
{
  const QVariantMap data = WebBridge::channel(id);

  if (!data.isEmpty())
    m_data.insert(kAuthor, data);
}


void Message::setDate(qint64 date)
{
  if (!date)
    date = DateTime::utc();

  m_data.insert(kDate, date);
}


/*!
 * Установка идентификатора сообщения.
 */
void Message::setId(const QByteArray &id)
{
  m_data.insert(kId, ChatId(id).toString());
}
