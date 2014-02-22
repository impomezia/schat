/* $Id: Groups.h 3209 2012-10-21 21:44:28Z IMPOMEZIA $
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

#ifndef GROUPS_H_
#define GROUPS_H_

#include <QStringList>

class Groups
{
public:
  inline Groups() {}
  inline bool contains(const QString &name)  { return m_groups.contains(name); }
  inline const QStringList& all() const      { return m_groups; }
  inline Groups &add(const QString &name)    { if (!m_groups.contains(name) && !name.contains(QLatin1String(","))) m_groups.append(name); return *this; }
  inline QString toString() const            { return m_groups.join(QLatin1String(",")); }
  inline void remove(const QString &name)    { m_groups.removeAll(name); }
  inline void set(const QString &groups)     { m_groups = groups.split(QLatin1String(","), QString::SkipEmptyParts); }
  inline void set(const QStringList &groups) { m_groups = groups; }

  inline Groups &operator+=(const QString &name)        { return add(name); }
  inline Groups &operator=(const QString &groups)       { set(groups); return *this; }
  inline Groups &operator=(const QStringList &groups)   { set(groups); return *this; }

private:
  QStringList m_groups; ///< Список групп.
};

#endif /* GROUPS_H_ */
