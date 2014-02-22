/* $Id: UrlQuery.h 3490 2013-02-11 02:15:27Z IMPOMEZIA $
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

#ifndef URLQUERY_H_
#define URLQUERY_H_

#if QT_VERSION >= 0x050000
# include <QUrlQuery>
#else
# include <QUrl>
  /*!
   * Класс обвёртка для QUrl обеспечивающий совместимость с Qt 5.
   */
  class QUrlQuery
  {
  public:
    inline explicit QUrlQuery(const QUrl &url)
    : m_url(url)
    {}

    inline bool hasQueryItem(const QString &key) const      { return m_url.hasQueryItem(key); }
    inline QString queryItemValue(const QString &key) const { return m_url.queryItemValue(key); }

  private:
    QUrl m_url; ///< Копия объекта QUrl.
  };
#endif

#endif /* URLQUERY_H_ */
