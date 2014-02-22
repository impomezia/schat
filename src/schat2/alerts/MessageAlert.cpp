/* $Id: MessageAlert.cpp 3586 2013-03-22 12:36:44Z IMPOMEZIA $
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

#include "alerts/MessageAlert.h"
#include "client/ChatClient.h"
#include "client/ClientChannels.h"
#include "messages/ChannelMessage.h"
#include "net/packets/MessageNotice.h"
#include "net/SimpleID.h"
#include "QtEscape.h"
#include "sglobal.h"

MessageAlert::MessageAlert(const ChannelMessage &message)
  : Alert(LS("public"), message.packet()->id(), message.packet()->date())
{
  m_tab = message.tab();

  if (message.data().value(LS("Status")) == LS("referring"))
    m_type = LS("referring");
  else if (SimpleID::typeOf(m_tab) == SimpleID::UserId)
    m_type = LS("private");

  const QVariantMap &data  = message.data();
  m_data[LS("Message")] = data;

  popup(data);
}


/*!
 * Формирование данных для всплывающего окна.
 */
void MessageAlert::popup(const QVariantMap &data)
{
  AlertType *type = ChatAlerts::type(m_type);
  if (!type || !type->popup())
    return;

  const QVariantMap author = data.value(LS("Author")).toMap();
  const QString nick = QString(LS("<b>%1</b>")).arg(Qt::escape(author.value(LS("Name")).toString()));
  QVariantMap popup;

  if (SimpleID::typeOf(m_tab) == SimpleID::ChannelId) {
    ClientChannel channel = ChatClient::channels()->get(m_tab);
    if (channel)
      popup[LS("title")] = QString(LS("<b>%1</b>")).arg(channel->name());

    popup[LS("text")]  = nick + LS(": ") + data.value(LS("Text")).toString();
  }
  else {
    popup[LS("text")]  = data.value(LS("Text"));
    popup[LS("title")] = nick;
  }

  m_data[LS("popup")] = popup;
}


MessageAlertType::MessageAlertType(const QString &type, int weight)
  : AlertType(type, weight)
{
  m_defaults[ALERT_POPUP_KEY] = type != LS("public");
  m_defaults[ALERT_TRAY_KEY]  = true;
  m_defaults[ALERT_SOUND_KEY] = true;
  m_defaults[ALERT_FILE_KEY]  = LS("Received.wav");

  if (type == LS("public"))
    m_icon = QIcon(LS(":/images/channel.png"));
  else
    m_icon = QIcon(LS(":/images/balloon.png"));
}


QString MessageAlertType::name() const
{
  if (m_type == LS("public"))
    return QObject::tr("Public message");
  else if (m_type == LS("referring"))
    return QObject::tr("Referring to you");
  else
    return QObject::tr("Private message");
}
