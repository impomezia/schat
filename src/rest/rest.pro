# $Id: rest.pro 3514 2013-02-22 19:20:06Z IMPOMEZIA $
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
TARGET = schat-rest
DEFINES += SCHAT_REST_LIBRARY
win32:RC_FILE = rest.rc

SCHAT_DAEMON_LIB  = 1
SCHAT_TUFAO_LIB   = 1

DEPENDPATH  += ../schatd2
INCLUDEPATH += ../schatd2

unix:!macx{
  include(../common/prefix.pri)

  target.path += $${LIBDIR}
  INSTALLS += target
}

win32 {
  target.path += ../../os/win32/schatd2/
  INSTALLS += target
}

HEADERS += \
    RestApiCore.h \
    RestHandler.h \

SOURCES += \
    RestApiCore.cpp \
    RestHandler.cpp \

include(../common/config.pri)
include(../common/common.pri)
