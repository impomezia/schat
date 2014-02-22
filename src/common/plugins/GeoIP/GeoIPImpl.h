/* $Id: GeoIPImpl.h 2497 2012-04-05 19:57:53Z IMPOMEZIA $
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

#ifndef GEOIPIMPL_H_
#define GEOIPIMPL_H_

#include "plugins/GeoHook.h"

class QGeoIP;

class GeoIPImpl : public GeoHook
{
public:
  GeoIPImpl();
  ~GeoIPImpl();

protected:
  virtual void geo(const QString &ip, QVariantMap &out);

private:
  QGeoIP *m_geo;
};

#endif /* GEOIPIMPL_H_ */
