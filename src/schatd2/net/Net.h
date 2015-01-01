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

#ifndef NET_H_
#define NET_H_

#include <QObject>

#include "ServerChannel.h"
#include "net/NetRecord.h"

class DataCreator;
class NetContext;
class NetReply;

class SCHAT_EXPORT Net : public QObject
{
  Q_OBJECT

public:
  Net(QObject *parent = 0);

  void add(DataCreator *creator);
  void pub(ChatChannel channel, const QString &path);
  void pub(ChatChannel channel, const QString &path, const NetRecord &record);
  void req(const NetContext &context, NetReply &reply);

signals:
  void subscriptionChanged(const QByteArray &id, const QString &path) const;

private:
  class Creators
  {
  public:
    Creators() {};
    ~Creators();
    inline DataCreator* get(const QString &path) const { return m_map.value(path); }
    void add(DataCreator *creator);

  private:
    QList<DataCreator*> m_list;
    QMap<QString, DataCreator*> m_map;
  };

  bool get(const NetContext &context, NetReply &reply) const;
  bool prepare(const NetContext &context, NetReply &reply);
  bool subscribe(const QString &path) const;
  int matchAcl(const QString &path) const;
  void broadcast(ChatChannel channel, const QString &path, const NetRecord &record);

  ChatChannel m_dest;                ///< Канал назначения.
  ChatChannel m_sender;              ///< Канал через который было доставлен запрос.
  ChatChannel m_user;                ///< Пользователь отправивший запрос.
  Creators m_creators;               ///< Обработчики для создания данных.
  QMap<ChatId, NetRecordMap> m_data; ///< Хранилище данных.
};

#endif /* NET_H_ */
