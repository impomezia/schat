/*   Copyright (C) 2013-2014 Alexander Sedov <imp@schat.me>
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

#include "interfaces/IProvider.h"
#include "Providers.h"
#include "sglobal.h"

Providers::Providers(QObject *parent)
  : QObject(parent)
{
  if (!m_list.isEmpty())
    m_currentId = m_list.first();
}


Providers::~Providers()
{
  qDeleteAll(m_map);
  m_map.clear();
}


IProvider *Providers::current() const
{
  IProvider *provider = m_map.value(currentId());
  Q_ASSERT(provider);

  return provider;
}


IProvider *Providers::get(const QString &id) const
{
  IProvider *provider = m_map.value(id);
  Q_ASSERT(provider);

  return provider;
}


void Providers::add(IProvider *provider)
{
  Q_ASSERT(provider);
  if (!provider)
    return;

  const QString id = provider->id();

  if (m_map.contains(id)) {
    delete provider;
    return;
  }

  if (m_list.isEmpty())
    m_currentId = id;

  m_map.insert(id, provider);
  m_list.append(id);
}


void Providers::create(QMap<QString, Uploader*> &map, QObject *parent)
{
  QMapIterator<QString, IProvider*> i(m_map);
  while (i.hasNext()) {
    i.next();

    map.insert(i.key(), i.value()->uploader(parent));
  }
}


void Providers::setCurrentId(const QString &id)
{
  if (m_map.contains(id))
    m_currentId = id;
}
