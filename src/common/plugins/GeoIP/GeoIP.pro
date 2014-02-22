# $Id: GeoIP.pro 3554 2013-03-04 15:04:23Z IMPOMEZIA $
# IMPOMEZIA Simple Chat
# Copyright (c) 2008-2013 IMPOMEZIA <schat@impomezia.com>
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program. If not, see <http://www.gnu.org/licenses/>.

SCHAT_DAEMON_LIB = 1
QT = core network
OTHER_FILES += GeoIP.json

DEFINES += GEOIPDATADIR=\\\"./res\\\"

HEADERS = \
   GeoIPImpl.h \
   GeoIPPlugin.h \
   GeoIPPlugin_p.h \
   QGeoIP.h \

SOURCES = \
   GeoIPImpl.cpp \
   GeoIPPlugin.cpp \
   QGeoIP.cpp \

unix:!macx {
  equals(GEOIP, 1) {
    LIBS      +=-lGeoIP
    DEFINES   += SCHAT_EXTERNAL_GEOIP
  }
  else:system(pkg-config --modversion geoip > /dev/null 2>&1) {
    CONFIG    += link_pkgconfig
    PKGCONFIG += geoip
    DEFINES   += SCHAT_EXTERNAL_GEOIP
  }
  else {
    include(GeoIP/GeoIP.pri)
  }
}
else {
  include(GeoIP/GeoIP.pri)
}

include(../plugins.pri)
