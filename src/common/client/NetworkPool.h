/* $Id: NetworkPool.h 2550 2012-04-16 03:31:26Z IMPOMEZIA $
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

#ifndef NETWORKPOOL_H_
#define NETWORKPOOL_H_

#include <QUrl>

class NetworkPool
{
public:
  NetworkPool();
  inline int count() const { return m_urls.size(); }
  QUrl current() const;
  QUrl next() const;
  QUrl random() const;
  void reset();
  void setUrls(const QList<QUrl> &urls);

private:
  int m_last;               ///< Индекс последнего сервера к которому было осуществлено успешное подключение.
  mutable int m_current;    ///< Индекс текущего Url.
  QList<QUrl> m_urls;       ///< Пул адресов.
};

#endif /* NETWORKPOOL_H_ */
