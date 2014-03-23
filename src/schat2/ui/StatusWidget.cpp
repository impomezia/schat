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

#include <QMouseEvent>

#include "ChatCore.h"
#include "ui/StatusMenu.h"
#include "ui/StatusWidget.h"

StatusWidget::StatusWidget(QWidget *parent)
  : QLabel(parent)
{
  hide();

  connect(StatusMenu::i(), SIGNAL(updated()), SLOT(reload()));

  if (!ChatCore::isReady())
    connect(ChatCore::i(), SIGNAL(ready()), SLOT(onReady()));
  else
    onReady();
}


/*!
 * Обработка нажатий кнопок мыши для показа меню.
 */
void StatusWidget::mouseReleaseEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton)
    StatusMenu::i()->exec(event->globalPos());
  else
    QLabel::mouseReleaseEvent(event);
}


void StatusWidget::onReady()
{
  reload();
  show();
}


void StatusWidget::reload()
{
  setPixmap(StatusMenu::i()->icon().pixmap(16));
  setToolTip(StatusMenu::i()->title());
}
