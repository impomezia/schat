# $Id: plugins.pro 3488 2013-02-10 15:05:50Z IMPOMEZIA $
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

CONFIG   += ordered
TEMPLATE = subdirs

SUBDIRS += \
    GenericNode \
    Messages \
    NodeChannels \
    NodeConsole \
    RestApi \
    GeoIP \
    NodeFeedLog \
    Cache \
    History \
    Profile \
    Channels \
    Popup \
    Emoticons \
    Idle \
    SendFile \
    SpellChecker \
    Proxy \
    Console \
    RawFeeds \
    YouTube \
    Update \
