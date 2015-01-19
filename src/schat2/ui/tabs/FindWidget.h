/* Simple Chat
 * Copyright (c) 2008-2015 Alexander Sedov <imp@schat.me>
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

#ifndef FINDWIDGET_H_
#define FINDWIDGET_H_

#include <QFrame>

#include "schat.h"

class QToolBar;
class SLineEdit;

class FindWidget : public QFrame
{
  Q_OBJECT

public:
  FindWidget(QWidget *parent = 0);
  QString text() const;
  void setPalette(bool found);

signals:
  void find(const QString &text, bool forward);
  void hidden();

public slots:
  void find();
  void setFocus();

protected:
  void changeEvent(QEvent *event) Q_DECL_OVERRIDE;

private slots:
  void actionTriggered(QAction *action);

private:
  bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;
  void retranslateUi();

  QAction *m_close;      ///< Кнопка закрытия виджета.
  QAction *m_next;       ///< Кнопка поиска следующей позиции найденого текста.
  QAction *m_previous;   ///< Кнопка поиска предыдущей позиции найденого текста.
  QToolBar *m_toolBar;   ///< Основной тулбар для размещения виджетов.
  SLineEdit *m_editFind; ///< Виджет для ввода текста.
};

#endif /* FINDWIDGET_H_ */
