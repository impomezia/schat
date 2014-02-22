/* $Id: ProfileFieldFactory.h 3454 2013-02-01 21:21:54Z IMPOMEZIA $
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

#ifndef PROFILEFIELDFACTORY_H_
#define PROFILEFIELDFACTORY_H_

#include <QMap>
#include <QSharedPointer>

#include "schat.h"

class ProfileField;
class QWidget;

class SCHAT_CORE_EXPORT ProfileFieldFactory
{
public:
  ProfileFieldFactory(const QString &name)
    : m_name(name)
  {}
  virtual ~ProfileFieldFactory() {}

  static ProfileField* create(const QString &field, QWidget *parent = 0);
  static void add(ProfileFieldFactory *hook);
  static void clear();

protected:
  virtual ProfileField* createImpl(const QString &field, QWidget *parent = 0);

  QString m_name;

private:
  static QMap<QString, QSharedPointer<ProfileFieldFactory> > m_hooks;
};

#endif /* PROFILEFIELDFACTORY_H_ */
