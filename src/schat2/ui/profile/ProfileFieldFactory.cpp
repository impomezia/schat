/* $Id: ProfileFieldFactory.cpp 3454 2013-02-01 21:21:54Z IMPOMEZIA $
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

#include "ui/profile/ProfileFieldFactory.h"
#include "ui/profile/TextField.h"

QMap<QString, QSharedPointer<ProfileFieldFactory> > ProfileFieldFactory::m_hooks;


ProfileField* ProfileFieldFactory::create(const QString &field, QWidget *parent)
{
  if (m_hooks.contains(field))
    return m_hooks.value(field)->createImpl(field, parent);

  return new TextField(field, parent);
}


void ProfileFieldFactory::add(ProfileFieldFactory *hook)
{
  if (hook->m_name.isEmpty()) {
    delete hook;
    return;
  }

  m_hooks[hook->m_name] = QSharedPointer<ProfileFieldFactory>(hook);
}


void ProfileFieldFactory::clear()
{
  m_hooks.clear();
}


ProfileField* ProfileFieldFactory::createImpl(const QString & /*field*/, QWidget * /*parent*/)
{
  return 0;
}
