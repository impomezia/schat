/* $Id: ClientHooks.h 3664 2013-05-11 18:19:01Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2013 IMPOMEZIA <schat@impomezia.com>
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

#ifndef CLIENTHOOKS_H_
#define CLIENTHOOKS_H_

#include <QObject>

#include "Channel.h"
#include "net/packets/MessageNotice.h"

class ChannelInfo;
class ClientCmd;
class FeedNotice;

namespace Hooks
{

/*!
 * Хуки, связанные с обработкой сообщений.
 *
 * \sa ClientMessages.
 */
class SCHAT_EXPORT Messages : public QObject
{
  Q_OBJECT

public:
  Messages(QObject *parent = 0);
  static QString remove(const QString &cmd, const QString &msg);

  virtual bool command(const QByteArray &dest, const ClientCmd &cmd);
  virtual bool command(const QByteArray &dest, const QString &text, const QString &plain);
  virtual int read(MessagePacket packet);
  virtual void error(MessagePacket packet);
  virtual void sent(MessagePacket packet);
  virtual void unhandled(MessagePacket packet);
};


/*!
 * Хуки, связанные с обработкой каналов.
 */
class SCHAT_EXPORT Channels : public QObject
{
  Q_OBJECT

public:
  Channels(QObject *parent = 0);
  inline void add(Channels *hook)    { if (!m_hooks.contains(hook)) m_hooks.append(hook); }
  inline void remove(Channels *hook) { m_hooks.removeAll(hook); }

  virtual ClientChannel get(const QByteArray &id);
  virtual void add(ClientChannel channel, const ChannelInfo &info, const QVariantMap &json);

protected:
  QList<Channels*> m_hooks; ///< Хуки.
};


/*!
 * Хуки, связанные c работой клиента.
 */
class SCHAT_EXPORT Client : public QObject
{
  Q_OBJECT

public:
  Client(QObject *parent = 0);
  inline void add(Client *hook)    { if (!m_hooks.contains(hook)) m_hooks.append(hook); }
  inline void remove(Client *hook) { m_hooks.removeAll(hook); }

  virtual bool openId(const QByteArray &id, bool &matched);
  virtual QByteArray id() const;
  virtual QByteArray serverId() const;
  virtual void restore();
  virtual void setup();

protected:
  QList<Client*> m_hooks; ///< Хуки.
};


/*!
 * Хуки, связанные с обработкой фидов.
 */
class SCHAT_EXPORT Feeds : public QObject
{
  Q_OBJECT

public:
  Feeds(QObject *parent = 0);
  inline void add(Feeds *hook)    { if (!m_hooks.contains(hook)) m_hooks.append(hook); }
  inline void remove(Feeds *hook) { m_hooks.removeAll(hook); }

  void add(ClientChannel channel, const ChannelInfo &info, const QVariantMap &json);
  void readFeed(const FeedNotice &packet);

protected:
  virtual QStringList unsynced(ClientChannel channel, const QVariantMap &feeds, const QStringList &extra = QStringList());
  virtual void addImpl(ClientChannel channel, const ChannelInfo &info, const QVariantMap &json);
  virtual void readFeedImpl(const FeedNotice &packet);

  QList<Feeds*> m_hooks;            ///< Хуки.
  QMap<QByteArray, qint64> m_cache; ///< Блокирующий кэш препятствующий двойной загрузке фидов.
};

}  // namespace Hooks

#endif /* CLIENTHOOKS_H_ */
