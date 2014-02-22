# $Id: prefix.pri 3555 2013-03-04 23:55:58Z IMPOMEZIA $
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

unix:!macx {
  isEmpty(PREFIX):PREFIX = /usr
  isEmpty(LIBDIR):LIBDIR = $${PREFIX}/lib

  DEFINES += SCHAT_PREFIX=\\\"$${PREFIX}\\\"
  DEFINES += SCHAT_LIBDIR=\\\"$${LIBDIR}\\\"

  LRELEASE = $$[QT_INSTALL_BINS]/lrelease
}

AVAILABLE_LANGS=en ru uk

isEmpty(LRELEASE) {
  LRELEASE = $$[QT_INSTALL_BINS]/lrelease

  win32 {
    LRELEASE = $$replace(LRELEASE, \\\\, \\\\)
    LRELEASE = $$replace(LRELEASE, /, \\\\)
  }
}
