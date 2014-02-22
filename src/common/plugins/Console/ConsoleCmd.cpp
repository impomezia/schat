/* $Id: ConsoleCmd.cpp 3698 2013-06-17 13:41:51Z IMPOMEZIA $
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

#include "ChatNotify.h"
#include "client/ChatClient.h"
#include "client/ClientCmd.h"
#include "client/ClientFeeds.h"
#include "client/ClientMessages.h"
#include "ConsoleCmd.h"
#include "ConsolePlugin_p.h"
#include "feeds/ConsoleFeed.h"
#include "messages/AlertMessage.h"
#include "net/SimpleID.h"
#include "sglobal.h"

ConsoleCmd::ConsoleCmd(ConsolePluginImpl *plugin)
  : MessagesHook(plugin)
  , m_plugin(plugin)
{
  connect(ChatNotify::i(), SIGNAL(notify(Notify)), SLOT(notify(Notify)));
}


bool ConsoleCmd::command(const QByteArray &dest, const ClientCmd &cmd)
{
  Q_UNUSED(dest)

  const QString command = cmd.command().toLower();
  if (command == LS("console")) {
    m_plugin->show();
  }
  else if (command == LS("cookie")) {
    if (isTalk(dest, command) && isOnline())
      getCookie(dest);
  }
  else
    return false;

  return true;
}


void ConsoleCmd::notify(const Notify &notify)
{
  if (notify.type() == Notify::FeedReply) {
    const FeedNotify &n = static_cast<const FeedNotify &>(notify);
    if (n.channel() == ChatClient::serverId() && n.name() == CONSOLE_FEED_COOKIE_REQ && !m_cookie.isEmpty()) {
      if (n.status() == Notice::OK) {
        const QByteArray id = SimpleID::decode(n.json().value(LS("user")).toString());
        if (m_cookie == id)
          AlertMessage::show(LS("<b>Cookie:</b> ") + n.json().value(CONSOLE_FEED_COOKIE_KEY).toString(), ALERT_MESSAGE_INFO);
      }

      m_cookie.clear();
    }

  }
}


/*!
 * Отправка запроса на получение сookie пользователя.
 */
void ConsoleCmd::getCookie(const QByteArray &id)
{
  if (!(ClientFeeds::match(ChatClient::server(), ChatClient::channel()) & (Acl::Edit | Acl::SpecialEdit))) {
    AlertMessage::show(LS("<b>cookie</b> ") + tr("Not enough rights, you must be a server administrator."), ALERT_MESSAGE_ERROR);
    return;
  }

  ClientFeeds::get(ChatClient::serverId(), CONSOLE_FEED_COOKIE_REQ, SimpleID::encode(id));
  m_cookie = id;
}
