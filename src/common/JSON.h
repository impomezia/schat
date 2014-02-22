/* $Id: JSON.h 2637 2012-05-11 19:01:10Z IMPOMEZIA $
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

#ifndef JSON_H_
#define JSON_H_

#include <QVariant>

#include "schat.h"

class SCHAT_EXPORT JSON
{
  JSON() {}

public:
  static QByteArray generate(const QVariant &data, bool formatted = false);
  static QString quote(const QString &text);
  static QVariant parse(const QByteArray &data);

private:
  static void gen(QByteArray &res, const QVariant &val);
};

#endif /* JSON_H_ */
