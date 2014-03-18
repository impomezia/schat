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
  include(../common/prefix.pri)

  target.path += $${PREFIX}/sbin
  INSTALLS += target
}

win32 {
  qt.files += $$[QT_INSTALL_PREFIX]/bin/libeay32.dll
  qt.files += $$[QT_INSTALL_PREFIX]/bin/ssleay32.dll
  qt.files += $$[QT_INSTALL_PREFIX]/bin/msvcp100.dll
  qt.files += $$[QT_INSTALL_PREFIX]/bin/msvcr100.dll 

  greaterThan(QT_MAJOR_VERSION, 4) {
    qt.files += $$[QT_INSTALL_PREFIX]/bin/Qt5Core.dll
    qt.files += $$[QT_INSTALL_PREFIX]/bin/Qt5Network.dll
    qt.files += $$[QT_INSTALL_PREFIX]/bin/Qt5Sql.dll
    qt.files += $$[QT_INSTALL_PREFIX]/bin/icudt49.dll
    qt.files += $$[QT_INSTALL_PREFIX]/bin/icuin49.dll
    qt.files += $$[QT_INSTALL_PREFIX]/bin/icuuc49.dll
  } else {
    qt.files += $$[QT_INSTALL_PREFIX]/bin/zlib.dll
    qt.files += $$[QT_INSTALL_PREFIX]/bin/QtCore4.dll
    qt.files += $$[QT_INSTALL_PREFIX]/bin/QtNetwork4.dll
    qt.files += $$[QT_INSTALL_PREFIX]/bin/QtSql4.dll
  }
  qt.path += ../../os/win32/schatd2/
  
  greaterThan(QT_MAJOR_VERSION, 4) {
    sqldrivers.files += $$[QT_INSTALL_PREFIX]/plugins/sqldrivers/qsqlite.dll
  } else {
    sqldrivers.files += $$[QT_INSTALL_PREFIX]/plugins/sqldrivers/qsqlite4.dll
  }
  sqldrivers.path += ../../os/win32/schatd2/plugins/qt/sqldrivers

  doc.files += ../../res/doc/ChangeLog.html
  doc.path += ../../os/win32/schatd2/doc/

  target.path += ../../os/win32/schatd2/
  INSTALLS += target qt sqldrivers doc

  greaterThan(QT_MAJOR_VERSION, 4) {
    platforms.files += $$[QT_INSTALL_PREFIX]/plugins/platforms/qwindows.dll
    platforms.path += ../../os/win32/schatd2/platforms
    INSTALLS += platforms
  }
}
