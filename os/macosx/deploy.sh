#!/bin/bash
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

VERSION="2.3.2"
TARGET="SimpleChat2"

set -e

cd ../..
qmake -r
make -j9
make install
cd out

macdeployqt $TARGET.app

DMG="dmg/$TARGET-$VERSION.dmg"

cd ../os/macosx
test -f $DMG && rm $DMG
./create-dmg/create-dmg --window-size 500 300 --volicon schat.icns --icon-size 96 --volname "Simple Chat 2" --background background.png --icon "Applications" 380 205 --icon "SimpleChat2" 110 205 $DMG ../../out/SimpleChat2.app
hdiutil internet-enable -yes $DMG 



