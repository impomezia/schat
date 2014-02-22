/* $Id: TokenFilter.h 3650 2013-04-21 00:21:16Z IMPOMEZIA $
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

#ifndef TOKENFILTER_H_
#define TOKENFILTER_H_

#include <QMap>
#include <QStringList>
#include <QVariant>
#include <QSharedPointer>

#include "schat.h"
#include "text/HtmlToken.h"

/*!
 * Базовый абстрактный класс фильтрации текста.
 */
class SCHAT_CORE_EXPORT AbstractFilter
{
public:
  inline AbstractFilter(int weight = 0)
  : m_weight(weight)
  {}

  virtual ~AbstractFilter() {}
  inline int weight() const { return m_weight; }
  virtual bool filter(QList<HtmlToken> &tokens, int options = 0) const = 0;

private:
  int m_weight;
};


typedef QSharedPointer<AbstractFilter> FilterPtr;


class SCHAT_CORE_EXPORT TokenFilter
{
  TokenFilter() {}

public:
  static QString filter(const QString &type, const QString &text, int options = 0);
  static void add(const QString &type, AbstractFilter *filter);
  static void clear();

private:
  static QMap<QString, QMap<int, FilterPtr> > m_filters; ///< Доступные фильтры.
};


#endif /* TOKENFILTER_H_ */
