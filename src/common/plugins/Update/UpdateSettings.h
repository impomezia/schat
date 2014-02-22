/* $Id: UpdateSettings.h 3763 2013-08-09 15:05:39Z IMPOMEZIA $
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

#ifndef UPDATESETTINGS_H_
#define UPDATESETTINGS_H_

#include <QWidget>

class QCheckBox;

class UpdateSettings : public QWidget
{
  Q_OBJECT

public:
  UpdateSettings(QWidget *parent = 0);

protected:
  void changeEvent(QEvent *event);

private slots:
  void save();

private:
  void retranslateUi();

  QCheckBox *m_autoDownload;
  QCheckBox *m_beta;
};

#endif /* UPDATESETTINGS_H_ */
