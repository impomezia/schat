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

HEADERS += \
    acl/Acl.h \
    acl/AclValue.h \
    acl/Groups.h \
    base32/base32.h \
    Channel.h \
    DateTime.h \
    feeds/Feed.h \
    feeds/FeedCreator.h \
    feeds/FeedEvent.h \
    feeds/FeedEvents.h \
    feeds/FeedHeader.h \
    feeds/Feeds.h \
    feeds/FeedStorage.h \
    Gender.h \
    id/ChatId.h \
    id/ObjectId.h \
    id/SecureRandom.h \
    interfaces/ITranslation.h \
    JSON.h \
    k8json/k8json.h \
    net/NetReply.h \
    net/NetRequest.h \
    net/PacketReader.h \
    net/packets/auth.h \
    net/packets/ChannelNotice.h \
    net/packets/FeedNotice.h \
    net/packets/MessageNotice.h \
    net/packets/Notice.h \
    net/PacketWriter.h \
    net/SimpleID.h \
    net/SimpleSocket.h \
    net/SimpleSocket_p.h \
    net/TransportReader.h \
    net/TransportWriter.h \
    Path.h \
    schat.h \
    Settings.h \
    sglobal.h \
    tools/OsInfo.h \
    tools/Ver.h \
    Tr.h \

SOURCES += \
    acl/Acl.cpp \
    acl/AclValue.cpp \
    base32/base32.cpp \
    Channel.cpp \
    DateTime.cpp \
    feeds/Feed.cpp \
    feeds/FeedEvent.cpp \
    feeds/FeedEvents.cpp \
    feeds/FeedHeader.cpp \
    feeds/Feeds.cpp \
    feeds/FeedStorage.cpp \
    Gender.cpp \
    id/ChatId.cpp \
    id/ObjectId.cpp \
    id/SecureRandom.cpp \
    JSON.cpp \
    k8json/k8json.cpp \
    net/NetReply.cpp \
    net/NetRequest.cpp \
    net/PacketReader.cpp \
    net/packets/auth.cpp \
    net/packets/ChannelNotice.cpp \
    net/packets/FeedNotice.cpp \
    net/packets/MessageNotice.cpp \
    net/packets/Notice.cpp \
    net/SimpleID.cpp \
    net/SimpleSocket.cpp \
    net/TransportReader.cpp \
    net/TransportWriter.cpp \
    Path.cpp \
    Settings.cpp \
    tools/OsInfo.cpp \
    tools/Ver.cpp \
    Tr.cpp \

win32 {
  SOURCES += Path_win.cpp tools/OsInfo_win.cpp
}

unix:!macx: {
  SOURCES += Path_unix.cpp tools/OsInfo_unix.cpp
}

macx: {
  SOURCES += Path_mac.cpp tools/OsInfo_mac.cpp
}

win32-msvc*:DEFINES += _CRT_RAND_S

DEFINES += SCHAT_NO_QDNS
