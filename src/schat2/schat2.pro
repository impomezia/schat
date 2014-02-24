# $Id: schat2.pro 3768 2013-08-13 22:46:46Z IMPOMEZIA $
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

sounds.files += ../../res/sounds/Received.wav

unix {  
  macx {
    translations.path += ../../out/SimpleChat2.app/Contents/Resources/translations
    sounds.path  += ../../out/SimpleChat2.app/Contents/Resources/sounds/

    INSTALLS += translations sounds
  } else {
    icon16.files = ../../res/images/icons/16x16/schat2.png
    icon16.path = $${PREFIX}/share/icons/hicolor/16x16/apps

    icon22.files = ../../res/images/icons/22x22/schat2.png
    icon22.path = $${PREFIX}/share/icons/hicolor/22x22/apps

    icon24.files = ../../res/images/icons/24x24/schat2.png
    icon24.path = $${PREFIX}/share/icons/hicolor/24x24/apps

    icon32.files = ../../res/images/icons/32x32/schat2.png
    icon32.path = $${PREFIX}/share/icons/hicolor/32x32/apps

    icon36.files = ../../res/images/icons/36x36/schat2.png
    icon36.path = $${PREFIX}/share/icons/hicolor/36x36/apps

    icon48.files = ../../res/images/icons/48x48/schat2.png
    icon48.path = $${PREFIX}/share/icons/hicolor/48x48/apps

    icon64.files = ../../res/images/icons/64x64/schat2.png
    icon64.path = $${PREFIX}/share/icons/hicolor/64x64/apps

    icon72.files = ../../res/images/icons/72x72/schat2.png
    icon72.path = $${PREFIX}/share/icons/hicolor/72x72/apps

    icon96.files = ../../res/images/icons/96x96/schat2.png
    icon96.path = $${PREFIX}/share/icons/hicolor/96x96/apps

    icon128.files = ../../res/images/icons/128x128/schat2.png
    icon128.path = $${PREFIX}/share/icons/hicolor/128x128/apps

    icon196.files = ../../res/images/icons/196x196/schat2.png
    icon196.path = $${PREFIX}/share/icons/hicolor/196x196/apps

    icon256.files = ../../res/images/icons/256x256/schat2.png
    icon256.path = $${PREFIX}/share/icons/hicolor/256x256/apps

    applications.files = ../../os/ubuntu/schat2.desktop
    applications.path = $${PREFIX}/share/applications

    translations.path = $${PREFIX}/share/schat2/translations
    sounds.path  += $${PREFIX}/share/schat2/sounds/
    target.path += $${PREFIX}/bin
    
    INSTALLS += target applications translations sounds icon16 icon22 icon24 icon32 icon36 icon48 icon64 icon72 icon96 icon128 icon196 icon256
  }
}

