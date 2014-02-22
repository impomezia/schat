/* $Id: ServerTab.cpp 3713 2013-06-25 23:56:26Z IMPOMEZIA $
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

#include <QApplication>
#include <QFile>
#include <QVBoxLayout>
#include <QWebPage>
#include <QWebFrame>

#include "AuthBridge.h"
#include "ChatAlerts.h"
#include "ChatCore.h"
#include "ChatNotify.h"
#include "client/ChatClient.h"
#include "client/SimpleClient.h"
#include "hooks/ChannelMenu.h"
#include "messages/AlertMessage.h"
#include "messages/AlertMessage.h"
#include "messages/ServiceMessage.h"
#include "net/SimpleID.h"
#include "QtEscape.h"
#include "sglobal.h"
#include "ui/ChatIcons.h"
#include "ui/tabs/ChatView.h"
#include "ui/tabs/ProgressTab.h"
#include "ui/tabs/ServerTab.h"
#include "ui/tabs/WelcomeTab.h"
#include "ui/TabWidget.h"
#include "WebBridge.h"

ServerTab::ServerTab(TabWidget *parent)
  : AbstractTab(QByteArray(), LS("server"), parent)
{
  m_options |= CanSendMessage;
  m_options |= NoDelete;
  m_options |= CanBePinned;

  QString file = QApplication::applicationDirPath() + "/styles/test/html/Server.html";
  if (QFile::exists(file))
    file = QUrl::fromLocalFile(file).toString();
  else
    file = "qrc:/html/Server.html";

  m_chatView = new ChatView(QByteArray(), file, this);
  m_auth = new AuthBridge(this);

  QVBoxLayout *mainLay = new QVBoxLayout(this);
  mainLay->addWidget(m_chatView);
  mainLay->setMargin(0);
  mainLay->setSpacing(0);

  setIcon(SCHAT_ICON(Globe));

  connect(ChatClient::i(), SIGNAL(online()), SLOT(online()));
  connect(ChatAlerts::i(), SIGNAL(alert(Alert)), SLOT(alert(Alert)));
  connect(ChatNotify::i(), SIGNAL(notify(Notify)), SLOT(notify(Notify)));
  connect(ChatClient::io(), SIGNAL(clientStateChanged(int,int)), SLOT(clientStateChanged(int)));
  connect(chatView()->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), SLOT(populateJavaScriptWindowObject()));

  retranslateUi();
}


bool ServerTab::bindMenu(QMenu *menu)
{
  Hooks::ChannelMenu::bind(menu, ChatClient::server(), Hooks::TabScope);
  return true;
}


void ServerTab::copy()
{
  m_chatView->copy();
}


void ServerTab::changeEvent(QEvent *event)
{
  AbstractTab::changeEvent(event);

  if (event->type() == QEvent::LanguageChange) {
    WebBridge::i()->retranslate();
    ChatNotify::start(Notify::Language);
  }
}


void ServerTab::alert(const Alert &alert)
{
  if (alert.type() == LS("online")) {
    ServiceMessage message(alert.data().value(LS("popup")).toMap().value(LS("text")).toString());
    message.data().insert(Message::kType,  LS("info"));
    message.data().insert(Message::kExtra, LS("green-text"));

    m_chatView->add(message);
  }
  else if (alert.type() == LS("offline")) {
    ServiceMessage message(alert.data().value(LS("popup")).toMap().value(LS("text")).toString());
    message.data().insert(Message::kType,  LS("info"));
    message.data().insert(Message::kExtra, LS("red-text"));

    m_chatView->add(message);
  }
}


/*!
 * Обработка изменения состояния клиента.
 */
void ServerTab::clientStateChanged(int state)
{
  if (state == ChatClient::WaitAuth) {
    m_auth->start(ChatClient::io()->json().value(CLIENT_PROP_AUTH_SERVER).toString());

    chatView()->add(AlertMessage(tr("Server %1 requires authorization").arg(LS("<b>") + Qt::escape(ChatClient::serverName()) + LS("</b>"))));
    chatView()->evaluateJavaScript(LS("AuthDialog.show();"));
    m_tabs->closePage(PROGRESS_TAB);
    m_tabs->closePage(WELCOME_TAB);

    if (m_tabs->indexOf(this) == -1) {
      m_tabs->addTab(this, QString());
      setOnline();
    }

    m_tabs->setCurrentIndex(m_tabs->indexOf(this));
    ChatNotify::start(Notify::ShowChat);
  }
  else
    chatView()->evaluateJavaScript(LS("AuthDialog.hide();"));

  retranslateUi();
}


void ServerTab::notify(const Notify &notify)
{
  int type = notify.type();
  if (type == Notify::ShowID) {
    if (notify.data().toByteArray() == id())
      AlertMessage::show(LS("<b>Id:</b> ") + SimpleID::encode(id()), ALERT_MESSAGE_INFO, id());
  }
  else if (type == Notify::OpenChannel || type == Notify::OpenInfo) {
    if (notify.data() != id())
      return;

    if (m_tabs->indexOf(this) == -1) {
      m_tabs->addTab(this, QString());
      setOnline();
      retranslateUi();
    }

    m_tabs->setCurrentIndex(m_tabs->indexOf(this));

    if (type == Notify::OpenInfo)
      chatView()->evaluateJavaScript(LS("Pages.setPage(1);"));
    else
      chatView()->evaluateJavaScript(LS("Pages.setPage(0);"));
  }
  if (notify.type() == Notify::ServerRenamed) {
    retranslateUi();
  }
}


void ServerTab::online()
{
  setId(ChatClient::serverId());
  m_chatView->setId(id());
}


void ServerTab::populateJavaScriptWindowObject()
{
  chatView()->page()->mainFrame()->addToJavaScriptWindowObject(LS("Auth"), m_auth);
}


void ServerTab::retranslateUi()
{
  QString name = ChatClient::serverName();
  if (name.isEmpty())
    name = tr("Network");

  setText(name);
}
