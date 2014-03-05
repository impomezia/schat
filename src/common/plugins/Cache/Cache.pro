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

SCHAT_CLIENT_LIB = 1
SCHAT_CORE_LIB = 1
QT = core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

HEADERS  = \
   CacheChannels.h \
   CacheDB.h \
   CachePlugin.h \
   CachePlugin_p.h \
   feeds/CacheFeedStorage.h \
   StateCache.h \
   TalksCache.h \

SOURCES  = \
   CacheChannels.cpp \
   CacheDB.cpp \
   CachePlugin.cpp \
   feeds/CacheFeedStorage.cpp \
   StateCache.cpp \
   TalksCache.cpp \

RESOURCES += Cache.qrc

TRANSLATIONS += res/translations/cache_en.ts
TRANSLATIONS += res/translations/cache_ru.ts
TRANSLATIONS += res/translations/cache_uk.ts

include(../plugins.pri)
