/* $Id: AlertType.cpp 3113 2012-09-22 00:19:56Z IMPOMEZIA $
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

#include "alerts/AlertType.h"

AlertType::AlertType(const QString &type, int weight)
  : m_weight(weight)
  , m_type(type)
{
}


QVariant AlertType::value(const QString &key, const QVariant &defaultValue) const
{
  return options.value(key, m_defaults.value(key, defaultValue));
}
