/* $Id: StatusBar.cpp 3698 2013-06-17 13:41:51Z IMPOMEZIA $
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

#include <QAction>
#include <QApplication>
#include <QEvent>
#include <QLabel>
#include <QMenu>
#include <QMouseEvent>
#include <QWidgetAction>

#include "ChatCore.h"
#include "ChatNotify.h"
#include "client/ChatClient.h"
#include "client/SimpleClient.h"
#include "hooks/ChannelMenu.h"
#include "net/packets/Notice.h"
#include "NetworkManager.h"
#include "QtEscape.h"
#include "sglobal.h"
#include "ui/BgOperationWidget.h"
#include "ui/LoginIcon.h"
#include "ui/network/NetworkWidget.h"
#include "ui/Spinner.h"
#include "ui/StatusBar.h"
#include "ui/StatusWidget.h"

StatusBar *StatusBar::m_self = 0;

StatusBar::StatusBar(QWidget *parent)
  : QStatusBar(parent)
{
  m_self = this;

  m_spinner = new Spinner(this);
  m_icon = new QLabel(this);
  m_login = new LoginIcon(this);
  m_secure = new QLabel(this);
  m_label = new QLabel(this);

# if !defined(SCHAT_NO_SSL)
  m_secure->setPixmap(QPixmap(LS(":/images/secure.png")));
# endif

  m_url = new NetworkWidget(this, NetworkWidget::BasicLayout);
  m_url->setMinimumWidth(m_url->width() * 2 + 50);
  m_urlAction = new QWidgetAction(this);
  m_urlAction->setDefaultWidget(m_url);

  m_bg = new BgOperationWidget(this);
  m_status = new StatusWidget(this);

  addWidget(m_spinner);
  addWidget(m_icon);
  addWidget(m_login);
  addWidget(m_secure);
  addWidget(m_label, 1);
  addWidget(m_bg);
  addPermanentWidget(m_status);

  connect(ChatClient::io(), SIGNAL(clientStateChanged(int,int)), SLOT(clientStateChanged(int)));
  connect(ChatNotify::i(), SIGNAL(notify(Notify)), SLOT(notify(Notify)));

  updateStyleSheet();
  clientStateChanged(SimpleClient::ClientOffline);
}


bool StatusBar::event(QEvent *event)
{
  #if defined(Q_OS_WIN32)
  if (event->type() == QEvent::ApplicationPaletteChange) {
    updateStyleSheet();
  }
  #endif

  return QStatusBar::event(event);
}


void StatusBar::changeEvent(QEvent *event)
{
  if (event->type() == QEvent::LanguageChange)
    retranslateUi();

  QStatusBar::changeEvent(event);
}


/*!
 * Показ меню.
 */
void StatusBar::mouseReleaseEvent(QMouseEvent *event)
{
  if (childAt(event->pos()) == m_icon || event->button() == Qt::RightButton)
    menu(event->globalPos());
  else
    QStatusBar::mouseReleaseEvent(event);
}


void StatusBar::clientStateChanged(int state)
{
  if (state == SimpleClient::ClientConnecting || state == SimpleClient::ClientWaitAuth) {
    m_icon->setVisible(false);
    m_spinner->setVisible(true);
    m_spinner->startAnimation();
  }
  else {
    m_icon->setVisible(true);
    m_spinner->setVisible(false);
    m_spinner->stopAnimation();
  }

  if (state != SimpleClient::ClientOnline)
    m_secure->setVisible(false);

  if (state == SimpleClient::ClientOffline) {
    m_icon->setPixmap(QPixmap(LS(":/images/offline.png")));
  }
  else if (state == SimpleClient::ClientOnline) {
    m_icon->setPixmap(QPixmap(LS(":/images/online.png")));

#   if !defined(SCHAT_NO_SSL)
    if (ChatClient::io()->isEncrypted())
      m_secure->setVisible(true);
#   endif
  }
  else if (state == SimpleClient::ClientError) {
    m_icon->setPixmap(QPixmap(LS(":/images/exclamation-red.png")));
  }

  retranslateUi();
}


void StatusBar::menuTriggered(QAction *action)
{
  if (action == m_connect)
    m_url->open();
}


void StatusBar::notify(const Notify &notify)
{
  if (notify.type() == Notify::ServerRenamed)
    setServerName();
}


void StatusBar::menu(const QPoint &point)
{
  QMenu menu(this);
  if (ChatClient::state() == ChatClient::Offline || ChatClient::state() == ChatClient::Error)
    menu.addAction(m_urlAction);

  QAction *action = m_url->connectAction();
  m_connect = menu.addAction(action->icon(), action->text());

  menu.addSeparator();
  Hooks::ChannelMenu::bind(&menu, ChatClient::server(), Hooks::StatusBarScope);

  connect(&menu, SIGNAL(triggered(QAction*)), SLOT(menuTriggered(QAction*)));
  menu.exec(point);
}


void StatusBar::retranslateUi()
{
  m_secure->setToolTip(tr("Encrypted connection"));
  int state = ChatClient::state();

  if (state == ChatClient::Offline) {
    m_label->setText(tr("No connection"));
    m_icon->setToolTip(tr("No connection"));
  }
  else if (state == ChatClient::Connecting) {
    m_label->setText(ChatClient::io()->url().toString());
  }
  else if (state == ChatClient::Online) {
    setServerName();
    m_icon->setToolTip(tr("Connected"));
  }
  else if (state == ChatClient::Error) {
    setError();
  }
  else if (state == ChatClient::WaitAuth)
    setServerName();
}


void StatusBar::setError()
{
  QVariantMap error = ChatClient::io()->json().value(LS("error")).toMap();
  QString type = error.value(LS("type")).toString();
  if (type == LS("auth")) {
    m_label->setText(tr("Authorization Error"));
    m_icon->setToolTip(Notice::status(error.value(LS("auth")).toInt()));
  }
  else if (type == LS("dns")) {
    m_label->setText(tr("Server not found‎"));
    m_icon->setToolTip(QString());
  }
  else {
    m_label->setText(tr("Error"));
    m_icon->setToolTip(QString());
  }
}


void StatusBar::setServerName()
{
  m_label->setText(Qt::escape(ChatClient::serverName()));
}


void StatusBar::updateStyleSheet()
{
  #if defined(Q_OS_MAC)
  setStyleSheet(LS("QStatusBar { background: qlineargradient(x1: 1, y1: 0, x2: 1, y2: 1, stop: 0 #ededed, stop: 1 #c8c8c8); } QStatusBar::item { border-width: 0; }"));
  #else
    #if defined(Q_OS_WIN32)
    setStyleSheet(QString(LS("QStatusBar { background-color: %1; } QStatusBar::item { border-width: 0; }")).arg(parentWidget()->palette().color(QPalette::Window).name()));
    #else
    setStyleSheet(LS("QStatusBar::item { border-width: 0; }"));
    #endif
  #endif
}

