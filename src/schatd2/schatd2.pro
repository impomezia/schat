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

SCHAT_RESOURCES     = 0
SCHAT_RC_FILE       = 1
SCHAT_SINGLEAPP     = 0
SCHAT_RESOURCES     = 1
SCHAT_LITTLE_CLIENT = 1

TARGET=schat-proxy

QT = core network sql
TEMPLATE = app
    
DEFINES += SCHAT_DAEMON
SCHAT_DAEMON_LIB = 1

SOURCES = NodeInit.cpp schatd2.cpp

HEADERS = \
    Ch.h \
    ChHook.h \
    Client.h \
    ClientListener.h \
    cores/AuthProxy.h \
    cores/Core.h \
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
    NodeInit.h \
    NodeLog.h \
    NodeNoticeReader.h \
    NodePlugins.h \
    Normalize.h \
    plugins/NodePlugin.h \
    ServerChannel.h \
    SettingsHook.h \
    ShortID.h \
    SJMPPacket.h \
    Sockets.h \
    Storage.h \
    StorageHook.h \
    tools/CmdLine.h \

SOURCES = \
    Ch.cpp \
    ChHook.cpp \
    Client.cpp \
    ClientListener.cpp \
    cores/AuthProxy.cpp \
    cores/Core.cpp \
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
    NodeInit.cpp \
    NodeLog.cpp \
    NodeNoticeReader.cpp \
    NodePlugins.cpp \
    Normalize.cpp \
    plugins/NodePlugin.cpp \
    schatd2.cpp \
    ServerChannel.cpp \
    SettingsHook.cpp \
    ShortID.cpp \
    SJMPPacket.cpp \
    Sockets.cpp \
    Storage.cpp \
    StorageHook.cpp \
    tools/CmdLine.cpp \

include(../common/plugins/GenericNode/GenericNode.pri)
include(../common/plugins/Messages/Messages.pri)
include(../common/plugins/NodeChannels/NodeChannels.pri)
include(../common/plugins/NodeMaster/NodeMaster.pri)

include(../common/client.pri)
include(../common/config.pri)
include(../common/common.pri)
include(install.pri)

if (win32-msvc*) {
  include(../common/prefix.pri)
  
  HEADERS += ExceptionHandler.h
  SOURCES += ExceptionHandler.cpp
  LIBS += -lshell32
}
