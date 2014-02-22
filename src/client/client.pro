# $Id: client.pro 3560 2013-03-05 21:43:23Z IMPOMEZIA $
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

TEMPLATE = lib
QT = core network
TARGET = schat-client
DEFINES += SCHAT_LIBRARY
win32:RC_FILE = client.rc

unix {
  macx {
    target.path += ../../out/SimpleChat2.app/Contents/Frameworks/
  } else {
    include(../common/prefix.pri)

    target.path += $${LIBDIR}
  }

  INSTALLS += target
}

win32 {
  target.path += ../../os/win32/schat2/
  INSTALLS += target
}

TRANSLATIONS += ../../res/translations/schat2-client_en.ts
TRANSLATIONS += ../../res/translations/schat2-client_ru.ts
TRANSLATIONS += ../../res/translations/schat2-client_uk.ts
CODECFORTR  = UTF-8
CODECFORSRC = UTF-8 

include(../common/config.pri)
include(../common/client.pri)
include(../common/common.pri)
