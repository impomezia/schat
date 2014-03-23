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
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>

#include "BackdropWidget.h"
#include "ChatCore.h"
#include "ChatSettings.h"
#include "sglobal.h"

BackdropWidget::BackdropWidget(QWidget *parent)
  : QFrame(parent)
  , m_autoClose(true)
{
  m_layout = new QGridLayout(this);
  m_layout->setColumnStretch(0, 1);
  m_layout->setColumnStretch(2, 1);
  m_layout->setRowStretch(1, 1);
  m_layout->setMargin(22);
}


void BackdropWidget::setWidget(QWidget *widget)
{
  Q_ASSERT(widget);

  if (m_widget) {
    m_layout->removeWidget(m_widget);
    m_widget->close();
  }

  widget->setParent(this);
  widget->setAutoFillBackground(true);
  widget->installEventFilter(this);
  widget->setFocus();

  m_widget = widget;
  m_layout->addWidget(widget, 0, 1);
}


bool BackdropWidget::eventFilter(QObject *watched, QEvent *event)
{
  if (m_widget && m_widget == watched && event->type() == QEvent::Close) {
    m_layout->removeWidget(m_widget);
    close();

    emit closed();
  }

  return QFrame::eventFilter(watched, event);
}


void BackdropWidget::contextMenuEvent(QContextMenuEvent *event)
{
  if (childAt(event->pos())) {
    QFrame::contextMenuEvent(event);
    return;
  }

  QMenu menu(this);
  QAction *blur = menu.addAction(tr("Blur Effect"));
  blur->setCheckable(true);
  blur->setChecked(ChatCore::settings()->value(SETTINGS_BLUR_EFFECT).toBool());

  QAction *action = menu.exec(event->globalPos());
  if (action == blur)
    ChatCore::settings()->setValue(SETTINGS_BLUR_EFFECT, blur->isChecked());
}


void BackdropWidget::mouseReleaseEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton && m_autoClose && m_widget && !childAt(event->pos())) {
    m_widget->close();
  }

  QFrame::mouseReleaseEvent(event);
}


void BackdropWidget::paintEvent(QPaintEvent *event)
{
  Q_UNUSED(event)

  QPainter painter(this);

  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(Qt::NoPen);
  painter.setBrush(QColor(0, 0, 0, 127));
  painter.drawRect(0, 0, width(),  height());
}
