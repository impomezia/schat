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

SCHAT_RESOURCES   = 0
SCHAT_RC_FILE     = 1
SCHAT_SINGLEAPP   = 0

QT = core network sql
TEMPLATE = app
    
DEFINES += SCHAT_DAEMON
SCHAT_DAEMON_LIB = 1

win32 {
  QT += gui

  greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
  }

  HEADERS = \
    NodeInit.h \
    ui/DaemonUi.h \

  SOURCES = \
    NodeInit.cpp \
    schatd2-ui.cpp \
    ui/DaemonUi.cpp \
 
  SCHAT_RESOURCES = 0
  SCHAT_SINGLEAPP = 1
}
else {
  HEADERS = NodeInit.h
  SOURCES = NodeInit.cpp schatd2.cpp
}

include(../common/config.pri)
include(../common/common.pri)
include(install.pri)

if (win32-msvc*) {
  include(../common/prefix.pri)
  
  HEADERS += ExceptionHandler.h
  SOURCES += ExceptionHandler.cpp
  LIBS += -lshell32
}

contains( SCHAT_SINGLE_APP, 1 ) {
  HEADERS += qtsingleapplication/qtsingleapplication.h qtsingleapplication/qtlocalpeer.h
  SOURCES += qtsingleapplication/qtsingleapplication.cpp qtsingleapplication/qtlocalpeer.cpp
}
else {
  DEFINES += SCHAT_NO_SINGLEAPP
}

