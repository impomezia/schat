#!/bin/bash
# $Id: build.sh 3242 2012-10-30 09:37:15Z IMPOMEZIA $
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

set -e

cat debian/changelog.in | sed "s/##RDATE##/`date -R`/g" | sed "s/##DIST##/`lsb_release -cs`/g" | sed "s/##SCHAT_VERSION##/$SCHAT_VERSION/g" > debian/changelog
cp -fr debian ../../
cd ../..
dpkg-buildpackage -us -uc
cp -f ../*.deb os/ubuntu/deb
rm ../schat*.deb ../schat2*.changes ../schat2*.dsc ../schat2*.tar.gz