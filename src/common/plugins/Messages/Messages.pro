# $Id: Messages.pro 3040 2012-08-29 08:21:23Z IMPOMEZIA $
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

SCHAT_DAEMON_LIB = 1
QT = core network sql
OTHER_FILES += Messages.json

HEADERS  = \
   feeds/NodeMessagesFeed.h \
   MessagesCh.h \
   MessagesPlugin.h \
   MessagesPlugin_p.h \
   NodeMessages.h \
   NodeMessagesDB.h \

SOURCES  = \
   feeds/NodeMessagesFeed.cpp \
   MessagesCh.cpp \
   MessagesPlugin.cpp \
   NodeMessages.cpp \
   NodeMessagesDB.cpp \

include(../plugins.pri)
