/* Simple Chat
 * Copyright (c) 2008-2014 Alexander Sedov <imp@schat.me>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NODECHANNELS_H_
#define NODECHANNELS_H_

#include "NodeNoticeReader.h"
#include "net/Channels.h"
#include "net/packets/ChannelNotice.h"

class ChannelNotice;
class Core;
class FeedEvent;
class Storage;

class NodeChannels : public QObject, NodeNoticeReader
{
  Q_OBJECT

public:
  NodeChannels(Core *core);

protected:
  bool read(PacketReader *reader) Q_DECL_OVERRIDE;
  void acceptImpl(ChatChannel user, const AuthResult & /*result*/, QList<QByteArray> &packets) Q_DECL_OVERRIDE;
  void addImpl(ChatChannel user) Q_DECL_OVERRIDE;
  void releaseImpl(ChatChannel user, quint64 socket) Q_DECL_OVERRIDE;

private slots:
  void join();
  void notify(const FeedEvent &event);

private:
  bool info();
  bool join(const ChatId &channelId, const QString &name = QString());
  int name();
  bool part();
  bool quit();
  int update();

  bool isForbidden(ChatChannel channel) const;
  ChannelPacket reply(ChatChannel channel, bool forbidden = false, const QString &command = CHANNELS_CHANNEL_CMD, const QString &xName = QString()) const;
  void dump() const;
  void reply(int status);

  ChannelNotice *m_packet;      ///< Текущий пакет.
  ChatChannel m_user;           ///< Текущий пользователь.
  QList<ChatChannel> m_pending; ///< Список для отложенного входа в обычные каналы.
};

#endif /* NODECHANNELS_H_ */
