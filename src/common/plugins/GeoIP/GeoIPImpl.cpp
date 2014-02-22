/* $Id: GeoIPImpl.cpp 3544 2013-03-02 00:57:33Z IMPOMEZIA $
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

#include "GeoIPImpl.h"
#include "Path.h"
#include "QGeoIP.h"
#include "sglobal.h"
#include "Storage.h"

GeoIPImpl::GeoIPImpl()
  : GeoHook()
{
  m_geo = new QGeoIP();

  const QString path = Path::data(Path::SystemScope) + LS("/res/");
  if (!m_geo->open(path + LS("GeoLiteCity.dat"), path + LS("GeoIPASNum.dat"))) {
#   if defined(Q_OS_LINUX)
    m_geo->open(LS("/usr/share/GeoIP/GeoIPCity.dat"), LS("/usr/share/GeoIP/GeoIPASNum.dat"));
#   endif
  }
}


GeoIPImpl::~GeoIPImpl()
{
  delete m_geo;
}


void GeoIPImpl::geo(const QString &ip, QVariantMap &out)
{
//  QGeoIPRecord record = m_geo->recordByAddr(QHostAddress("58.8.141.253"));
  QGeoIPRecord record = m_geo->recordByAddr(QHostAddress(ip));
  if (record.country.isEmpty())
    return;

  out[LS("countryName")] = record.countryName;
  out[LS("country")]     = record.country;
  out[LS("continent")]   = record.continent;
  out[LS("region")]      = record.region;
  out[LS("city")]        = record.city;
  out[LS("org")]         = record.org;
  out[LS("tz")]          = record.tz;
  out[LS("latitude")]    = static_cast<double>(record.latitude);
  out[LS("longitude")]   = static_cast<double>(record.longitude);
}
