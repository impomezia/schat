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

#ifndef STATUSBAR_H_
#define STATUSBAR_H_

#include <QObject>
#include <QStatusBar>
#include <QUrl>

#include "schat.h"

class BgOperationWidget;
class LoginIcon;
class NetworkWidget;
class Notify;
class QLabel;
class QProgressBar;
class QWidgetAction;
class Spinner;
class StatusWidget;

class SCHAT_CORE_EXPORT StatusBar : public QStatusBar
{
  Q_OBJECT

public:
  StatusBar(QWidget *parent = 0);

protected:
  bool event(QEvent *event) Q_DECL_OVERRIDE;
  void changeEvent(QEvent *event) Q_DECL_OVERRIDE;
  void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private slots:
  void onClientStateChanged(int state);
  void onMenuTriggered(QAction *action);
  void onNotify(const Notify &notify);
  void onReady();

private:
  void menu(const QPoint &point);
  void retranslateUi();
  void setError();
  void setServerName();
  void updateStyleSheet();

  BgOperationWidget *m_bg;        ///< Видежет для отображения фоновых операций.
  LoginIcon *m_login;             ///< Иконка авторизации по имени и паролю.
  NetworkWidget *m_url;           ///< Поле ввода адреса сервера.
  QAction *m_connect;             ///< Главное действие связанное с сетевым подключением.
  QLabel *m_icon;                 ///< Главная иконка.
  QLabel *m_label;                ///< Информация о состоянии.
  QLabel *m_secure;               ///< Иконка безопасного соединения.
  QWidgetAction *m_urlAction;     ///< Действие для добавления в меню поля ввода адреса сервера.
  Spinner *m_spinner;             ///< Отображает состояние подключения.
  StatusWidget *m_status;         ///< Виджет выбора статуса.
};

#endif /* STATUSBAR_H_ */
