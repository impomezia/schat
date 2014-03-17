# $Id: schatd2.pro 3535 2013-02-27 22:42:00Z IMPOMEZIA $
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

SCHAT_RESOURCES   = 0
SCHAT_RC_FILE     = 1
SCHAT_SINGLEAPP   = 0

QT = core network sql
TEMPLATE = app
CONFIG += console

HEADERS = \
    NodeInit.h \

SOURCES = \
    NodeInit.cpp \
    schatd2.cpp \

DEFINES += SCHAT_DAEMON
SCHAT_DAEMON_LIB = 1

include(../common/config.pri)
include(../common/common.pri)
include(install.pri)

