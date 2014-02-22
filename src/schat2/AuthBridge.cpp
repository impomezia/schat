/* $Id: AuthBridge.cpp 3767 2013-08-13 22:30:40Z IMPOMEZIA $
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

#include "AuthBridge.h"
#include "ChatNotify.h"
#include "client/AuthClient.h"
#include "client/ChatClient.h"
#include "client/SimpleClient.h"
#include "messages/AlertMessage.h"
#include "net/packets/auth.h"
#include "net/SimpleID.h"
#include "sglobal.h"
#include "ui/tabs/ChatView.h"
#include "ui/tabs/ServerTab.h"
#include "ui/TabWidget.h"

AuthBridge::AuthBridge(QObject *parent)
  : QObject(parent)
  , m_client(0)
{
}


bool AuthBridge::anonymous() const
{
  return ChatClient::io()->json().value(CLIENT_PROP_ANONYMOUS).toBool();
}


/*!
 * Обработка закрытия авторизационного диалога.
 *
 * \sa AuthClient::cancel()
 */
void AuthBridge::cancel()
{
  if (ChatClient::state() == ChatClient::WaitAuth) {
    if (m_client)
      m_client->cancel();

    m_providers.clear();
    ChatClient::io()->leave();

    add(AlertMessage(tr("Authorization has been canceled by you. %1").arg(retryLink()), ALERT_MESSAGE_ERROR));
  }
}


/*!
 * Открытие нового анонимного подключения.
 */
void AuthBridge::open()
{
  ChatClient::io()->setAuthType(AuthRequest::Anonymous);
  ChatClient::open(ChatClient::io()->url());
}


void AuthBridge::retry()
{
  ChatClient::open();
}


void AuthBridge::start(const QString &url)
{
  if (url.isEmpty())
    return;

  if (!m_client) {
    m_client = new AuthClient(this);
    connect(m_client, SIGNAL(providersReady(QVariantMap)), SLOT(providersReady(QVariantMap)));
    connect(m_client, SIGNAL(forbidden()), SLOT(forbidden()));
    connect(m_client, SIGNAL(ready(QString,QByteArray,QByteArray,QVariantMap)), SLOT(ready(QString,QByteArray,QByteArray)));
    connect(m_client, SIGNAL(timeout()), SLOT(timeout()));
  }

  m_providers.clear();
  m_client->start(url);
}


void AuthBridge::forbidden()
{
  ChatClient::io()->leave();

  add(AlertMessage(tr("Access denied. %1").arg(retryLink()), ALERT_MESSAGE_ERROR));
  ChatNotify::start(Notify::ShowChat);
}


void AuthBridge::providersReady(const QVariantMap &data)
{
  m_providers = data;
  emit providersReady();
}


void AuthBridge::ready(const QString &provider, const QByteArray &id, const QByteArray &cookie)
{
  Q_UNUSED(provider)
  Q_UNUSED(id)

  if (SimpleID::typeOf(cookie) != SimpleID::CookieId)
    return;

  ChatClient::io()->setAuthType(AuthRequest::External);
  ChatClient::io()->setAuthId(SimpleID::decode(m_client->state()));
  ChatClient::io()->openUrl(ChatClient::io()->url(), cookie);

  const QString htmlName = m_providers.value(LS("providers")).toMap().value(provider).toMap().value(LS("htmlName")).toString();
  if (!htmlName.isEmpty())
    add(AlertMessage(tr("You have successfully logged in using <b>%1</b>").arg(htmlName), ALERT_MESSAGE_SUCCESS));

  ChatNotify::start(Notify::ForceShowChat);
}


void AuthBridge::timeout()
{
  ChatClient::io()->leave();
  add(AlertMessage(tr("Exceeded number of attempts to check authorization. %1").arg(retryLink()), ALERT_MESSAGE_ERROR));
}


QString AuthBridge::retryLink() const
{
  return LS("<a href=\"#\" class=\"retry-auth\">") + tr("Try again?") + LS("</a>");
}


void AuthBridge::add(const AlertMessage &message)
{
  TabWidget::i()->serverTab()->chatView()->add(message);
}
