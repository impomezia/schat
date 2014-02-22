/* $Id: ConsoleView.cpp 3775 2013-08-24 03:41:07Z IMPOMEZIA $
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

#include <QDesktopServices>
#include <QWebFrame>

#include "ChatNotify.h"
#include "client/ChatClient.h"
#include "client/ClientChannels.h"
#include "net/SimpleID.h"
#include "sglobal.h"
#include "ui/ConsoleView.h"
#include "WebBridge.h"

ConsoleView::ConsoleView(QWidget *parent)
  : WebView(parent)
{
  setAcceptDrops(false);

  connect(page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), SLOT(populateJavaScriptWindowObject()));
  connect(ChatNotify::i(), SIGNAL(notify(Notify)), SLOT(notify(Notify)));
}


bool ConsoleView::setName(const QString &channelId, const QString &name)
{
  return ChatClient::channels()->name(SimpleID::decode(channelId), name);
}


QString ConsoleView::expirationText(int days) const
{
  return tr("Certificate expires in %n days", "", days);
}


QString ConsoleView::toPassword(const QString &password) const
{
  return SimpleID::encode(SimpleID::make(password.toUtf8(), SimpleID::PasswordId));
}


void ConsoleView::openUrl(const QString &url)
{
  QDesktopServices::openUrl(QUrl(url));
}


void ConsoleView::notify(const Notify &notify)
{
  int type = notify.type();
  if (type == Notify::FeedData || type == Notify::FeedReply) {
    const FeedNotify &n = static_cast<const FeedNotify &>(notify);
    if (n.channel() != ChatClient::serverId())
      return;

    QVariantMap json = WebBridge::feed(n);
    if (json.isEmpty())
      return;

    emit feed(json);
  }
  else if (type == Notify::ServerRenamed) {
    emit serverRenamed(ChatClient::serverName());
  }
}


void ConsoleView::populateJavaScriptWindowObject()
{
  page()->mainFrame()->addToJavaScriptWindowObject(LS("SimpleChat"), WebBridge::i());
  page()->mainFrame()->addToJavaScriptWindowObject(LS("ConsoleView"), this);
}
