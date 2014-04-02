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

#ifndef DAEMONUI_H_
#define DAEMONUI_H_

#include <QDialog>
#include <QMap>
#include <QPointer>
#include <QSystemTrayIcon>
#include <QTimer>

class DaemonSettings;
class QAction;
class QGroupBox;
class QLabel;
class QMenu;
class QProcess;
class QToolBar;
class Settings;
class Translation;

/*!
 * \brief Диалог управления сервером
 */
class DaemonUi : public QDialog
{
  Q_OBJECT

public:
  enum Actions {
    StartAction,
    StopAction,
    RestartAction,
    SettingsAction
  };

  DaemonUi(QWidget *parent = 0);
  ~DaemonUi();

public slots:
  void handleMessage(const QString& message);

protected:
  void changeEvent(QEvent *event);

private slots:
  void checkStart();
  void exit();
  void iconActivated(QSystemTrayIcon::ActivationReason reason);
  void init();
  void restart();
  void settings();
  void start();
  void stop();

private:
  enum Status {
    Unknown,
    Error,
    Starting,
    Started,
    Stopped,
    Restarting
  };

  enum LedColor {
    Red,
    Yellow,
    Green
  };

  bool arguments(const QStringList &args);
  void createActions();
  void createButtons();
  void createTray();
  void loadTranslation();
  void retranslateUi();
  void setActionsState(bool start = true, bool stop = true, bool restart = true, bool quit = true, bool settings = true);
  void setEnabled(Actions action, bool enabled);
  void setLedColor(LedColor color = Red);
  void setStatus(Status status);
  void showUi();

  QGroupBox *m_controlGroup;
  QGroupBox *m_statusGroup;
  QLabel *m_aboutLabel;
  QLabel *m_ledLabel;
  QLabel *m_statusLabel;
  QMenu *m_menu;
  QPushButton *m_hideButton;
  QPushButton *m_quitButton;
  QString m_appDir;
  QSystemTrayIcon *m_tray;
  QTimer m_checkTimer;
  QToolBar *m_toolBar;
  Settings *m_settings;
  Status m_status;
  Translation *m_translation;
  QMap<Actions, QList<QAction*> > m_actions;
};

#endif /*DAEMONUI_H_*/
