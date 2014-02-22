/* $Id: QGeoIP.h 3542 2013-03-01 23:42:49Z IMPOMEZIA $
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

#ifndef QGEOIP_H
#define QGEOIP_H

#include <QString>
#include <QHostAddress>
#include <QMetaType>

#if defined(SCHAT_EXTERNAL_GEOIP)
# include <GeoIP.h>
# include <GeoIPCity.h>
#else
# include "GeoIP/GeoIP.h"
# include "GeoIP/GeoIPCity.h"
#endif

#ifdef DEBUG
#include <QDebug>
#endif

struct QGeoIPRecord {
  QGeoIPRecord()
    : latitude(0)
    , longitude(0)
  {}

  float latitude;
  float longitude;
  QString city;
  QString continent;
  QString country; // Stores a ISO-3166-1-alpha2 country code.
  QString countryName;
  QString org;
  QString region;
  QString tz;
};

// Register metatype to allow these types to be streamed in QTests.
Q_DECLARE_METATYPE(QGeoIPRecord)

#ifdef DEBUG
// QDebug() streaming output operators.
QDebug operator<<(QDebug dbg, const QGeoIPRecord & record);
#endif


class QGeoIP
{
public:
  QGeoIP();
  ~QGeoIP();

  bool open(const QString &city, const QString &asn = QString());
  void close();
  QGeoIPRecord recordByAddr(const QHostAddress &ip);

private:
  bool open(GeoIP **db, const QString &fileName);
  void close(GeoIP **db);
  bool isOpen(GeoIP const * const db) const;

  GeoIP *m_asn;
  GeoIP *m_city;
};

#endif // QGEOIP_H
