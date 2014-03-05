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
CONFIG += exceptions

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEPENDPATH += 3rdparty
INCLUDEPATH += 3rdparty

HEADERS  = \
   HunSpellChecker.h \
   SpellBackend.h \
   SpellChecker.h \
   SpellCheckerPlugin.h \
   SpellCheckerWidget.h \
   SpellHighlighter.h \

SOURCES  = \
   HunSpellChecker.cpp \
   SpellBackend.cpp \
   SpellChecker.cpp \
   SpellCheckerPlugin.cpp \
   SpellCheckerWidget.cpp \
   SpellHighlighter.cpp \

RESOURCES += SpellChecker.qrc

unix:!macx {
  system(pkg-config --modversion hunspell > /dev/null 2>&1) {
    CONFIG    += link_pkgconfig
    PKGCONFIG += hunspell
    DEFINES   += SCHAT_EXTERNAL_HUNSPELL
  }
  else {
    include(3rdparty/hunspell/hunspell.pri)
  }
}
else {
  include(3rdparty/hunspell/hunspell.pri)
}

TRANSLATIONS += res/translations/spellchecker_en.ts
TRANSLATIONS += res/translations/spellchecker_ru.ts
TRANSLATIONS += res/translations/spellchecker_uk.ts

include(../plugins.pri)

macx:dictionaries.path += ../../../../out/SimpleChat2.app/Contents/Resources/spelling
win32:dictionaries.path += ../../../../os/win32/$${PLUGIN_TARGET}/spelling

win32|macx {
  dictionaries.files += spelling/en_US.aff
  dictionaries.files += spelling/en_US.dic
  dictionaries.files += spelling/ru_RU.aff
  dictionaries.files += spelling/ru_RU.dic
  
  INSTALLS += dictionaries
}