win32 {
  translations.files += $$[QT_INSTALL_PREFIX]/translations/qt_ru.qm
  translations.files += $$[QT_INSTALL_PREFIX]/translations/qt_uk.qm
  translations.path += ../../os/win32/schat2/translations

  qt.files += $$[QT_INSTALL_PREFIX]/bin/libeay32.dll
  qt.files += $$[QT_INSTALL_PREFIX]/bin/ssleay32.dll
  qt.files += $$[QT_INSTALL_PREFIX]/bin/msvcp100.dll
  qt.files += $$[QT_INSTALL_PREFIX]/bin/msvcr100.dll

  greaterThan(QT_MAJOR_VERSION, 4) {
    qt.files += $$[QT_INSTALL_PREFIX]/bin/Qt5Core.dll
    qt.files += $$[QT_INSTALL_PREFIX]/bin/Qt5Gui.dll
    qt.files += $$[QT_INSTALL_PREFIX]/bin/Qt5Widgets.dll
    qt.files += $$[QT_INSTALL_PREFIX]/bin/Qt5Network.dll
    qt.files += $$[QT_INSTALL_PREFIX]/bin/Qt5Sql.dll
    qt.files += $$[QT_INSTALL_PREFIX]/bin/Qt5WebKit.dll
    qt.files += $$[QT_INSTALL_PREFIX]/bin/Qt5WebKitWidgets.dll
    qt.files += $$[QT_INSTALL_PREFIX]/bin/Qt5Multimedia.dll
    qt.files += $$[QT_INSTALL_PREFIX]/bin/Qt5MultimediaWidgets.dll
    qt.files += $$[QT_INSTALL_PREFIX]/bin/Qt5OpenGL.dll
    qt.files += $$[QT_INSTALL_PREFIX]/bin/Qt5PrintSupport.dll
    qt.files += $$[QT_INSTALL_PREFIX]/bin/Qt5Quick.dll
    qt.files += $$[QT_INSTALL_PREFIX]/bin/Qt5Qml.dll
    qt.files += $$[QT_INSTALL_PREFIX]/bin/Qt5V8.dll
    qt.files += $$[QT_INSTALL_PREFIX]/bin/D3DCompiler_43.dll
    qt.files += $$[QT_INSTALL_PREFIX]/bin/libEGL.dll
    qt.files += $$[QT_INSTALL_PREFIX]/bin/libGLESv2.dll
    qt.files += $$[QT_INSTALL_PREFIX]/bin/icudt49.dll
    qt.files += $$[QT_INSTALL_PREFIX]/bin/icuin49.dll
    qt.files += $$[QT_INSTALL_PREFIX]/bin/icuuc49.dll
  } else {
    qt.files += $$[QT_INSTALL_PREFIX]/bin/libpng15.dll
    qt.files += $$[QT_INSTALL_PREFIX]/bin/zlib.dll
    qt.files += $$[QT_INSTALL_PREFIX]/bin/QtCore4.dll
    qt.files += $$[QT_INSTALL_PREFIX]/bin/QtGui4.dll
    qt.files += $$[QT_INSTALL_PREFIX]/bin/QtNetwork4.dll
    qt.files += $$[QT_INSTALL_PREFIX]/bin/QtSql4.dll
    qt.files += $$[QT_INSTALL_PREFIX]/bin/QtWebKit4.dll
    qt.files += ../../os/win32/qt.conf
  }

  qt.path += ../../os/win32/schat2/

  greaterThan(QT_MAJOR_VERSION, 4) {
    imageformats.files += $$[QT_INSTALL_PREFIX]/plugins/imageformats/qgif.dll
    imageformats.files += $$[QT_INSTALL_PREFIX]/plugins/imageformats/qico.dll
    imageformats.files += $$[QT_INSTALL_PREFIX]/plugins/imageformats/qjpeg.dll
  } else {
    imageformats.files += $$[QT_INSTALL_PREFIX]/plugins/imageformats/qgif4.dll
    imageformats.files += $$[QT_INSTALL_PREFIX]/plugins/imageformats/qico4.dll
    imageformats.files += $$[QT_INSTALL_PREFIX]/plugins/imageformats/qjpeg4.dll
  }
  imageformats.path += ../../os/win32/schat2/plugins/qt/imageformats
  
  greaterThan(QT_MAJOR_VERSION, 4) {
    sqldrivers.files += $$[QT_INSTALL_PREFIX]/plugins/sqldrivers/qsqlite.dll
  } else {
    sqldrivers.files += $$[QT_INSTALL_PREFIX]/plugins/sqldrivers/qsqlite4.dll
  }
  sqldrivers.path += ../../os/win32/schat2/plugins/qt/sqldrivers

  greaterThan(QT_MAJOR_VERSION, 4) {
    bearer.files += $$[QT_INSTALL_PREFIX]/plugins/bearer/qgenericbearer.dll
  } else {
    bearer.files += $$[QT_INSTALL_PREFIX]/plugins/bearer/qgenericbearer4.dll
  }
  bearer.path += ../../os/win32/schat2/plugins/qt/bearer

  doc.files += ../../res/doc/ChangeLog.html
  doc.path += ../../os/win32/schat2/doc/

  sounds.path += ../../os/win32/schat2/sounds/

  target.path += ../../os/win32/schat2/
  INSTALLS += target translations qt imageformats sqldrivers bearer doc sounds

  greaterThan(QT_MAJOR_VERSION, 4) {
    platforms.files += $$[QT_INSTALL_PREFIX]/plugins/platforms/qwindows.dll
    platforms.path += ../../os/win32/schat2/platforms
    INSTALLS += platforms
  }
}

win32:LIBS += -luser32

include(../common/config.pri)
include(../common/common.pri)

contains( SCHAT_SINGLE_APP, 1 ) {
    HEADERS += qtsingleapplication/qtsingleapplication.h qtsingleapplication/qtlocalpeer.h
    SOURCES += qtsingleapplication/qtsingleapplication.cpp qtsingleapplication/qtlocalpeer.cpp
}
else {
    DEFINES += SCHAT_NO_SINGLEAPP
}
