#!/bin/bash
# Simple Chat
# Copyright (c) 2008-2015 Alexander Sedov <imp@schat.me>
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

SCHAT_VERSION=3.0.0

cat debian/changelog.in | sed "s/##RDATE##/`date -R`/g" | sed "s/##DIST##/`lsb_release -cs`/g" | sed "s/##SCHAT_VERSION##/$SCHAT_VERSION/g" > debian/changelog
cp -fr debian ../../
cd ../..
dpkg-buildpackage -us -uc
cp -f ../*.deb os/ubuntu/deb
rm ../schat-proxy*.deb ../schat-proxy*.changes ../schat-proxy*.dsc ../schat-proxy*.tar.gz