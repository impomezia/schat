/* $Id: NetworkWidget.h 3494 2013-02-11 06:17:19Z IMPOMEZIA $
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

#ifndef NETWORKWIDGET_H_
#define NETWORKWIDGET_H_

#include <QWidget>

class NetworkComboBox;
class NetworkManager;
class QComboBox;
class QGridLayout;
class QLabel;
class QMenu;
class QToolBar;
class QToolButton;
class SLineEdit;

class NetworkWidget : public QWidget
{
  Q_OBJECT

public:
  /// Дополнительные возможности виджета.
  enum WidgetOptions {
    BasicLayout = 0,
    BigButton   = 1,
    ExtraLayout = 2
  };

  NetworkWidget(QWidget *parent, int options = ExtraLayout);
  QAction *connectAction();

public slots:
  void open();

protected:
  bool eventFilter(QObject *watched, QEvent *event);
  void changeEvent(QEvent *event);

private slots:
  void applyUrl();
  void indexChanged(int index);
  void menuTriggered(QAction *action);
  void reload();
  void showMenu();

private:
  void createActionsButton();
  void retranslateUi();
  void setTitle(const QString &title);
  void signOut();

  int m_options;                 ///< Дополнительные возможности виджета.
  NetworkComboBox *m_combo;      ///< Комбобокс выбора серверов.
  NetworkManager *m_manager;     ///< Указатель на менеджер сетевых подключений.
  QAction *m_add;                ///< Действие для добавления сервера.
  QAction *m_computers;          ///< Действие для просмотра компьютеров.
  QAction *m_connect;            ///< Действие для подключения или отключения от сервера.
  QAction *m_edit;               ///< Действие для редактирования текущего подключения.
  QAction *m_remove;             ///< Действие для удаления сервера.
  QAction *m_signOut;            ///< Действие для выхода из текущей учётной записи.
  QGridLayout *m_layout;         ///< Основной компоновщик.
  QLabel *m_urlLabel;            ///< Текстоая метка для поля редактирования адреса сервера.
  QMenu *m_menu;                 ///< Меню дополнительных действий.
  QToolBar *m_toolBar;           ///< Тулбар для размещения основных действий.
  QToolButton *m_actions;        ///< Кнопка дополнительных действий.
  SLineEdit *m_urlEdit;          ///< Поле редактирования адреса сервера.
};

#endif /* NETWORKWIDGET_H_ */
