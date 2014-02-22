# $Id: NodeFeedLog.pro 3411 2013-01-17 04:07:15Z IMPOMEZIA $
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
OTHER_FILES += NodeFeedLog.json

HEADERS  = \
   NodeFeedLogPlugin.h \
   NodeFeedLogPlugin_p.h \

SOURCES  = \
   NodeFeedLogPlugin.cpp \

include(../plugins.pri)
