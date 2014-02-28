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

#ifndef BACKDROPWIDGET_H_
#define BACKDROPWIDGET_H_

#include <QFrame>

#include "schat.h"

class QGridLayout;

class BackdropWidget : public QFrame
{
  Q_OBJECT

public:
  BackdropWidget(QWidget *parent = 0);
  inline QWidget *widget() const           { return m_widget; }
  inline void setAutoClose(bool autoClose) { m_autoClose = autoClose; }
  void setWidget(QWidget *widget);

signals:
  void closed();

protected:
  bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;
  void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;
  void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
  void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
  bool m_autoClose;
  QGridLayout *m_layout;
  QWidget *m_widget;
};

#endif // BACKDROPWIDGET_H_
