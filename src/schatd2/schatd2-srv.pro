# $Id: schatd2-srv.pro 3153 2012-10-11 15:58:35Z IMPOMEZIA $
# IMPOMEZIA Simple Chat
# Copyright (c) 2008-2012 IMPOMEZIA <schat@impomezia.com>
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

SCHAT_RESOURCES   = 0
SCHAT_RC_FILE     = 1
SCHAT_SINGLEAPP   = 0

QT = core network sql
TEMPLATE = app

target.path += ../../os/win32/schatd2/
INSTALLS += target

HEADERS = \
    NodeInit.h \
    qtservice/qtservice.h \
    qtservice/qtservice_p.h \
    ServiceApp.h \

SOURCES = \
    NodeInit.cpp \
    qtservice/qtservice.cpp \
    qtservice/qtservice_win.cpp \
    schatd2-srv.cpp \
    ServiceApp.cpp \

DEFINES += SCHAT_DAEMON
SCHAT_DAEMON_LIB = 1

include(../common/config.pri)
include(../common/common.pri)

