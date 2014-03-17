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

include(../common/prefix.pri)

SCHAT_RESOURCES   = 0
SCHAT_RC_FILE     = 1
SCHAT_SINGLEAPP   = 0

macx {
   TARGET = SimpleChat2
   ICON = ../../os/macosx/schat.icns
   QMAKE_INFO_PLIST = ../../os/macosx/Info.plist
}

QT = core gui network webkit
TEMPLATE = app

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
}

HEADERS = \
    ChatApp.h \
    debugstream.h \
    ui/ChatWindow.h \

SOURCES = \
    ChatApp.cpp \
    main.cpp \
    ui/ChatWindow.cpp \

DEFINES += SCHAT_WEBKIT

SCHAT_CLIENT_LIB = 1
SCHAT_CORE_LIB = 1

win32 {
  greaterThan(QT_MAJOR_VERSION, 4):greaterThan(QT_MINOR_VERSION, 1) {
    QT += winextras
  }
  else {
    HEADERS += qtwin/qtwin.h
    SOURCES += qtwin/qtwin.cpp
  }

  SOURCES += ui/ChatWindow_win.cpp

  if (win32-msvc*) {
    HEADERS += ExceptionHandler.h
    SOURCES += ExceptionHandler.cpp
  }

  LIBS += -luser32
}

unix {
  macx {
    SOURCES += ui/ChatWindow_mac.cpp
  }
  else {
    SOURCES += ui/ChatWindow_unix.cpp
  }
}

for(LANG, AVAILABLE_LANGS) {
  TS = ../../res/translations/schat2-client_$${LANG}.ts ../../res/translations/schat2_$${LANG}.ts
  QM = ../../res/translations/schat2_$${LANG}.qm

  system($${LRELEASE} -silent $${TS} -qm $${QM})

  win32:QM = $$replace(QM, /, \\\\)
  QMAKE_DISTCLEAN += $${QM}

  translations.files += ../../res/translations/schat2_$${LANG}.qm
  translations.files += ../../res/translations/$${LANG}.png
}

include(../common/config.pri)
include(../common/common.pri)
include(install.pri)

contains( SCHAT_SINGLE_APP, 1 ) {
    HEADERS += qtsingleapplication/qtsingleapplication.h qtsingleapplication/qtlocalpeer.h
    SOURCES += qtsingleapplication/qtsingleapplication.cpp qtsingleapplication/qtlocalpeer.cpp
}
else {
    DEFINES += SCHAT_NO_SINGLEAPP
}
