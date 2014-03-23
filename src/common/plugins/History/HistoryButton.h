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

#ifndef HISTORYBUTTON_H_
#define HISTORYBUTTON_H_

#include <QApplication>
#include <QDate>
#include <QMap>
#include <QToolButton>

#include "ui/ToolBarAction.h"

class HistoryButton : public QToolButton
{
  Q_OBJECT

public:
  HistoryButton(QWidget *parent = 0);

protected:
  void changeEvent(QEvent *event);

private slots:
  void activated(const QDate &date);
  void menuAboutToHide();
  void menuAboutToShow();

private:
  void retranslateUi();

  QMap<QByteArray, QDate> m_dates; ///< Сохранённы даты для вкладок.
  QMenu *m_menu;                   ///< Меню для показа календаря.
};


class HistoryAction : public ToolBarActionCreator
{
  Q_DECLARE_TR_FUNCTIONS(HistoryAction)

public:
  HistoryAction();
  bool isVisible(const QString &type, const QByteArray &id);
  QIcon icon() const;
  QString title() const;
  QWidget* createWidget(QWidget *parent) const;
};

#endif /* HISTORYBUTTON_H_ */
