/* $Id: QGeoIP.cpp 3549 2013-03-02 23:44:00Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2013 IMPOMEZIA <schat@impomezia.com>
 *
 * Original QGeoIPRecord and QGeoIP classes
 * Copyright © Wim Leers <http://wimleers.com/> <https://github.com/wimleers/QGeoIP>
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

#include <QFile>

#include "QGeoIP.h"

QGeoIP::QGeoIP()
  : m_asn(0)
  , m_city(0)
{
}


QGeoIP::~QGeoIP() {
  close();
}


bool QGeoIP::open(const QString &city, const QString &asn) {
  if (!QFile::exists(city) || !open(&m_city, city))
    return false;

  if (!asn.isEmpty() && QFile::exists(asn) && !open(&m_asn, asn))
    return false;

  return true;
}


void QGeoIP::close() {
  close(&m_city);
  close(&m_asn);
}


/**
 * Resolve an IP address to a QGeoIPRecord, in which the following information
 * is contained:
 * - the country code (ISO-3166-1-alpha2)
 * - the country name
 * - city name
 * - region name
 * - latitude and longitude
 *
 * @return
 *   The corresponding QGeoIPRecord.
 */
QGeoIPRecord QGeoIP::recordByAddr(const QHostAddress &ip) {
  if (!isOpen(m_city) || ip.isNull())
    return QGeoIPRecord();

  GeoIPRecord *r;
  if (ip.protocol() == QAbstractSocket::IPv6Protocol)
    r = GeoIP_record_by_addr_v6(m_city, ip.toString().toLatin1().constData());
  else
    r = GeoIP_record_by_ipnum(m_city, ip.toIPv4Address());

  if (!r)
    return QGeoIPRecord();

  QGeoIPRecord result;

  if (isOpen(m_asn)) {
    const char *ispName;
    if (ip.protocol() == QAbstractSocket::IPv6Protocol)
      ispName = GeoIP_org_by_name_v6(m_asn, ip.toString().toLatin1().constData());
    else
      ispName = GeoIP_org_by_ipnum(m_asn, ip.toIPv4Address());

    if (ispName != 0) {
      result.org = ispName;
      free((char *) ispName);
    }
  }

  result.tz          = GeoIP_time_zone_by_country_and_region(r->country_code, r->region);
  result.countryName = r->country_name;
  result.city        = r->city;
  result.region      = GeoIP_region_name_by_code(r->country_code, r->region);
  result.latitude    = r->latitude;
  result.longitude   = r->longitude;
  result.country     = r->country_code;
  result.continent   = r->continent_code;

  GeoIPRecord_delete(r);
  return result;
}


//-----------------------------------------------------------------------------------
// Private methods.

/**
 * Open a GeoIP database. If a database is already opened in the current
 * instance, it will be closed before the requested database is opened.
 *
 * @return
 *   true if successful, otherwise false.
 */
bool QGeoIP::open(GeoIP **db, const QString &fileName) {
# if defined(Q_OS_LINUX)
  *db = GeoIP_open(fileName.toLocal8Bit().constData(), GEOIP_MEMORY_CACHE | GEOIP_CHECK_CACHE);
# else
  *db = GeoIP_open(fileName.toLocal8Bit().constData(), GEOIP_STANDARD);
# endif

  if (isOpen(*db)) {
    GeoIP_set_charset(*db, GEOIP_CHARSET_ISO_8859_1);
    return true;
  }

  return false;
}


void QGeoIP::close(GeoIP **db) {
  if (isOpen(*db)) {
    GeoIP_delete(*db);
    *db = 0;
  }
}


bool QGeoIP::isOpen(GeoIP const * const db) const {
    return db != 0;
}

#ifdef DEBUG
QDebug operator<<(QDebug dbg, const QGeoIPRecord & record) {
    dbg.nospace() << "countryName: "    << record.countryName
                  << "\ncountry:     "  << record.country
                  << "\ncontinent:   "  << record.continent
                  << "\nregion:      "  << record.region
                  << "\ncity:        "  << record.city
                  << "\norg:         "  << record.org
                  << "\ntz:          "  << record.tz
                  << "\nlatitude:     " << record.latitude
                  << "\nlongitude:    " << record.longitude;

    return dbg.nospace();
}
#endif
