/* $Id: HistorySettings.h 3603 2013-03-27 15:19:21Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2013 IMPOMEZIA <schat@impomezia.com>
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

#ifndef HISTORYSETTINGS_H_
#define HISTORYSETTINGS_H_

#include <QWidget>

class QCheckBox;

class HistorySettings : public QWidget
{
  Q_OBJECT

public:
  HistorySettings(QWidget *parent = 0);

protected:
  void changeEvent(QEvent *event);

private slots:
  void autoLoad(bool checked);

private:
  void retranslateUi();

  QCheckBox *m_autoLoad;
};

#endif /* HISTORYSETTINGS_H_ */
