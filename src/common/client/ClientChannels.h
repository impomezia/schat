/* $Id: ClientChannels.h 3724 2013-07-02 23:42:52Z IMPOMEZIA $
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

#ifndef CLIENTCHANNELS_H_
#define CLIENTCHANNELS_H_

#include <QObject>

#include "Channel.h"

class ChannelNotice;
class SimpleClient;

namespace Hooks
{
  class Channels;
}


class ChannelInfo
{
public:
  /// Информация об создании канала.
  enum Option {
    New,      ///< Канал создан в результате чтения пакета и ранее был не известен клиенту.
    Updated,  ///< Канал существовал ранее и был обновлён.
    Renamed,  ///< Канал существовал ранее и был обновлён и переименован.
    Forbidden ///< Доступ в канал запрещён.
  };

  ChannelInfo(const QByteArray &id)
  : m_option(New)
  , m_id(id)
  {}

  inline const QByteArray& id() const { return m_id; }
  inline int option() const           { return m_option; }
  inline void setOption(int option)   { m_option = option; }

private:
  int m_option;
  QByteArray m_id;
};


class SCHAT_EXPORT ClientChannels : public QObject
{
  Q_OBJECT

public:
  ClientChannels(QObject *parent = 0);
  inline const QByteArray& mainId() const                         { return m_mainId; }
  inline const QList<QByteArray>& joined() const                  { return m_joined; }
  inline const QMap<QByteArray, ClientChannel>& channels() const  { return m_channels; }
  inline Hooks::Channels *hooks() const                           { return m_hooks; }
  inline int policy() const                                       { return m_policy; }

  bool join(const QByteArray &id);
  bool join(const QString &name);
  bool name(const QByteArray &id, const QString &name);
  bool nick(const QString &nick);
  bool part(const QByteArray &id);
  bool sync(const QList<QByteArray> &channels);
  bool update();
  ClientChannel get(const QByteArray &id);

signals:
  void channel(const ChannelInfo &info);                          ///< Общая информация о канале.
  void channel(const QByteArray &id);                             ///< Команда "channel".
  void channels(const QList<QByteArray> &channels);               ///< Пакет новых каналов.
  void joined(const QByteArray &channel, const QByteArray &user); ///< Команда "+".
  void notice(const ChannelNotice &notice);                       ///< Сырой пакет.
  void part(const QByteArray &channel, const QByteArray &user);   ///< Команда "-".
  void quit(const QByteArray &user, bool offline = false);        ///< Команда "quit".

private slots:
  void clientStateChanged(int state, int previousState);
  void idle();
  void notice(int type);
  void restore();
  void setup();

private:
  bool info(const QList<QByteArray> &channels);
  ClientChannel add();
  void channel();
  void join();
  void part();
  void quit();
  void sync(ClientChannel channel);

  ChannelNotice *m_packet;                     ///< Текущий прочитанный пакет.
  Hooks::Channels *m_hooks;                    ///< Хуки.
  int m_policy;                                ///< Политика основного канала. \sa ServerFeed::Policy.
  QByteArray m_mainId;                         ///< Идентификатор главного канала.
  QList<QByteArray> m_joined;                  ///< Список каналов в которых находится клиент.
  QList<QByteArray> m_synced;                  ///< Список синхронизированных каналов.
  QList<QByteArray> m_unsynced;                ///< Список не синхронизированных каналов.
  QMap<QByteArray, ClientChannel> m_channels;  ///< Таблица каналов.
  SimpleClient *m_client;                      ///< Клиент чата.
};

#endif /* CLIENTCHANNELS_H_ */
