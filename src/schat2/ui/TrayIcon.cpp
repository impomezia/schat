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

#include <QBasicTimer>
#include <QMenu>
#include <QTimer>
#include <QTimerEvent>

#include "ChatAlerts.h"
#include "ChatCore.h"
#include "ChatNotify.h"
#include "ChatSettings.h"
#include "client/ChatClient.h"
#include "client/SimpleClient.h"
#include "sglobal.h"
#include "ui/AlertsPixmap.h"
#include "ui/ChatIcons.h"
#include "ui/StatusMenu.h"
#include "ui/TrayIcon.h"

TrayIcon *TrayIcon::m_self = 0;

TrayIcon::TrayIcon(QObject *parent)
  : QSystemTrayIcon(parent)
  , m_alertIcon(0)
{
  m_self = this;

  m_staticAlerts = ChatCore::settings()->value(LS("Labs/StaticTrayAlerts")).toBool();

  m_menu = new QMenu();
  m_timer = new QBasicTimer();
  m_show = m_menu->addAction(SCHAT_ICON(SmallLogo), tr("Show..."));
  m_menu->addSeparator();

  m_menu->addMenu(StatusMenu::i());
  m_menu->addSeparator();

  m_settings = m_menu->addAction(SCHAT_ICON(Settings), tr("Preferences..."));
  m_about    = m_menu->addAction(SCHAT_ICON(SmallLogo), tr("About..."));
  m_menu->addSeparator();
  m_quit     = m_menu->addAction(SCHAT_ICON(Quit), tr("Quit"));

  setContextMenu(m_menu);

  connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
  connect(StatusMenu::i(), SIGNAL(updated()), SLOT(reload()));
  connect(m_menu, SIGNAL(triggered(QAction*)), SLOT(triggered(QAction*)));
  connect(ChatAlerts::i(), SIGNAL(alert(bool)), SLOT(alert(bool)));
  connect(ChatCore::settings(), SIGNAL(changed(QString,QVariant)), SLOT(changed(QString,QVariant)));
  connect(ChatAlerts::i(), SIGNAL(countChanged(int,int,QByteArray)), SLOT(reload()));

  reload();

  QTimer::singleShot(0, this, SLOT(show()));
  QTimer::singleShot(1000, this, SLOT(show()));
}


TrayIcon::~TrayIcon()
{
  if (m_timer->isActive())
    m_timer->stop();

  delete m_timer;
  delete m_menu;
}


void TrayIcon::retranslateUi()
{
  m_show->setText(tr("Show..."));
  m_settings->setText(tr("Preferences..."));
  m_about->setText(tr("About..."));
  m_quit->setText(tr("Quit"));
}


void TrayIcon::timerEvent(QTimerEvent *event)
{
  if (event->timerId() == m_timer->timerId()) {
    if (m_alertIcon) {
      m_alertIcon = 0;
      setIcon(m_icon);
    }
    else {
      startAlert();
    }

    return;
  }

  QSystemTrayIcon::timerEvent(event);
}


void TrayIcon::alert(bool start)
{
  if (!start) {
    m_timer->stop();
    setIcon(m_icon);
    m_alertIcon = 0;
  }
  else if (!m_timer->isActive() || m_staticAlerts) {
    startAlert();
  }
}


/*!
 * Обработка изменения настроек.
 */
void TrayIcon::changed(const QString &key, const QVariant &value)
{
  if (key == LS("Labs/StaticTrayAlerts")) {
    m_staticAlerts = value.toBool();

    if (m_alertIcon) {
      m_alertIcon = 0;
      startAlert();
    }
  }
}


void TrayIcon::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
  if (reason == QSystemTrayIcon::Trigger || reason == QSystemTrayIcon::MiddleClick) {
    ChatNotify::start(Notify::ToggleVisibility);
  }
}


/*!
 * Обновление состояния в зависимости от статуса пользователя и подключения.
 */
void TrayIcon::reload()
{
  int status = ChatClient::channel()->status().value();
  if (ChatClient::state() != ChatClient::Online || ChatAlerts::total())
    status = -1;

  m_icon = SCHAT_ICON(SmallLogo);
  if (status != -1)
    m_icon = ChatIcons::icon(m_icon, ChatIcons::overlay(status));

  if (ChatAlerts::total())
    m_icon = AlertsPixmap::icon(m_icon, ChatAlerts::total());

  if (!m_timer->isActive())
    setIcon(m_icon);
}


void TrayIcon::triggered(QAction *action)
{
  if (action == m_settings) {
    ChatNotify::start(Notify::OpenSettings);
    ChatNotify::start(Notify::ShowChat);
  }
  else if (action == m_about) {
    ChatNotify::start(Notify::OpenAbout);
    ChatNotify::start(Notify::ShowChat);
  }
  else if (action == m_quit)
    ChatNotify::start(Notify::Quit);
  else if (action == m_show)
    ChatNotify::start(Notify::ShowChat);
}


/*!
 * Смена иконки в трее на иконку уведомления.
 */
void TrayIcon::startAlert()
{
  if (m_alertIcon)
    return;

  show();
  m_alertIcon = 1;

  if (m_staticAlerts) {
    if (m_timer->isActive())
      m_timer->stop();

    return;
  }

  setIcon(AlertsPixmap::icon(QIcon(LS(":/images/message-active.png")), ChatAlerts::total()));

  if (!m_timer->isActive())
    m_timer->start(666, this);
}
