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
SCHAT_CORE_LIB   = 1
QT = core network sql gui webkit

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets

HEADERS  = \
   PreviewPlugin.h \
   src/PreviewChatView.h \
   src/PreviewCore.h \
   src/PreviewDB.h \
   src/PreviewFilter.h \
   src/PreviewItem.h \
   src/PreviewStorage.h \
   src/PreviewWindowObject.h \

SOURCES  = \
   PreviewPlugin.cpp \
   src/PreviewChatView.cpp \
   src/PreviewCore.cpp \
   src/PreviewDB.cpp \
   src/PreviewFilter.cpp \
   src/PreviewItem.cpp \
   src/PreviewStorage.cpp \
   src/PreviewWindowObject.cpp \

RESOURCES += Preview.qrc

TRANSLATIONS += res/translations/preview_en.ts
TRANSLATIONS += res/translations/preview_ru.ts
TRANSLATIONS += res/translations/preview_uk.ts

include(../plugins.pri)
