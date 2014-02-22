/* $Id: NodeChannelIndex.h 3616 2013-04-03 19:57:26Z IMPOMEZIA $
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

#ifndef NODECHANNELINDEX_H_
#define NODECHANNELINDEX_H_

#include <QObject>

#include <ServerChannel.h>

class FeedEvent;
class NotifyItem;

/*!
 * Информация о канале в списке каналов.
 */
class ChannelIndexData
{
public:
  /// Опции.
  enum Options {
    NoOptions = 0, ///< Нет опций.
    Permanent = 1, ///< Постоянный канал.
    Pinned    = 2, ///< Закреплённый канал.
    Private   = 4  ///< Приватный канал.
  };

  ChannelIndexData(ChatChannel channel);
  bool isValid() const;
  bool operator<(const ChannelIndexData &other) const;
  QVariantList toList() const;

  int count;         ///< Число пользователей в канале.
  int options;       ///< Опции.
  int visibility;    ///< Видимость канала.
  QByteArray id;     ///< Идентификатор канала.
  QString name;      ///< Имя канала.
  QString title;     ///< Заголовок канала.
};


/*!
 * Формирует сортированный список каналов доступных на сервере.
 */
class NodeChannelIndex : public QObject
{
  Q_OBJECT

public:
  NodeChannelIndex(QObject *parent = 0);
  inline const QList<ChannelIndexData> &list() const { return m_list; }
  inline qint64 date() const                         { return m_date; }

private slots:
  void build();
  void notify(const FeedEvent &event);

private:
  bool isDirty(const FeedEvent &event) const;
  QList<ChatChannel> channels() const;
  void reload();

  bool m_updated;                 ///< \b true если обновлён список каналов.
  qint64 m_date;                  ///< Дата формирования списка.
  QList<ChannelIndexData> m_list; ///< Список каналов.
};

#endif /* NODECHANNELINDEX_H_ */
