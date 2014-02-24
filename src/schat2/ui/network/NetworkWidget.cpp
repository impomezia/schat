/* Simple Chat
 * Copyright (c) 2008-2014 Alexander Sedov <imp@schat.me>
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

#include <QEvent>
#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QMenu>
#include <QTimer>
#include <QToolBar>
#include <QToolButton>

#include "Account.h"
#include "ChatCore.h"
#include "ChatNotify.h"
#include "client/ChatClient.h"
#include "client/ClientFeeds.h"
#include "client/SimpleClient.h"
#include "net/packets/auth.h"
#include "net/SimpleID.h"
#include "NetworkManager.h"
#include "sglobal.h"
#include "ui/ChatIcons.h"
#include "ui/network/NetworkComboBox.h"
#include "ui/network/NetworkWidget.h"
#include "ui/SLineEdit.h"

NetworkWidget::NetworkWidget(QWidget *parent, int options)
  : QWidget(parent)
  , m_options(options)
  , m_manager(ChatCore::networks())
  , m_edit(0)
  , m_urlLabel(0)
  , m_urlEdit(0)
{
  m_combo = new NetworkComboBox(this);
  m_combo->installEventFilter(this);

  createActionsButton();

  m_toolBar = new QToolBar(this);
  m_toolBar->setIconSize(QSize(16, 16));
  m_toolBar->addWidget(m_actions);

  if (m_options & BigButton)
    m_toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

  m_connect = m_toolBar->addAction(QString(), this, SLOT(open()));
  m_toolBar->setStyleSheet(LS("QToolBar { margin:0px; border:0px; }"));

  m_layout = new QGridLayout(this);
  m_layout->addWidget(m_combo, 0, 0, 1, 2);
  m_layout->addWidget(m_toolBar, 0, 2);
  m_layout->setColumnStretch(1, 1);
  m_layout->setMargin(0);

  if (m_options & ExtraLayout) {
    m_urlLabel = new QLabel(tr("Server url:"), this);
    m_urlEdit  = new SLineEdit(this);
    m_layout->addWidget(m_urlLabel, 1, 0);
    m_layout->addWidget(m_urlEdit, 1, 1);

    connect(m_urlEdit, SIGNAL(returnPressed()), SLOT(applyUrl()));
  }

  m_combo->load();

  connect(m_combo, SIGNAL(currentIndexChanged(int)), SLOT(indexChanged(int)));
  connect(ChatClient::io(), SIGNAL(clientStateChanged(int,int)), SLOT(reload()));

  reload();
}


QAction *NetworkWidget::connectAction()
{
  const int state = m_manager->isSelectedActive();

  if (state == 1) {
    m_connect->setIcon(SCHAT_ICON(Disconnect));
    m_connect->setText(tr("Disconnect"));
  }
  else if (state == 2) {
    m_connect->setIcon(SCHAT_ICON(Disconnect));
    m_connect->setText(tr("Abort"));
  }
  else {
    m_connect->setIcon(SCHAT_ICON(Connect));
    m_connect->setText(tr("Connect"));
  }

  return m_connect;
}


/*!
 * Обработка действия для \p m_connect.
 */
void NetworkWidget::open()
{
  QMenu *popup = qobject_cast<QMenu *>(parentWidget());
  if (popup && isVisible())
    popup->close();

  int index = m_combo->currentIndex();
  if (index == -1)
    return;

  if (m_urlEdit && m_urlEdit->isVisible()) {
    Network item = m_manager->item(m_manager->selected());
    item->setUrl(m_urlEdit->text());
  }

  if (m_manager->isSelectedActive()) {
    ChatClient::io()->leave();
    return;
  }

  m_combo->open();
}


bool NetworkWidget::eventFilter(QObject *watched, QEvent *event)
{
  if (watched == m_combo && event->type() == QEvent::KeyPress) {
    QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
    if (keyEvent->key() == Qt::Key_Return) {
      open();
      return true;
    }
  }

  return QWidget::eventFilter(watched, event);
}


