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
QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
+greaterThan(QT_MAJOR_VERSION, 4): QT += x11extras

HEADERS  = \
   Idle.h \
   IdlePlugin.h \
   IdlePlugin_p.h \
   IdleSettings.h \

SOURCES  = \
   Idle.cpp \
   IdlePlugin.cpp \
   IdleSettings.cpp \

RESOURCES += Idle.qrc

unix {
  macx {
    SOURCES += idle_mac.cpp
    LIBS += -framework IOKit -framework Cocoa
  }
  else {
    SOURCES += idle_x11.cpp

    system(pkg-config --modversion xscrnsaver > /dev/null 2>&1) {
      CONFIG    += link_pkgconfig
      PKGCONFIG += xscrnsaver
    }
    else {
      DEFINES += SCHAT_NO_XSS
      message(XScrnSaver not found. Idle plugin will use minimal functionality.)
    }
  }
} else:win32 {
  SOURCES += idle_win.cpp
  LIBS += -luser32
} else {
  SOURCES += idle_stub.cpp
}

TRANSLATIONS += res/translations/idle_en.ts
TRANSLATIONS += res/translations/idle_ru.ts
TRANSLATIONS += res/translations/idle_uk.ts

include(../plugins.pri)
