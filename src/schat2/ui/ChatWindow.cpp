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

#include <QApplication>
#include <QDesktopWidget>
#include <QFile>
#include <QKeyEvent>
#include <QTimer>
#include <QVBoxLayout>

#if QT_VERSION >= 0x050200
# include <QWinEvent>
#endif

#include "ChatAlerts.h"
#include "ChatCore.h"
#include "ChatNotify.h"
#include "ChatSettings.h"
#include "client/ChatClient.h"
#include "client/SimpleClient.h"
#include "debugstream.h"
#include "sglobal.h"
#include "ui/ChatWindow.h"
#include "ui/SendWidget.h"
#include "ui/StatusBar.h"
#include "ui/StatusMenu.h"
#include "ui/tabs/AbstractTab.h"
#include "ui/TabWidget.h"

#if defined(Q_OS_WIN32) && QT_VERSION < 0x050000
# include <qt_windows.h>
# include "qtwin/qtwin.h"
#endif

#if defined(SCHAT_OPTION)
# undef SCHAT_OPTION
# define SCHAT_OPTION(x) m_settings->value(QLatin1String(x))
#endif

ChatWindow::ChatWindow(QWidget *parent)
  : QMainWindow(parent)
  , m_settings(ChatCore::settings())
{
  setObjectName(LS("ChatWindow"));

  m_desktop = new QDesktopWidget();
  new StatusMenu(this);

  m_central = new QWidget(this);
  m_tabs = new TabWidget(this);
  m_statusBar = new StatusBar(this);
  m_send = new SendWidget(this);
  m_send->setVisible(false);

  setStatusBar(m_statusBar);

  m_mainLay = new QVBoxLayout(m_central);
  m_mainLay->addWidget(m_tabs);
  m_mainLay->addWidget(m_send);
  m_mainLay->setStretchFactor(m_tabs, 999);
  m_mainLay->setStretchFactor(m_send, 1);
  m_mainLay->setSpacing(0);

# if defined(Q_OS_WIN)
  m_mainLay->setContentsMargins(3, 3, 3, 0);
#else
  m_mainLay->setMargin(0);
#endif

  setMinimumSize(400, 300);
  setCentralWidget(m_central);
  stylize();
  restoreGeometry();

  connect(m_send, SIGNAL(send(QString)), ChatCore::i(), SLOT(send(QString)));
  connect(m_settings, SIGNAL(changed(QString,QVariant)), SLOT(settingsChanged(QString,QVariant)));
  connect(m_tabs, SIGNAL(pageChanged(AbstractTab*)), SLOT(pageChanged(AbstractTab*)));
  connect(ChatNotify::i(), SIGNAL(notify(Notify)), SLOT(notify(Notify)));

  setWindowTitle(QApplication::applicationName());

  setupAppIcon();
}


ChatWindow::~ChatWindow()
{
  delete m_desktop;
}


/*!
 * Закрытие чата.
 */
void ChatWindow::closeChat()
{
  ChatClient::io()->leave();
  hideChat();

  QApplication::quit();
}


void ChatWindow::showChat()
{
  SCHAT_DEBUG_STREAM(this << "showChat()")

  if (!m_settings->value(SETTINGS_MAXIMIZED).toBool()) {
    setWindowState(windowState() & ~Qt::WindowMinimized);
    show();
  }
  else
    showMaximized();

  activateWindow();
  m_tabs->stopAlert();

  if (m_send->isVisible())
    m_send->setInputFocus();
}


bool ChatWindow::event(QEvent *event)
{
# if QT_VERSION >= 0x050200
  if (event->type() == QWinEvent::CompositionChange || event->type() == QWinEvent::ColorizationChange)
    stylize();
# endif

  return QMainWindow::event(event);
}


void ChatWindow::changeEvent(QEvent *event)
{
  if (event->type() == QEvent::LanguageChange)
    retranslateUi();
  else if (event->type() == QEvent::ActivationChange)
    m_activationChanged = QTime::currentTime();

  QMainWindow::changeEvent(event);
}


