/* $Id: SLineEdit.h 3104 2012-09-20 17:33:12Z IMPOMEZIA $
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

#ifndef SLINEEDIT_H_
#define SLINEEDIT_H_

#include "arora/lineedit.h"

class SLineEdit : public LineEdit
{
  Q_OBJECT

public:
  SLineEdit(QWidget *parent = 0);
  SLineEdit(const QString &contents, QWidget *parent = 0);

protected:
  void contextMenuEvent(QContextMenuEvent *event);

private slots:
  inline void deleteSelected() { if (!hasSelectedText()) return; del(); }
};

#endif /* SLINEEDIT_H_ */
