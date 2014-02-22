/* $Id: SLineEdit.cpp 3104 2012-09-20 17:33:12Z IMPOMEZIA $
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

#include <QMenu>
#include <QContextMenuEvent>
#include <QClipboard>
#include <QApplication>

#include "ui/SLineEdit.h"
#include "ui/ChatIcons.h"

SLineEdit::SLineEdit(QWidget *parent)
  : LineEdit(parent)
{
}


SLineEdit::SLineEdit(const QString &contents, QWidget *parent)
  : LineEdit(contents, parent)
{
}


void SLineEdit::contextMenuEvent(QContextMenuEvent *event)
{
  bool selection = hasSelectedText();
  bool ro = isReadOnly();
  QMenu menu(this);

  if (selection && !ro)
    menu.addAction(SCHAT_ICON(EditCut), tr("Cut"), this, SLOT(cut()));

  if (selection)
    menu.addAction(SCHAT_ICON(EditCopy), tr("Copy"), this, SLOT(cut()));

  if (!ro && !QApplication::clipboard()->text().isEmpty())
    menu.addAction(SCHAT_ICON(EditPaste), tr("Paste"), this, SLOT(paste()));

  if (selection && !ro)
    menu.addAction(SCHAT_ICON(Remove), tr("Delete"), this, SLOT(deleteSelected()));

  if (!menu.isEmpty())
    menu.addSeparator();

  if (!text().isEmpty() && text() != selectedText())
    menu.addAction(SCHAT_ICON(EditSelectAll), tr("Select All"), this, SLOT(selectAll()));

  if (!menu.isEmpty())
    menu.exec(event->globalPos());
}
