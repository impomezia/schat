# $Id: tufao.pro 3514 2013-02-22 19:20:06Z IMPOMEZIA $
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

TEMPLATE = lib
QT = core network
TARGET = schat-tufao
DEFINES += TUFAO_LIBRARY
win32:RC_FILE = tufao.rc

DEPENDPATH += ../3rdparty/Tufao
VPATH += ../3rdparty/Tufao
INCLUDEPATH += ../3rdparty/Tufao

unix:!macx{
  include(../common/prefix.pri)

  target.path += $${LIBDIR}
  INSTALLS += target
}

win32 {
  target.path += ../../os/win32/schatd2/
  INSTALLS += target
}

include(../common/config.pri)

SOURCES += \
    abstracthttpserverrequesthandler.cpp \
    headers.cpp \
    httpserver.cpp \
    httpserverrequest.cpp \
    httpserverrequestrouter.cpp \
    httpserverresponse.cpp \
    httpsserver.cpp \
    priv/asctime.cpp \
    priv/http_parser.c \
    priv/reasonphrase.cpp \
    priv/rfc1036.cpp \
    priv/rfc1123.cpp \
    priv/tcpserverwrapper.cpp \
    querystring.cpp \
    url.cpp \

HEADERS += \
    abstracthttpserverrequesthandler.h \
    headers.h \
    httpserver.h \
    httpserverrequest.h \
    httpserverrequestrouter.h \
    httpserverresponse.h \
    httpsserver.h \
    ibytearray.h \
    priv/asctime.h \
    priv/http_parser.h \
    priv/httpserver.h \
    priv/httpserverrequest.h \
    priv/httpserverrequestrouter.h \
    priv/httpserverresponse.h \
    priv/httpsserver.h \
    priv/reasonphrase.h \
    priv/rfc1036.h \
    priv/rfc1123.h \
    priv/tcpserverwrapper.h \
    priv/url.h \
    querystring.h \
    tufao_global.h \
    url.h \

include(../common/common.pri)
