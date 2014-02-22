/* $Id: NickEdit.h 3104 2012-09-20 17:33:12Z IMPOMEZIA $
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

#ifndef NICKEDIT_H_
#define NICKEDIT_H_

#include "ui/SLineEdit.h"

class ChannelNotice;
class QLabel;
class Spinner;

/*!
 * Виджет редактирования ника пользователя.
 */
class NickEdit : public SLineEdit
{
  Q_OBJECT

public:
  NickEdit(QWidget *parent = 0);

private slots:
  void editingFinished();
  void notice(const ChannelNotice &notice);
  void settingsChanged(const QString &key, const QVariant &value);
  void textChanged();

private:
  void makeRed(bool red = true);
  void spinner(bool start = true);

  QLabel *m_error;    ///< Виджет для отображения ошибки.
  Spinner *m_spinner; ///< Отображает состояние подключения.
};

#endif /* NICKEDIT_H_ */
