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

SCHAT_RESOURCES = 1

TEMPLATE = lib
QT = core network sql
TARGET = schatd
DEFINES += SCHAT_LIBRARY
win32:RC_FILE = schatd.rc
DEFINES += SCHAT_DAEMON
SCHAT_LITTLE_CLIENT = 1

HEADERS = \
    Ch.h \
    ChHook.h \
    Client.h \
    ClientListener.h \
    cores/AnonymousAuth.h \
    cores/CookieAuth.h \
    cores/Core.h \
    cores/DiscoveryAuth.h \
    cores/ExternalAuth.h \
    cores/NodeAuth.h \
    crc8.h \
    DataBase.h \
    events.h \
    feeds/FeedsCore.h \
    feeds/NodeAclFeed.h \
    Heartbeat.h \
    Host.h \
    Hosts.h \
    interfaces/IClient.h \
    interfaces/IClientListener.h \
    net/DataCreator.h \
    net/Net.h \
    net/NetContext.h \
    net/NetRecord.h \
    net/NodeAuthReply.h \
    net/NodePool.h \
    net/NodeWorker.h \
    net/Subscribers.h \
    net/TcpServer.h \
    NodeLog.h \
    NodeNoticeReader.h \
    NodePlugins.h \
    Normalize.h \
    plugins/GeoHook.h \
    plugins/NodePlugin.h \
    ServerChannel.h \
    SettingsHook.h \
    ShortID.h \
    SJMPPacket.h \
    Sockets.h \
    Storage.h \
    StorageHook.h \
    tools/CmdLine.h \
    User.h \
    cores/AuthProxy.h

SOURCES = \
    Ch.cpp \
    ChHook.cpp \
    Client.cpp \
    ClientListener.cpp \
    cores/AnonymousAuth.cpp \
    cores/CookieAuth.cpp \
    cores/Core.cpp \
    cores/DiscoveryAuth.cpp \
    cores/ExternalAuth.cpp \
    crc8.cpp \
    DataBase.cpp \
    events.cpp \
    feeds/FeedsCore.cpp \
    feeds/NodeAclFeed.cpp \
    Heartbeat.cpp \
    Host.cpp \
    Hosts.cpp \
    net/Net.cpp \
    net/NetContext.cpp \
    net/NodeAuthReply.cpp \
    net/NodePool.cpp \
    net/NodeWorker.cpp \
    net/Subscribers.cpp \
    net/TcpServer.cpp \
    NodeLog.cpp \
    NodeNoticeReader.cpp \
    NodePlugins.cpp \
    Normalize.cpp \
    plugins/GeoHook.cpp \
    plugins/NodePlugin.cpp \
    ServerChannel.cpp \
    SettingsHook.cpp \
    ShortID.cpp \
    SJMPPacket.cpp \
    Sockets.cpp \
    Storage.cpp \
    StorageHook.cpp \
    tools/CmdLine.cpp \
    User.cpp \
    cores/AuthProxy.cpp

include(../common/client.pri)

unix:!macx {
  include(../common/prefix.pri)

  target.path += $${LIBDIR}
  INSTALLS += target
}

win32 {
  target.path += ../../os/win32/schatd2/
  INSTALLS += target
}

include(../common/config.pri)
include(../common/common.pri)
