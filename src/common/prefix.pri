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

isEmpty(GIT_TIMESTAMP) {
    GIT_TIMESTAMP = $$system($$quote(git log -n 1 --format=format:"%at"))
    GIT_REVISION  = $$system($$quote(git rev-parse HEAD))
    
    DEFINES += GIT_TIMESTAMP=$$GIT_TIMESTAMP
    DEFINES += GIT_REVISION=\\\"$${GIT_REVISION}\\\"
}

lessThan(QT_MAJOR_VERSION, 5):DEFINES += Q_DECL_OVERRIDE
