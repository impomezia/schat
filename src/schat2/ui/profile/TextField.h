/* $Id: TextField.h 2533 2012-04-12 20:31:52Z IMPOMEZIA $
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

#ifndef TEXTFIELD_H_
#define TEXTFIELD_H_

#include "ui/profile/ProfileField.h"

class LineEdit;
class Spinner;

class SCHAT_CORE_EXPORT TextField : public ProfileField
{
  Q_OBJECT

public:
  TextField(const QString &field, QWidget *parent = 0);

private slots:
  void editingFinished();

protected:
  void setData(const QVariant &value);

  LineEdit *m_edit;   ///< Поле редактирования.
  Spinner *m_spinner; ///< Отображает состояние подключения.
};

#endif /* TEXTFIELD_H_ */