void NetworkWidget::changeEvent(QEvent *event)
{
  if (event->type() == QEvent::LanguageChange)
    retranslateUi();

  QWidget::changeEvent(event);
}


void NetworkWidget::applyUrl()
{
  int index = m_combo->currentIndex();
  if (index == -1 || !m_urlEdit || !m_urlEdit->isVisible())
    return;

  Network item = m_manager->item(m_manager->selected());
  if (item->url() == m_urlEdit->text())
    return;

  item->setUrl(m_urlEdit->text());

  if (m_manager->isSelectedActive())
    ChatClient::io()->leave();

  m_combo->open();
}


/*!
 * Обработка изменения текущего индекса.
 */
void NetworkWidget::indexChanged(int index)
{
  if (index == -1)
    return;

  reload();
}


void NetworkWidget::menuTriggered(QAction *action)
{
  if (action == m_computers) {
    ChatNotify::start(Notify::OpenInfo, ChatClient::serverId());
  }
  else if (action == m_signOut) {
    signOut();
  }
}


void NetworkWidget::reload()
{
  connectAction();

  if (!m_urlEdit)
    return;

  Network item = m_manager->item(m_manager->selected());
  m_urlEdit->setText(item->url());

  const bool visible = item->id() != m_manager->tmpId();
  m_urlLabel->setVisible(visible);
  m_urlEdit->setVisible(visible);
}


/*!
 * Показ меню.
 */
void NetworkWidget::showMenu()
{
  if (m_edit) {
    int index = m_combo->currentIndex();
    if (index == -1 || m_combo->itemData(index).type() != QVariant::ByteArray || m_combo->isEditable())
      m_edit->setVisible(false);
    else
      m_edit->setVisible(true);
  }

  bool computers = ChatClient::state() == ChatClient::Online && !m_combo->isEditable();
  m_computers->setVisible(computers);
  m_signOut->setVisible(computers);
}


void NetworkWidget::createActionsButton()
{
  m_menu = new QMenu(this);

  m_menu->addSeparator();

  if (!(m_options & ExtraLayout)) {
    m_edit = m_menu->addAction(SCHAT_ICON(TopicEdit), tr("Edit"), m_combo, SLOT(edit()));
    m_menu->addSeparator();
  }

  m_add = m_menu->addAction(SCHAT_ICON(Add), tr("Add"), m_combo, SLOT(add()));
  m_remove = m_menu->addAction(SCHAT_ICON(Remove), tr("Remove"), m_combo, SLOT(remove()));
  m_menu->addSeparator();
  m_signOut = m_menu->addAction(SCHAT_ICON(SignOut), tr("Sign out"));
  m_computers = m_menu->addAction(SCHAT_ICON(Computer), tr("My Computers"));

  m_actions = new QToolButton(this);
  m_actions->setIcon(SCHAT_ICON(Gear));
  m_actions->setMenu(m_menu);
  m_actions->setPopupMode(QToolButton::InstantPopup);
  m_actions->setToolTip(tr("Actions"));
  m_actions->setStyleSheet(LS("QToolButton::menu-indicator {image:none}"));

  connect(m_menu, SIGNAL(aboutToShow()), SLOT(showMenu()));
  connect(m_menu, SIGNAL(triggered(QAction*)), SLOT(menuTriggered(QAction*)));
}


void NetworkWidget::retranslateUi()
{
  connectAction();

  if (m_edit)
    m_edit->setText(tr("Edit"));

  m_add->setText(tr("Add"));
  m_remove->setText(tr("Remove"));
  m_signOut->setText(tr("Sign out"));
  m_computers->setText(tr("My Computers"));
  m_actions->setToolTip(tr("Actions"));

  if (m_urlLabel)
    m_urlLabel->setText(tr("Server url:"));
}


void NetworkWidget::signOut()
{
  if (ChatClient::state() != ChatClient::Online)
    return;

  ClientFeeds::del(ChatClient::id(), LS("hosts/") + SimpleID::encode(ChatClient::io()->json().value(CLIENT_PROP_HOST_ID).toByteArray()), 2);
  ChatClient::io()->setAuthType(AuthRequest::Discovery);
}
