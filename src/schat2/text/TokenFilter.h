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

#ifndef TOKENFILTER_H_
#define TOKENFILTER_H_

#include <QMap>
#include <QStringList>
#include <QVariant>
#include <QSharedPointer>

#include "interfaces/ITokenFilter.h"
#include "schat.h"


typedef QSharedPointer<ITokenFilter> FilterPtr;


class SCHAT_CORE_EXPORT TokenFilter
{
  TokenFilter() {}

public:
  static QString filter(const QString &type, const QString &text, int options, const ChatId &id = ChatId());
  static void add(const QString &type, ITokenFilter *filter);
  static void clear();

private:
  static QMap<QString, QMap<int, FilterPtr> > m_filters; ///< Доступные фильтры.
};


#endif /* TOKENFILTER_H_ */