void ChatWindow::closeEvent(QCloseEvent *event)
{
  SCHAT_DEBUG_STREAM(this << "closeEvent()")

  hideChat();
  QMainWindow::closeEvent(event);
}


void ChatWindow::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Escape)
    hideChat();
  else
    QMainWindow::keyPressEvent(event);
}


void ChatWindow::moveEvent(QMoveEvent *event)
{
  QMainWindow::moveEvent(event);

  saveGeometry();
}


void ChatWindow::resizeEvent(QResizeEvent *event)
{
  QMainWindow::resizeEvent(event);

  saveGeometry();
}


#if defined(Q_OS_WIN32) && QT_VERSION < 0x050000
bool ChatWindow::winEvent(MSG *message, long *result)
{
  if (message && (message->message == QtWin::CompositionChange || message->message == QtWin::ColorizationChange)) {
    stylize();
  }

  return QMainWindow::winEvent(message, result);
}
#endif


void ChatWindow::notify(const Notify &notify)
{
  if (notify.type() == Notify::Quit) {
    closeChat();
  }
  else if (notify.type() == Notify::Restart) {
    ChatClient::io()->leave();
    hideChat();
    emit restartRequest();
  }
  else if (notify.type() == Notify::ToggleVisibility) {
    if (TabWidget::isActiveChatWindow() || qAbs(m_activationChanged.msecsTo(QTime::currentTime())) < QApplication::doubleClickInterval()) {
      hideChat();
    }
    else {
      m_tabs->channelTab(ChatAlerts::last(), false);
      showChat();
    }
  }
  else if (notify.type() == Notify::ShowChat) {
    showChat();
  }
  else if (notify.type() == Notify::ForceShowChat) {
    showChat();
#   if defined(Q_OS_WIN) && QT_VERSION < 0x50000
    HWND hWnd = window()->internalWinId();
    ShowWindow(hWnd, SW_MINIMIZE);
    ShowWindow(hWnd, SW_RESTORE);
#   endif
  }
}


void ChatWindow::pageChanged(AbstractTab *tab)
{
  if (tab->options() & AbstractTab::CanSendMessage) {
    m_send->setVisible(true);
    m_send->setInputFocus();
  }
  else
    m_send->setVisible(false);
}


void ChatWindow::settingsChanged(const QString &key, const QVariant &value)
{
  if (key == SETTINGS_MAXIMIZED) {
    if (value.toBool())
      showMaximized();
    else
      showNormal();
  }
}


QString ChatWindow::geometryKey() const
{
  const int count = m_desktop->screenCount();
  QString out;
  for (int i = 0; i < count; ++i) {
    QRect r = m_desktop->screenGeometry(i);
    out += QString::number(r.x()) + LC('.') + QString::number(r.y()) + LC('.') + QString::number(r.width()) + LC('x') + QString::number(r.height());
  }

  return out;
}


/*!
 * Скрытие окна чата.
 */
void ChatWindow::hideChat()
{
  SCHAT_DEBUG_STREAM(this << "hideChat()")

  m_settings->setValue(SETTINGS_MAXIMIZED, isMaximized(), false);
  hide();
}


void ChatWindow::retranslateUi()
{
}


void ChatWindow::restoreGeometry()
{
  const QVariantList data = m_settings->value(LS("Geometry/") + geometryKey()).toList();

  if (data.size() == 4) {
    move(data.at(0).toInt(), data.at(1).toInt());
    resize(data.at(2).toInt(), data.at(3).toInt());
  } else
    resize(666, 440);
}


void ChatWindow::saveGeometry()
{
  if (isMaximized() || m_settings->value(SETTINGS_MAXIMIZED).toBool())
    return;

  QVariantList data;
  data.append(pos().x());
  data.append(pos().y());
  data.append(width());
  data.append(height());

  m_settings->setValue(LS("Geometry/") + geometryKey(), data, false, true);
}
