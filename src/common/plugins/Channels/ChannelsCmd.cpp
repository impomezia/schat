/* $Id: ChannelsCmd.cpp 3714 2013-06-26 00:57:48Z IMPOMEZIA $
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

#include "alerts/AlertType.h"
#include "ChannelsCmd.h"
#include "ChannelsPlugin_p.h"
#include "ChatAlerts.h"
#include "client/ClientCmd.h"
#include "DateTime.h"
#include "messages/AlertMessage.h"
#include "net/SimpleID.h"
#include "sglobal.h"
#include "ui/ChatIcons.h"
#include "ui/TabWidget.h"
#include "WebBridge.h"
#include "JSON.h"


class InviteAlertType : public AlertType
{
public:
  InviteAlertType(int weight)
  : AlertType(LS("invite"), weight)
  {
    m_defaults[ALERT_POPUP_KEY] = true;
    m_defaults[ALERT_TRAY_KEY]  = true;
    m_defaults[ALERT_SOUND_KEY] = true;
    m_defaults[ALERT_FILE_KEY]  = LS("Received.wav");

    m_icon = SCHAT_ICON(Add);
  }

  QString name() const
  {
    return QObject::tr("Invitation to a channel");
  }
};

ChannelsCmd::ChannelsCmd(QObject *parent)
  : MessagesHook(parent)
{
  ChatAlerts::add(new InviteAlertType(380));
}


bool ChannelsCmd::command(const QByteArray &dest, const ClientCmd &cmd)
{
  Q_UNUSED(dest)
  const QString command = cmd.command().toLower();

  if (command == LS("ignore")) {
    if (isTalk(dest, command))
      ChannelsPluginImpl::ignore(dest);
  }
  else if (command == LS("unignore")) {
    if (isTalk(dest, command))
      ChannelsPluginImpl::ignore(dest, false);
  }
  else if (command == LS("list")) {
    ChannelsPluginImpl::show();
  }
  else
    return false;

  return true;
}


int ChannelsCmd::read(MessagePacket packet)
{
  const QString &command = packet->command();

  if (command == LS("invite")) {
    const QByteArray tab = Message::detectTab(packet->sender(), packet->dest());
    if (SimpleID::typeOf(tab) != SimpleID::UserId)
      return 1;

    const QVariantMap json     = JSON::parse(packet->text().toUtf8()).toMap();
    const QByteArray channelId = SimpleID::decode(json.value(LS("id")).toString());
    if (SimpleID::typeOf(channelId) != SimpleID::ChannelId)
      return 1;

    const QString name = json.value(LS("name")).toString();
    const qint64 date  = DateTime::utc();
    const QString text = inviteText(tab, channelId, name);

    AlertMessage message(text, ALERT_MESSAGE_INFO);
    message.setTab(tab);
    message.setId(packet->id());
    message.setDate(date);
    message.data().insert(Message::kFunc, LS("addInviteMessage"));
    message.data().insert(LS("Url"), LS("chat://channel/") + SimpleID::encode(channelId) + LS("/open?name=") + ChatId::toBase32(name.toUtf8()) + LS("&gender=0"));
    message.data().insert(LS("Channel"), QString(SimpleID::encode(channelId)));
    TabWidget::add(message);

    Alert alert(command, packet->id(), date);
    alert.setTab(tab);
    popup(alert, text);
    ChatAlerts::start(alert);
    return 1;
  }

  return 0;
}


/*!
 * Формирование текста приглашения в канал.
 *
 * \param tab       Идентификатор отправителя приглашения.
 * \param channelId Идентификатор канала, в который приглашается пользователь.
 * \param name      Имя канала, на случай если получатель ничего не знает об этом канале.
 */
QString ChannelsCmd::inviteText(const QByteArray &tab, const QByteArray &channelId, const QString &name) const
{
  return tr("%1 invites you to channel %2")
      .arg(WebBridge::toLink(tab))
      .arg(WebBridge::toLink(channelId, LS("open"), name));
}


/*!
 * Формирование информации для всплывающего окна.
 */
void ChannelsCmd::popup(Alert &alert, const QString &text) const
{
  AlertType *type = ChatAlerts::type(LS("invite"));
  if (!type || !type->popup())
    return;

  QVariantMap popup;
  popup[LS("text")]  = text;
  popup[LS("title")] = type->name();
  alert.data()[LS("popup")] = popup;
}
