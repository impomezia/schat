/* $Id: SendWidget.h 2769 2012-06-12 23:18:08Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2012 IMPOMEZIA <schat@impomezia.com>
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

#ifndef SENDWIDGET_H_
#define SENDWIDGET_H_

#include <QWidget>
#include <QMap>

#include "schat.h"
#include "ui/ToolBarAction.h"

class AbstractTab;
class InputWidget;
class Notify;
class QMenu;
class QToolBar;

class SCHAT_CORE_EXPORT SendWidget : public QWidget
{
  Q_OBJECT

public:
  SendWidget(QWidget *parent = 0);
  inline InputWidget *input()                           { return m_input; }
  inline static SendWidget *i()                         { return m_self; }
  static void add(ToolBarActionCreator *creator, bool refill = true);
  void setInputFocus();

signals:
  void send(const QString &text);

public slots:
  void insertHtml(const QString &text);

protected:
  bool event(QEvent *event);
  void contextMenuEvent(QContextMenuEvent *event);

private slots:
  void notify(const Notify &notify);
  void pageChanged(AbstractTab *tab);
  void settingsChanged(const QString &key, const QVariant &value);

private:
  QAction* before(int weight) const;
  QStringList available() const;
  void add(int weight, ToolBarActionCreator *creator, bool refill = true);
  void add(ToolBarAction action);
  void refill();
  void remove(ToolBarAction action);
  void updateStyleSheet();

  InputWidget *m_input;                 ///< Виджет ввода текста.
  QMap<int, ToolBarAction> m_actions;   ///< Отсортированная таблица всех доступных действий.
  QMap<QString, ToolBarAction> m_names; ///< Таблица имён действий.
  QStringList m_layout;                 ///< Список действий размещённых на тулбаре.
  QToolBar *m_toolBar;                  ///< Панель инструментов.
  static SendWidget *m_self;            ///< Указатель на себя.
};

#endif /* SENDWIDGET_H_ */
