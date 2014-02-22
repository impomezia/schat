/* $Id: NetworkPool.cpp 2550 2012-04-16 03:31:26Z IMPOMEZIA $
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

#include "client/AbstractClient.h"
#include "client/NetworkPool.h"

NetworkPool::NetworkPool()
  : m_last(-1)
  , m_current(-1)
{
}


QUrl NetworkPool::current() const
{
  if (m_current == -1 || m_current >= m_urls.size())
    return m_urls.at(0);

  return m_urls.at(m_current);
}


/*!
 * Получение следующего адреса.
 */
QUrl NetworkPool::next() const
{
  if (m_urls.isEmpty())
    return QUrl();

  if (m_urls.size() == 1)
    return m_urls.at(0);

  m_current++;
  if (m_current == m_urls.size())
    m_current = 0;

  return m_urls.at(m_current);
}


/*!
 * Получение адреса случайного сервера.
 */
QUrl NetworkPool::random() const
{
  if (m_urls.isEmpty())
    return QUrl();

  if (m_urls.size() == 1)
    return m_urls.at(0);

  int rand = qrand() % m_urls.size();
  if (m_current == rand) {
    rand++;
    if (rand == m_urls.size())
      rand = 0;
  }

  m_current = rand;
  return m_urls.at(m_current);
}


void NetworkPool::reset()
{
  m_urls.clear();
  m_current = -1;
  m_last = -1;
}


void NetworkPool::setUrls(const QList<QUrl> &urls)
{
  reset();
  m_urls = urls;
}
