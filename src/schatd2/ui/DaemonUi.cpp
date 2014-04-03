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
#include <QDateTime>
#include <QDir>
#include <QFrame>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QProcess>
#include <QPushButton>
#include <QToolBar>

#include "DaemonUi.h"
#include "Path.h"
#include "Settings.h"
#include "sglobal.h"
#include "Translation.h"
#include "version.h"
#include "Storage.h"
#include "cores/Core.h"
#include "feeds/FeedsCore.h"
#include "net/NodePool.h"
#include "NodePlugins.h"

/*!
 * Конструктор класса DaemonUi.
 */
DaemonUi::DaemonUi(QWidget *parent)
  : QDialog(parent, Qt::Tool)
  , m_core(0)
  , m_feeds(0)
  , m_plugins(0)
  , m_pool(0)
  , m_storage(0)
{
  Path::init(LS("schatd2"));

  m_settings = new Settings(Path::data() + LS("/schatd2.conf"), this);

  loadTranslation();

  m_toolBar = new QToolBar(this);
  m_toolBar->setIconSize(QSize(22, 22));

  m_menu = new QMenu();

  createActions();
  createButtons();

  m_controlGroup = new QGroupBox(this);
  QHBoxLayout *controlGroupLay = new QHBoxLayout(m_controlGroup);
  controlGroupLay->addWidget(m_toolBar);
  controlGroupLay->setMargin(2);
  controlGroupLay->setSpacing(0);

  // Отображение статуса
  m_statusLabel = new QLabel(this);
  m_ledLabel = new QLabel(this);
  m_statusGroup = new QGroupBox(this);
  QHBoxLayout *statusGroupLay = new QHBoxLayout(m_statusGroup);
  statusGroupLay->setMargin(2);
  statusGroupLay->setSpacing(0);
  statusGroupLay->addWidget(m_statusLabel);
  statusGroupLay->addStretch();
  statusGroupLay->addWidget(m_ledLabel);

  QHBoxLayout *controlLay = new QHBoxLayout;
  controlLay->addWidget(m_controlGroup);
  controlLay->addWidget(m_statusGroup);

  // Кнопки внизу окна
  QHBoxLayout *bottomLay = new QHBoxLayout;
  bottomLay->addStretch();
  bottomLay->addWidget(m_hideButton);
  bottomLay->addWidget(m_quitButton);

  // Все основные виджеты
  QVBoxLayout *bodyLay = new QVBoxLayout;
  bodyLay->setMargin(6);
  bodyLay->setSpacing(6);
  bodyLay->addLayout(controlLay);
  bodyLay->addLayout(bottomLay);

  // Надпись вверху окна
  m_aboutLabel = new QLabel(QString(
      "<html><body style='color:#333;margin:6px;'>"
      "<h4 style='margin-bottom:0px;'>Simple Chat Daemon %1</h4>"
      "<p style='margin-left:16px;margin-top:5px;'>Copyright © 2008-%2 Alexander Sedov &lt;<a href='mailto:support@schat.me' style='color:#1a4d82;'>support@schat.me</a>&gt;</p>"
      "</body></html>").arg(SCHAT_VERSION).arg(QDateTime::currentDateTime().toString("yyyy")), this);
  m_aboutLabel->setStyleSheet("background:#fff; border:4px solid #fff;");
  m_aboutLabel->setOpenExternalLinks(true);

  QFrame *line2 = new QFrame(this);
  line2->setFrameShape(QFrame::HLine);
  line2->setFrameShadow(QFrame::Sunken);

  // End
  QVBoxLayout *mainLay = new QVBoxLayout(this);
  mainLay->setMargin(0);
  mainLay->setSpacing(0);
  mainLay->addWidget(m_aboutLabel);
  mainLay->addWidget(line2);
  mainLay->addLayout(bodyLay);

  createTray();
  setState(Unknown);

  setWindowIcon(QIcon(":/images/schat16-green.png"));

  retranslateUi();

  QTimer::singleShot(0, this, SLOT(init()));
}


DaemonUi::~DaemonUi()
{
}


void DaemonUi::handleMessage(const QString& message)
{
  QStringList args = message.split(LS(", "));

  if (args.contains(LS("-exit"))) {
    onQuit();
    return;
  }

  if (!arguments(args))
    showUi();
}


void DaemonUi::changeEvent(QEvent *event)
{
  if (event->type() == QEvent::LanguageChange)
    retranslateUi();

  QDialog::changeEvent(event);
}


void DaemonUi::init()
{
  QStringList args = QApplication::arguments();
  args.takeFirst();
  arguments(args);

  onStart();
}


