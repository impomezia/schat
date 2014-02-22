# $Id: benchmark.pro 2122 2012-01-08 23:05:44Z IMPOMEZIA $
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

SCHAT_CONSOLE    = 1
SCHAT_DEBUG      = 0
SCHAT_RESOURCES  = 0
SCHAT_RC_FILE    = 0
SCHAT_SINGLEAPP  = 0
BENCHMARK_NO_UI  = 0
SCHAT_CLIENT_LIB = 1

QT = core network
TEMPLATE = app

HEADERS = \
    benchmark.h \
    benchmarkapp.h \
    BenchmarkClient.h \

SOURCES = \
    benchmark.cpp \
    benchmarkapp.cpp \
    BenchmarkClient.cpp \
    main.cpp \

CODECFORTR = UTF-8

contains( BENCHMARK_NO_UI, 1 ) {
  CONFIG += console
  DEFINES += BENCHMARK_NO_UI
}
else {
  CONFIG += console
  QT += gui
  HEADERS += benchmarkui.h
  SOURCES += benchmarkui.cpp
}

include(../common/common.pri)
