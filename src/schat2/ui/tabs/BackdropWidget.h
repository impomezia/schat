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

class QGridLayout;

class BackdropWidget : public QFrame
{
  Q_OBJECT

public:
  BackdropWidget(QWidget *parent = 0);
  void setWidget(QWidget *widget);
  inline QWidget *widget() const { return m_widget; }

private:
  QGridLayout *m_layout;
  QWidget *m_widget;
};

#endif // BACKDROPWIDGET_H_