void DaemonUi::onIconActivated(QSystemTrayIcon::ActivationReason reason)
{
  switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::MiddleClick:
      if (isHidden())
        showUi();
      else
        hide();

    default:
      break;
  }
}


void DaemonUi::onListen(const QStringList &hosts)
{
  setState(hosts.isEmpty() ? Error : Started);
}


void DaemonUi::onQuit()
{
  stop();
  QApplication::quit();
}


void DaemonUi::onRestart()
{
  onStop();
  onStart();
}


void DaemonUi::onSettings()
{
  if (isHidden())
    show();
}


void DaemonUi::onStart()
{
  setState(Starting);
}


void DaemonUi::onStop()
{
  setState(Stopping);
}


/*!
 * Запуск сервера.
 */
void DaemonUi::start()
{
  m_feeds = new FeedsCore(this);

  m_storage = new Storage(m_settings, Path::app(), this);
  if (m_storage->start()) {
    m_feeds->deleteLater();
    m_feeds = 0;

    m_storage->deleteLater();
    m_storage = 0;

    setState(Error);
    return;
  }

  m_core    = new Core(this);
  m_plugins = new NodePlugins(this);
  m_plugins->load();

  const QStringList listen = m_settings->value(STORAGE_LISTEN).toStringList();
  const int workers = m_settings->value(STORAGE_WORKERS).toInt();

  m_pool = new NodePool(listen, workers, m_core);
  connect(m_pool, SIGNAL(ready(QObject*)), m_core, SLOT(workerReady(QObject*)));
  connect(m_pool, SIGNAL(listen(QStringList)), SLOT(onListen(QStringList)));

  m_core->start();
  m_pool->start();

  m_storage->load();
}


bool DaemonUi::arguments(const QStringList &args)
{
  Q_UNUSED(args);
//  if (args.contains("-start") && m_startAction->isEnabled()) {
//    start();
//    return true;
//  }
//  else if (args.contains("-stop") && m_stopAction->isEnabled()) {
//    stop();
//    return true;
//  }
//  else if (args.contains("-restart") && m_restartAction->isEnabled()) {
//    restart();
//    return true;
//  }

  return false;
}


/*!
 * Создаёт объекты QAction
 */
void DaemonUi::createActions()
{
  m_actions[StartAction].append(m_menu->addAction(QIcon(":/images/start.png"), tr("Start"), this, SLOT(onStart())));
  m_actions[StopAction].append(m_menu->addAction(QIcon(":/images/stop.png"), tr("Stop"), this, SLOT(onStop())));
  m_actions[RestartAction].append(m_menu->addAction(QIcon(":/images/restart.png"), tr("Restart"), this, SLOT(onRestart())));
  m_actions[SettingsAction].append(m_menu->addAction(QIcon(":/images/gear.png"), tr("Settings..."), this, SLOT(onSettings())));

  m_menu->addSeparator();
  m_menu->addAction(QIcon(LS(":/images/quit.png")), tr("Quit"), this, SLOT(onQuit()));
}


/*!
 * Создаёт кнопки
 */
void DaemonUi::createButtons()
{
  m_hideButton = new QPushButton(QIcon(":/images/dialog-ok.png"), "", this);
  connect(m_hideButton, SIGNAL(clicked(bool)), SLOT(hide()));

  m_quitButton = new QPushButton(QIcon(LS(":/images/quit.png")), "", this);
  connect(m_quitButton, SIGNAL(clicked(bool)), this, SLOT(onQuit()));

  m_actions[StartAction].append(m_toolBar->addAction(QIcon(":/images/start-big.png"), tr("Start"), this, SLOT(onStart())));
  m_actions[StopAction].append(m_toolBar->addAction(QIcon(":/images/stop-big.png"), tr("Stop"), this, SLOT(onStop())));
  m_actions[RestartAction].append(m_toolBar->addAction(QIcon(":/images/restart-big.png"), tr("Restart"), this, SLOT(onRestart())));

  m_toolBar->addSeparator();
  m_actions[SettingsAction].append(m_toolBar->addAction(QIcon(":/images/gear-big.png"), tr("Settings..."), this, SLOT(onSettings())));
}


void DaemonUi::createTray()
{
  m_tray = new QSystemTrayIcon(this);
  m_tray->setIcon(QIcon(":/images/schat16-gray.png"));
  m_tray->setToolTip(QString(LS("Simple Chat Daemon %1")).arg(SCHAT_VERSION));
  m_tray->setContextMenu(m_menu);

  connect(m_tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), SLOT(onIconActivated(QSystemTrayIcon::ActivationReason)));
  m_tray->show();
}


