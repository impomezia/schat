#!/bin/bash
# $Id: obs-upload.sh 3558 2013-03-05 04:01:59Z IMPOMEZIA $
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

set -e

PROJECT=$1
VERSION=$2
SOURCE=schat2-$VERSION

cd ..

rm -fr $PROJECT
osc co $PROJECT schat2
cd $PROJECT/schat2
if [ -f *.tar.bz2 ]
then
  osc rm *.tar.bz2
fi
cd ../..

rm -fr $SOURCE
cp -fr build $SOURCE
find $SOURCE -name .svn -exec rm -rf {} \; > /dev/null 2>&1 || true
tar -cjf $SOURCE.tar.bz2 $SOURCE
mv $SOURCE.tar.bz2 $PROJECT/schat2

cd $PROJECT/schat2
if [ ! -f schat2.spec ]
then
  touch schat2.spec
  osc add schat2.spec
fi

cat ../../build/os/obs/schat2.spec | sed "s/##SCHAT_VERSION##/$VERSION/g" > schat2.spec
osc add $SOURCE.tar.bz2
osc commit -m "Simple Chat $VERSION" --skip-validation 
