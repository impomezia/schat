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

CONFIG   += ordered
TEMPLATE = subdirs
SUBDIRS  = \
    schatd2/schatd.pro \
    tufao \
    rest \
    schatd2 \
    authd \
    client \
    schat2/schat2-core.pro \
    schat2 \
    common/plugins \
    
win32 {
  lessThan(QT_MAJOR_VERSION, 5):SUBDIRS += schatd2/schatd2-srv.pro
  
  SUBDIRS += crashreport
}
