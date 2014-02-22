/* $Id: Tr.cpp 2502 2012-04-06 15:37:36Z IMPOMEZIA $
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

#include "Tr.h"

QList<Tr*> Tr::m_hooks;

Tr::Tr()
{
  add(this);
}


Tr::~Tr()
{
  remove(this);
}


QString Tr::value(const QString &key)
{
  QString out;
  foreach (Tr *hook, m_hooks) {
    if (!hook->prefix().isEmpty() && key.startsWith(hook->prefix())) {
      out = hook->valueImpl(key.mid(hook->prefix().size()));
      if (!out.isEmpty())
        return out;
    }
  }

  for (int i = 0; i < m_hooks.size(); ++i) {
    out = m_hooks.at(i)->valueImpl(key);
    if (!out.isEmpty())
      return out;
  }

  return key;
}


QString Tr::valueImpl(const QString & /*key*/) const
{
  return QString();
}
