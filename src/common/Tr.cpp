/* Simple Chat
 * Copyright (c) 2008-2014 Alexander Sedov <imp@schat.me>
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


QString Tr::value(const QString &key, int number)
{
  QString out;
  foreach (Tr *hook, m_hooks) {
    if (!hook->prefix().isEmpty() && key.startsWith(hook->prefix())) {
      out = hook->valueImpl(key.mid(hook->prefix().size()), number);
      if (!out.isEmpty())
        return out;
    }
  }

  for (int i = 0; i < m_hooks.size(); ++i) {
    out = m_hooks.at(i)->valueImpl(key, number);
    if (!out.isEmpty())
      return out;
  }

  return key;
}


QString Tr::valueImpl(const QString & /*key*/) const
{
  return QString();
}


QString Tr::valueImpl(const QString & /*key*/, int /*number*/) const
{
  return QString();
}
