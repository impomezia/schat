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

#ifndef TRAYICON_H_
#define TRAYICON_H_

#include <QSystemTrayIcon>

class QAction;
class QBasicTimer;

class TrayIcon : public QSystemTrayIcon
{
  Q_OBJECT

public:
  TrayIcon(QObject *parent = 0);
  ~TrayIcon();
  inline static TrayIcon *i() { return m_self; }
  void retranslateUi();

protected:
  void timerEvent(QTimerEvent *event);

private slots:
  void alert(bool start);
  void changed(const QString &key, const QVariant &value);
  void iconActivated(QSystemTrayIcon::ActivationReason reason);
  void reload();
  void triggered(QAction *action);

private:
  void startAlert();

  bool m_staticAlerts;       ///< \b true если необходимо использовать статические (без мигания) уведомления.
  int m_alertIcon;           ///< Номер текущей иконки, при отображении уведомлений.
  QAction *m_about;          ///< О Simple Chat.
  QAction *m_quit;           ///< Выход.
  QAction *m_settings;       ///< Настройки.
  QAction *m_show;           ///< Показать.
  QBasicTimer *m_timer;      ///< Таймер анимации.
  QIcon m_icon;              ///< Иконка.
  QMenu *m_menu;             ///< Меню.
  static TrayIcon *m_self;   ///< Указатель на себя.
};

#endif /* TRAYICON_H_ */
