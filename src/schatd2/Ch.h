/* Simple Chat
 * Copyright (c) 2008-2015 Alexander Sedov <imp@schat.me>
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

#ifndef CH_H_
#define CH_H_

#include <QObject>

#include "net/SimpleID.h"
#include "ServerChannel.h"

class AuthRequest;
class ChHook;

class SCHAT_EXPORT Ch : public QObject
{
  Q_OBJECT

public:
  Ch(QObject *parent = 0);
  ~Ch();
  inline static void add(ChHook *hook)           { if (!m_self) return; if (!m_self->m_hooks.contains(hook)) m_self->m_hooks.append(hook); }
  inline static void remove(ChHook *hook)        { if (!m_self) return; m_self->m_hooks.removeAll(hook); }
  static bool add(ChatChannel channel);
  static bool gc(ChatChannel channel);
  static ChatChannel channel(const QByteArray &id, int type = SimpleID::ChannelId, bool db = true);
  static ChatChannel channel(const QString &name, ChatChannel user = ChatChannel());
  static ChatChannel server();
  static ChatChannel user(const QString &nativeId);
  static int rename(ChatChannel channel, const QString &name);
  static QByteArray cookie();
  static QByteArray makeId(const QByteArray &normalized);
  static void load();
  static void userChannel(ChatChannel channel, const AuthRequest &data, const SJMPPacket &packet, const QString &ip, const QString &uuid, quint64 socket = 0);

  // Служебные функции.
  static void addNewFeedIfNotExist(ChatChannel channel, const QString &name, ChatChannel user = ChatChannel());
  static void addNewUserFeedIfNotExist(ChatChannel channel, const QString &name);

private:
  ChatChannel channelImpl(const QByteArray &id, int type = SimpleID::ChannelId, bool db = true);
  ChatChannel channelImpl(const QString &name, ChatChannel user);
  void cache(ChatChannel channel);
  void remove(ChatChannel channel);
  void remove(const QByteArray &id);
  void setOnline(ChatChannel channel);
  void sync(ChatChannel channel, ChatChannel user = ChatChannel());

  QList<ChHook *> m_hooks;                   ///< Хуки.
  QMap<QByteArray, ChatChannel> m_channels;  ///< Кеш каналов, ключ в таблице идентификатор канала и нормализированный идентификатор имени и cookie для пользовательских каналов.
  static Ch *m_self;                         ///< Указатель на себя.
};

#endif /* CH_H_ */
