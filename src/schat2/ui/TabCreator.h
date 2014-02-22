/* $Id: TabCreator.h 3501 2013-02-14 07:16:38Z IMPOMEZIA $
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

#ifndef TABCREATOR_H_
#define TABCREATOR_H_

#include <QByteArray>

class AbstractTab;
class QVariant;
class TabWidget;

class TabCreator
{
public:
  TabCreator() {}
  virtual ~TabCreator() {}
  virtual AbstractTab *create(const QVariant &data, TabWidget *parent) const = 0;
  virtual QByteArray name() const = 0;
  virtual void reload(AbstractTab *tab, const QVariant &data) const = 0;
};

#endif /* TABCREATOR_H_ */
