/* $Id: CountryField.h 2535 2012-04-12 22:51:35Z IMPOMEZIA $
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

#ifndef COUNTRYFIELD_H_
#define COUNTRYFIELD_H_

#include "ui/profile/ProfileField.h"
#include "ui/profile/ProfileFieldFactory.h"

class QComboBox;
class QHBoxLayout;
class Spinner;

class CountryField : public ProfileField
{
  Q_OBJECT

public:
  CountryField(QWidget *parent = 0);
  void retranslateUi();

protected:
  void setData(const QVariant &value);

private slots:
  void indexChanged(int index);

private:
  QIcon icon(const QString &code, const QPixmap &layout);
  QPoint pos(const QString &code);
  void load();

  QComboBox *m_box;
  QHBoxLayout *m_mainLay; ///< Компоновщик.
  Spinner *m_spinner;     ///< Отображает состояние подключения.
};


class CountryFieldCreator : public ProfileFieldFactory
{
public:
  CountryFieldCreator()
  : ProfileFieldFactory(QLatin1String("country"))
  {}

protected:
  ProfileField* createImpl(const QString & /*field*/, QWidget *parent = 0) { return new CountryField(parent); }
};

#endif /* СOUNTRYFIELD_H_ */