void DaemonUi::loadTranslation()
{
  m_translation = new Translation(this);
  m_translation->setPrefix("schatd2-ui_");
  m_translation->setSearch(QStringList() << (Path::data(Path::SystemScope) + LS("/translations")) << (Path::data() + LS("/translations")));
  m_translation->load(m_settings->value(QLatin1String("Translation")).toString());
}


void DaemonUi::retranslateUi()
{
  setWindowTitle(tr("Server control"));
  m_controlGroup->setTitle(tr("Control"));
  m_statusGroup->setTitle(tr("State"));

  m_hideButton->setText(tr("Hide"));
  m_hideButton->setToolTip(tr("Hide the program window"));
  m_quitButton->setToolTip(tr("Quit"));
}


void DaemonUi::setActionsState(bool start, bool stop, bool restart, bool quit, bool settings)
{
  Q_UNUSED(quit)

  setEnabled(StartAction, start);
  setEnabled(StopAction, stop);
  setEnabled(RestartAction, restart);
  setEnabled(SettingsAction, settings);
}


void DaemonUi::setEnabled(Actions action, bool enabled)
{
  const QList<QAction *> &actions = m_actions[action];
  foreach (QAction *action, actions) {
    action->setEnabled(enabled);
  }
}


void DaemonUi::setLedColor(LedColor color)
{
  QString img;
  QString icon;

  if (color == Green) {
    img = ":/images/led/green.png";
    icon = ":/images/schat16-green.png";
  }
  else if (color == Yellow) {
    img = ":/images/led/yellow.png";
    icon = ":/images/schat16-yellow.png";
  }
  else {
    img = ":/images/led/red.png";
    icon = ":/images/schat16-gray.png";
  }

  m_ledLabel->setPixmap(QPixmap(img));
  m_tray->setIcon(QIcon(icon));
}


void DaemonUi::setState(State state)
{
  if (m_state == state)
    return;

  m_state = state;

  switch (state) {
    case Unknown:
      setActionsState(false, false, false, false, false);
      setLedColor(Yellow);
      m_statusLabel->setText(LS("<b style='color:#bf8c00';>&nbsp;") + tr("Initialization...") + LS("</b>"));
      break;

    case Error:
      setActionsState(false, false, false, false);
      setLedColor();
      m_statusLabel->setText(LS("<b style='color:#c00;'>&nbsp;") + tr("Error") + LS("</b>"));
      stop();
      break;

    case Starting:
      setActionsState(false, false, false);
      setLedColor(Yellow);
      m_statusLabel->setText(LS("<b style='color:#bf8c00';>&nbsp;") + tr("Starting...") + LS("</b>"));
      QTimer::singleShot(25, this, SLOT(start()));
      break;

    case Started:
      setActionsState(false);
      setLedColor(Green);
      m_statusLabel->setText(LS("<b style='color:#090;'>&nbsp;") + tr("Started successfully") + LS("</b>"));
      break;

    case Stopped:
      setActionsState(true, false, true, false);
      setLedColor();
      m_statusLabel->setText(LS("<b style='color:#c00;'>&nbsp;") + tr("Stopped") + LS("</b>"));
      break;

    case Stopping:
      setActionsState(false, false, false, false);
      setLedColor(Yellow);
      m_statusLabel->setText(LS("<b style='color:#bf8c00';>&nbsp;") + tr("Stopping...") + LS("</b>"));
      stop();
      break;

    default:
      break;
  }
}


void DaemonUi::showUi()
{
  show();
  activateWindow();
}


/*!
 * Остановка сервера.
 */
void DaemonUi::stop()
{
  if (m_pool) {
    m_pool->quit();
    m_pool->wait();
    delete m_pool;
    m_pool = 0;
  }

  if (m_core) {
    m_core->quit();
    delete m_core;
    m_core = 0;
  }

  if (m_storage) {
    delete m_storage;
    m_storage = 0;
  }

  if (m_plugins) {
    delete m_plugins;
    m_plugins = 0;
  }

  if (m_feeds) {
    delete m_feeds;
    m_feeds = 0;
  }

//  #ifndef SCHATD_NO_SERVICE
//  if (m_controller->isInstalled()) {
//    m_client->leave();
//    m_controller->stop();
//  }
//  else
//    m_client->exit();
//  #else
//  m_client->leave();
//  if (!QProcess::startDetached('"' + m_daemonFile + "\" -terminate")) {
//    setStatus(Error);
//    return;
//  }
//  #endif

  setState(Stopped);
}
