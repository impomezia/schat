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

SCHAT_VERSION=${SCHAT_VERSION}.${SCHAT_BUILDNUMBER}
PPA_VERSION=${SCHAT_VERSION}-1~ppa0~`git rev-parse --short HEAD`
RDATE=`date -R`
SCHAT_SOURCE=schat2_${SCHAT_VERSION}
PPA_SOURCE=schat2_${PPA_VERSION}

function upload() {
  local DIST=$1
  cat ../../$WORKDIR/os/ubuntu/debian/changelog.ppa | sed "s/##RDATE##/$RDATE/g" | sed "s/##DIST##/$DIST/g" | sed "s/##SCHAT_VERSION##/$PPA_VERSION/g" > debian/changelog
  debuild -S -sa
  dput ppa:impomezia/$SCHAT_PPA ../$PPA_SOURCE~${DIST}_source.changes
}

cp schatd2.upstart debian
cd ../../
WORKDIR=${PWD##*/}

cd ..
rm -fr ppa
mkdir ppa
cp -fR $WORKDIR ppa/$SCHAT_SOURCE
cp -fR $WORKDIR ppa/$PPA_SOURCE
rm -fr ppa/$SCHAT_SOURCE/.git

cd ppa
tar -cJf $SCHAT_SOURCE.orig.tar.xz $SCHAT_SOURCE 
rm -fr $SCHAT_SOURCE
cp -fr ../$WORKDIR/os/ubuntu/debian $PPA_SOURCE

cd $PPA_SOURCE
mkdir debian/source
echo > debian/source/format "3.0 (quilt)"
rm debian/changelog.in
rm debian/changelog.ppa

upload "xenial"
upload "trusty"
upload "precise"
