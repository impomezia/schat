# $Id: schat2-bot.pri 1570 2011-07-25 14:52:01Z IMPOMEZIA $
# IMPOMEZIA Simple Chat
# Copyright (c) 2008-2011 IMPOMEZIA <schat@impomezia.com>
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

QT = core network
TEMPLATE = app

HEADERS = \
   BotApp.h \
   BotPlugins.h \
   version.h \

SOURCES = \
   BotApp.cpp \
   BotPlugins.cpp \
   main.cpp \
  
SCHAT_CLIENT_LIB = 1

include(../common/common.pri)

