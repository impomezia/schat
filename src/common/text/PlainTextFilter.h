/* $Id: PlainTextFilter.h 1749 2011-09-22 15:41:17Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2011 IMPOMEZIA <schat@impomezia.com>
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

#ifndef PLAINTEXTFILTER_H_
#define PLAINTEXTFILTER_H_

#include <QString>

#include "schat.h"

/*!
 * Фильтр преобразует HTML текст в обычный.
 */
class SCHAT_EXPORT PlainTextFilter
{
private:
  PlainTextFilter() {}

public:
  static QString filter(const QString &text);
  static void removeTag(QString &text, const QString &tag);
};

#endif /* PLAINTEXTFILTER_H_ */
