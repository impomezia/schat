# $Id: Channels.pro 3600 2013-03-25 23:11:33Z IMPOMEZIA $
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

SCHAT_CLIENT_LIB = 1
SCHAT_CORE_LIB = 1
QT = core gui network webkit
OTHER_FILES += Channels.json

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets

HEADERS  = \
   ChannelsCmd.h \
   ChannelsFeedListener.h \
   ChannelsMenuImpl.h \
   ChannelsPlugin.h \
   ChannelsPlugin_p.h \
   ChannelsSettings.h \
   ui/ChannelsView.h \
   ui/ListTab.h \

SOURCES  = \
   ChannelsCmd.cpp \
   ChannelsFeedListener.cpp \
   ChannelsMenuImpl.cpp \
   ChannelsPlugin.cpp \
   ChannelsSettings.cpp \
   ui/ChannelsView.cpp \
   ui/ListTab.cpp \

RESOURCES += Channels.qrc

TRANSLATIONS += res/translations/channels_en.ts
TRANSLATIONS += res/translations/channels_ru.ts
TRANSLATIONS += res/translations/channels_uk.ts
CODECFORTR  = UTF-8
CODECFORSRC = UTF-8

include(../plugins.pri)
