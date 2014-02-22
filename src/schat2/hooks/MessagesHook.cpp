/* $Id: MessagesHook.cpp 3485 2013-02-10 09:59:44Z IMPOMEZIA $
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
#include "client/ClientMessages.h"
#include "hooks/MessagesHook.h"
#include "messages/AlertMessage.h"
#include "net/SimpleID.h"
#include "sglobal.h"

MessagesHook::MessagesHook(QObject *parent)
  : Messages(parent)
{
  ChatClient::messages()->add(this);
}


bool MessagesHook::isOnline() const
{
  if (ChatClient::state() == ChatClient::Online)
    return true;

  AlertMessage::show(tr("No connection to the server."), ALERT_MESSAGE_ERROR);
  return false;
}


/*!
 * Возвращает \b true если текущая вкладка является приватным разговором, и \b false если нет.
 * Если вкладка не является приватным разговор, показывается уведомление об этом.
 */
bool MessagesHook::isTalk(const QByteArray &id, const QString &command) const
{
  if (SimpleID::typeOf(id) == SimpleID::UserId)
    return true;

  AlertMessage::show(tr("Command %1 can be executed only in private talks.").arg(LS("<b>") + command + LS("</b> ")), ALERT_MESSAGE_ERROR);
  return false;
}
