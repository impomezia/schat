# Simple Chat
# Copyright (c) 2008-2014 Alexander Sedov <imp@schat.me>
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

HEADERS  = \
   feeds/NodeChannelFeed.h \
   feeds/NodeFeedStorage.h \
   feeds/NodeHostsFeed.h \
   feeds/NodeProfileFeed.h \
   feeds/NodeServerFeed.h \
   feeds/NodeUserFeed.h \
   GenericCh.h \
   GenericNodePlugin.h \
   GenericNodePlugin_p.h \
   NodeChannels.h \
   NodeFeeds.h \

SOURCES  = \
   feeds/NodeChannelFeed.cpp \
   feeds/NodeFeedStorage.cpp \
   feeds/NodeHostsFeed.cpp \
   feeds/NodeProfileFeed.cpp \
   feeds/NodeServerFeed.cpp \
   feeds/NodeUserFeed.cpp \
   GenericCh.cpp \
   GenericNodePlugin.cpp \
   NodeChannels.cpp \
   NodeFeeds.cpp \

include(../plugins.pri)
