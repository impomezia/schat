/* $Id: GeoHook.h 2496 2012-04-05 19:52:35Z IMPOMEZIA $
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

#ifndef GEOHOOK_H_
#define GEOHOOK_H_

#include <QList>
#include <QVariantMap>

#include "schat.h"

class SCHAT_EXPORT GeoHook
{
public:
  GeoHook();
  virtual ~GeoHook();

  inline static void add(GeoHook *hook)    { m_hooks.append(hook); }
  inline static void remove(GeoHook *hook) { m_hooks.removeAll(hook); }

  static QVariantMap geo(const QString &ip);

protected:
  virtual void geo(const QString &ip, QVariantMap &out);

private:
  static QList<GeoHook*> m_hooks; ///< Хуки.
};

#endif /* GEOHOOK_H_ */
