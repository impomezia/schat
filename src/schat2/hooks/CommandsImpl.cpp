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

#include <QApplication>

#include "ChatCore.h"
#include "ChatNotify.h"
#include "ChatSettings.h"
#include "ChatUrls.h"
#include "client/ChatClient.h"
#include "client/ClientChannels.h"
#include "client/ClientCmd.h"
#include "client/ClientMessages.h"
#include "client/SimpleClient.h"
#include "DateTime.h"
#include "hooks/CommandsImpl.h"
#include "messages/AlertMessage.h"
#include "QtEscape.h"
#include "sglobal.h"
#include "tools/OsInfo.h"
#include "ui/StatusMenu.h"

#if QT_VERSION >= 0x050000
# include <QWebPage>
#else
# include <qwebkitversion.h>
#endif

namespace Hooks
{

CommandsImpl::CommandsImpl(QObject *parent)
  : Messages(parent)
{
  ChatClient::messages()->add(this);
}


/*!
 * Обработка команд.
 */
bool CommandsImpl::command(const QByteArray &dest, const ClientCmd &cmd)
{
  const QString command = cmd.command().toLower();

  /// - /about Открытие вкладки O Simple Chat.
  if (command == LS("about"))
    ChatNotify::start(Notify::OpenAbout);

  /// - /away Установка статуса «Отсутствую» (Away).
  else if (command == LS("away"))
    StatusMenu::apply(Status::Away);

  else if (command == LS("clear")) {
    if (cmd.body().isEmpty())
      ChatNotify::start(Notify::ClearChat, dest);
    else if (cmd.body() == LS("cache"))
      ChatNotify::start(Notify::ClearCache);
  }

  else if (command == LS("cache")) {
    if (cmd.body() == LS("clear"))
      ChatNotify::start(Notify::ClearCache);
    else
      AlertMessage::show(tr("Usage: <b>/cache clear</b>"), ALERT_MESSAGE_INFO);
  }

  /// - /color Установка цвета иконки пользователя.
  else if (command == LS("color")) {
    if (cmd.isBody())
      setGender(QString(), cmd.body());
    else
      setGender(QString(), LS("default"));
  }

  /// - /dnd Установка статуса «Не беспокоить» (Do not disturb).
  else if (command == LS("dnd"))
    StatusMenu::apply(Status::DnD);

  /// - /exit Выход из чата.
  else if (command == LS("exit") || command == LS("quit"))
    ChatNotify::start(Notify::Quit);

  /// - /female Установка женского пола пользователя.
  else if (command == LS("female"))
    setGender(command, cmd.body());

  /// - /ffc Установка статуса «Готов болтать» (Free for chat).
  else if (command == LS("ffc"))
    StatusMenu::apply(Status::FreeForChat);

  /// - /gender Установка пола пользователя.
  else if (command == LS("gender"))
    setGender(cmd.body(), QString());

  /// - /hide Скрытие окна чата.
  else if (command == LS("hide"))
    ChatNotify::start(Notify::ToggleVisibility);

  else if (command == LS("id"))
    ChatNotify::start(Notify::ShowID, dest);

  /// - /male Установка мужского пола пользователя.
  else if (command == LS("male"))
    setGender(command, cmd.body());

  /// - /nick Установка нового ника.
  else if (command == LS("nick")) {
    if (!Channel::isValidName(cmd.body()))
      return true;

    if (ChatClient::state() != ChatClient::Online) {
      ChatClient::io()->setNick(cmd.body());
      ChatCore::settings()->setValue(ChatSettings::kProfileNick, ChatClient::channel()->name());
      return true;
    }

    return false;
  }

  /// - /offline Установка статуса «Не в сети» (Offline) и отключение от сервера.
  else if (command == LS("offline"))
    StatusMenu::apply(Status::Offline);

  /// - /online Установка статуса «В сети» (Online).
  else if (command == LS("online"))
    StatusMenu::apply(Status::Online);

  /// - /open Открытие URL адреса.
  else if (command == LS("open"))
    ChatUrls::open(cmd.body());

  /// - /restart Перезапуск чата.
  else if (command == LS("restart"))
    ChatNotify::start(Notify::Restart);

  /// - /set Установка опции чата, имя опции чувствительно к регистру символов.
  else if (command == LS("set")) {
    ClientCmd body(cmd.body());
    if (body.isValid() && body.isBody())
      ChatCore::settings()->setValue(body.command(), body.body());
  }
  else if (command == LS("ping")) {
    MessagePacket packet(new MessageNotice(ChatClient::id(), dest, QString(), 0, ChatCore::randomId()));
    packet->setCommand(command);
    packet->setDirection(Notice::Internal);
    packet->setStatus(Notice::Found);
    ChatClient::io()->send(packet, false);
  }
  else if (command == LS("version") || command == LS("ver")) {
    AlertMessage::show(QString(LS("<b>%1-%2</b>, Qt %3, WebKit %4, %5"))
                       .arg(QApplication::applicationVersion())
                       .arg(QString(GIT_REVISION).left(7))
                       .arg(qVersion())
                       .arg(qWebKitVersion())
                       .arg(OsInfo::os())
                       , ALERT_MESSAGE_INFO);
  }
  else
    return false;

  return true;
}


int CommandsImpl::read(MessagePacket packet)
{
  const QString &command = packet->command();
  if (command == LS("pong")) {
    const int offset     = qAbs(DateTime::utc() - packet->date());
    const QString prefix = !packet->text().isEmpty() ? LS("<b>") + Qt::escape(packet->text()) + LS("</b> ") : QString();

    AlertMessage::show(prefix + Hooks::CommandsImpl::tr("Latency time: <b style='color:#%1'>%2 ms</b>")
        .arg(colorizePing(offset))
        .arg(offset), ALERT_MESSAGE_INFO, Message::detectTab(packet->sender(), packet->dest()));

    return 1;
  }
  else if (command == LS("ping")) {
    if (packet->dest() != ChatClient::id())
      return 1;

    MessagePacket pong(new MessageNotice(ChatClient::id(), packet->sender(), ChatClient::io()->json().value(CLIENT_PROP_HOST).toString(), packet->date(), packet->id()));
    pong->setCommand(LS("pong"));
    pong->setDirection(Notice::Internal);
    pong->setStatus(Notice::Found);
    ChatClient::io()->send(pong, ChatClient::id() == packet->sender());

    return 1;
  }

  return 0;
}


QString CommandsImpl::colorizePing(int offset) const
{
  if (offset > 500)
    return LS("da251d");

  if (offset > 200)
    return LS("ff9900");

  return LS("6bb521");
  QString color = "6bb521";
  if (offset > 200 && offset < 500)
    color = "ff9900";
  else if (offset >= 500)
    color = "da251d";

  return QString(LS("<b style='color:#%1'>%2</b>")).arg(color).arg(offset);
}


void CommandsImpl::setGender(const QString &gender, const QString &color)
{
  if (gender.isEmpty() && color.isEmpty())
    return;

  Gender data = ChatClient::channel()->gender();

  if (!gender.isEmpty()) {
    if (gender == LS("male"))
      data.set(Gender::Male);
    else if (gender == LS("female"))
      data.set(Gender::Female);
    else if (gender == LS("ghost"))
      data.set(Gender::Ghost);
    else if (gender == LS("unknown"))
      data.set(Gender::Unknown);
    else if (gender == LS("bot"))
      data.set(Gender::Bot);
    else
      return;
  }

  if (!color.isEmpty())
    data.setColor(Gender::stringToColor(color));

  if (ChatClient::channel()->gender().raw() == data.raw())
    return;

  ChatClient::channel()->gender() = data.raw();
  ChatCore::settings()->setValue(ChatSettings::kProfileGender, data.raw());

  if (ChatClient::state() == ChatClient::Online)
    ChatClient::channels()->update();
}

} // namespace Hooks
