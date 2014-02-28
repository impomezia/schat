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

#include <QGridLayout>

#include "BackdropWidget.h"
#include "sglobal.h"

BackdropWidget::BackdropWidget(QWidget *parent)
  : QFrame(parent)
  , m_widget(0)
{
  setObjectName(LS("BackdropWidget"));
  setStyleSheet(LS("BackdropWidget{background:rgba(0,0,0,.5)}"));

  m_layout = new QGridLayout(this);
  m_layout->setColumnStretch(0, 1);
  m_layout->setColumnStretch(2, 1);
  m_layout->setRowStretch(0, 1);
  m_layout->setRowStretch(2, 3);
}


void BackdropWidget::setWidget(QWidget *widget)
{
  Q_ASSERT(widget);

  widget->setParent(this);
  widget->setAutoFillBackground(true);

  m_layout->addWidget(widget, 1, 1);
}
