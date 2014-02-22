# $Id: RestApi.pro 3442 2013-01-25 19:58:38Z IMPOMEZIA $
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
SCHAT_REST_LIB   = 1
SCHAT_TUFAO_LIB  = 1
QT = core network
OTHER_FILES += RestApi.json

HEADERS  = \
   handlers/ChannelHandler.h \
   handlers/FeedHandler.h \
   handlers/ServerHandler.h \
   RestApi.h \
   RestApiPlugin.h \
   RestApiPlugin_p.h \

SOURCES  = \
   handlers/ChannelHandler.cpp \
   handlers/FeedHandler.cpp \
   handlers/ServerHandler.cpp \
   RestApiPlugin.cpp \

include(../plugins.pri)
