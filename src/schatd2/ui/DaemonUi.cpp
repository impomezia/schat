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

/*!
 * Конструктор класса DaemonUi.
 */
DaemonUi::DaemonUi(QWidget *parent)
  : QDialog(parent, Qt::Tool)
{
  Path::init(LS("schatd2"));

  m_settings = new Settings(QApplication::applicationDirPath() + "/schatd.conf", this);

  loadTranslation();

  m_checkTimer.setInterval(5000);
  connect(&m_checkTimer, SIGNAL(timeout()), SLOT(checkStart()));

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
  setStatus(Unknown);

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
    QApplication::quit();
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


void DaemonUi::checkStart()
{
  if (m_status == Starting || m_status == Restarting)
    setStatus(Error);
}


void DaemonUi::exit()
{
  stop();
  QApplication::quit();
}


void DaemonUi::iconActivated(QSystemTrayIcon::ActivationReason reason)
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


void DaemonUi::init()
{
//  if (!QFile::exists(m_daemonFile)) {
//    setStatus(Error);
//    return;
//  }

//  m_settings->read();
//  m_translation->load(m_settings->getString("Translation"));
//  retranslateUi();

//  #if !defined(SCHAT_NO_STYLE)
//  QString schatConf;
//  if (AbstractSettings::isUnixLike()) {
//    schatConf = SCHAT_UNIX_CONFIG("schat.conf");
//  }
//  else {
//    schatConf = QApplication::applicationDirPath() + "/schat.conf";
//  }

//  QSettings s(schatConf, QSettings::IniFormat, this);
//  QApplication::setStyle(s.value("Style", "Plastique").toString());
//  #endif

//  m_client = new LocalClientService(this);
//  connect(m_client, SIGNAL(notify(LocalClientService::Reason)), SLOT(notify(LocalClientService::Reason)));
//  m_client->connectToServer();

//  QStringList args = QApplication::arguments();
//  args.takeFirst();
//  arguments(args);
}


/*!
 * Перезапуск сервера.
 */
void DaemonUi::restart()
{
  if (m_status == Started)
    setStatus(Restarting);
  else
    setStatus(Starting);

  stop();
  QTimer::singleShot(1000, this, SLOT(start()));
}


void DaemonUi::settings()
{
  if (isHidden())
    show();

//  if (!m_settingsDialog) {
//    m_settingsDialog = new DaemonSettingsDialog(this);
//    m_settingsDialog->show();
//  }

//  m_settingsDialog->activateWindow();
}


/*!
 * Запуск сервера.
 */
void DaemonUi::start()
{
//  #ifndef SCHATD_NO_SERVICE
//  if (!m_controller->isInstalled()) {
//    if (!QProcess::startDetached('"' + m_daemonFile + "\" -exec")) {
//      setStatus(Error);
//      return;
//    }
//  }
//  else
//    m_controller->start();
//  #else
//  if (!QProcess::startDetached('"' + m_daemonFile + '"')) {
//    setStatus(Error);
//    return;
//  }
//  #endif

//  if (m_status != Restarting)
//    setStatus(Starting);

//  m_checkTimer.start();
//  m_client->connectToServer();
}


/*!
 * Остановка сервера.
 */
void DaemonUi::stop()
{
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
}


bool DaemonUi::arguments(const QStringList &args)
{
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
  m_actions[StartAction].append(m_menu->addAction(QIcon(":/images/play.png"), tr("Start"), this, SLOT(start())));
  m_actions[StopAction].append(m_menu->addAction(QIcon(":/images/stop.png"), tr("Stop"), this, SLOT(stop())));
  m_actions[RestartAction].append(m_menu->addAction(QIcon(":/images/restart.png"), tr("Restart"), this, SLOT(restart())));
  m_actions[SettingsAction].append(m_menu->addAction(QIcon(":/images/daemonsettings.png"), tr("Settings..."), this, SLOT(settings())));

  m_menu->addSeparator();
  m_menu->addAction(QIcon(LS(":/images/quit.png")), tr("Quit"), QApplication::instance(), SLOT(quit()));
}


/*!
 * Создаёт кнопки
 */
void DaemonUi::createButtons()
{
  m_hideButton = new QPushButton(QIcon(":/images/dialog-ok.png"), "", this);
  connect(m_hideButton, SIGNAL(clicked(bool)), SLOT(hide()));

  m_quitButton = new QPushButton(QIcon(LS(":/images/quit.png")), "", this);
  connect(m_quitButton, SIGNAL(clicked(bool)), QApplication::instance(), SLOT(quit()));

  m_actions[StartAction].append(m_toolBar->addAction(QIcon(":/images/play.png"), tr("Start"), this, SLOT(start())));
  m_actions[StopAction].append(m_toolBar->addAction(QIcon(":/images/stop.png"), tr("Stop"), this, SLOT(stop())));
  m_actions[RestartAction].append(m_toolBar->addAction(QIcon(":/images/restart.png"), tr("Restart"), this, SLOT(restart())));

  m_toolBar->addSeparator();
  m_actions[SettingsAction].append(m_toolBar->addAction(QIcon(":/images/daemonsettings.png"), tr("Settings..."), this, SLOT(settings())));
}


void DaemonUi::createTray()
{
  m_tray = new QSystemTrayIcon(this);
  m_tray->setIcon(QIcon(":/images/schat16-gray.png"));
  m_tray->setToolTip(QString(LS("Simple Chat Daemon %1")).arg(SCHAT_VERSION));
  m_tray->setContextMenu(m_menu);

  connect(m_tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
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
  m_statusGroup->setTitle(tr("Status"));

  m_hideButton->setText(tr("Hide"));
  m_hideButton->setToolTip(tr("Hide the program window"));
  m_quitButton->setToolTip(tr("Quit"));

  setStatus(m_status);
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
    img = ":/images/led/greenled.png";
    icon = ":/images/schat16-green.png";
  }
  else if (color == Yellow) {
    img = ":/images/led/yellowled.png";
    icon = ":/images/schat16-yellow.png";
  }
  else {
    img = ":/images/led/redled.png";
    icon = ":/images/schat16-gray.png";
  }

  m_ledLabel->setPixmap(QPixmap(img));
  m_tray->setIcon(QIcon(icon));
}


void DaemonUi::setStatus(DaemonUi::Status status)
{
  m_status = status;
  switch (status) {
    case Unknown:
      setActionsState(false, false, false, false, false);
      setLedColor(Yellow);
      m_statusLabel->setText("<b style='color:#bf8c00';>&nbsp;" + tr("Initialization...") + "</b>");
      break;

    case Error:
      setActionsState(false, false, false, false);
      setLedColor();
      m_statusLabel->setText("<b style='color:#c00;'>&nbsp;" + tr("Error") + "</b>");
      break;

    case Starting:
      setActionsState(false, false, false);
      setLedColor(Yellow);
      m_statusLabel->setText("<b style='color:#bf8c00';>&nbsp;" + tr("Starting...") + "</b>");
      break;

    case Started:
      setActionsState(false);
      setLedColor(Green);
      m_statusLabel->setText("<b style='color:#090;'>&nbsp;" + tr("Started successfully") + "</b>");
      break;

    case Stopped:
      setActionsState(true, false, true, false);
      setLedColor();
      m_statusLabel->setText("<b style='color:#c00;'>&nbsp;" + tr("Stopped") + "</b>");
      break;

    case Restarting:
      setActionsState(false, false, false, false);
      setLedColor(Yellow);
      m_statusLabel->setText("<b style='color:#bf8c00';>&nbsp;" + tr("Restart...") + "</b>");
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
