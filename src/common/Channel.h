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

#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <QList>
#include <QSharedPointer>
#include <QVariant>

#include "feeds/Feeds.h"
#include "Gender.h"
#include "id/ChatId.h"

class Account;

/*!
 * Базовый статус пользователя.
 */
class Status
{
public:
  /// Статус.
  enum Statuses {
    Offline,
    Online,
    Away,
    AutoAway,
    DnD,
    FreeForChat
  };

  Status(quint8 status = Offline)
  : m_status(status)
  {}

  inline quint8 value() const    { return m_status; }
  inline void set(quint8 status) { m_status = status; }

  inline bool operator!=(int other) const { return m_status != other; }
  inline bool operator==(int other) const { return m_status == other; }
  inline Status& operator=(int other)     { m_status = other; return *this; }

private:
  quint8 m_status;
};


/*!
 * Список каналов.
 */
class SCHAT_EXPORT Channels
{
public:
  Channels() {}

  bool add(const QByteArray &id);
  inline bool contains(const QByteArray &id) const   { return m_channels.contains(id); }
  inline const QList<QByteArray>& all() const        { return m_channels; }
  inline int size() const                            { return m_channels.size(); }
  inline void clear()                                { m_channels.clear(); }
  inline void remove(const QByteArray &id)           { m_channels.removeAll(id); }
  inline void set(const QList<QByteArray> &channels) { m_channels = channels; }
  QByteArray join() const;
  QList<QByteArray> all(int type) const;
  void restore(const QByteArray &channels);

  Channels& operator+=(const QByteArray &id);
  Channels& operator=(const QList<QByteArray> &channels);

private:
  QList<QByteArray> m_channels; ///< Список идентификаторов каналов-подписчиков.
};


/*!
 * Базовый класс канала.
 */
class SCHAT_EXPORT Channel
{
public:
  /// Ограничения.
  enum Limits {
    MinNameLength = 3,    ///< Минимальная длина имени.
    MaxNameLength = 64,   ///< Максимальная длина имени.
  };

  Channel();
  Channel(const QByteArray &id, const QString &name);
  virtual ~Channel();
  virtual bool isValid() const;

  inline bool isSynced() const            { return m_synced; }
  inline const QByteArray& id() const     { return m_id; }
  inline const QString& name() const      { return m_name; }
  inline const QVariantMap& data() const  { return m_data; }
  inline int type() const                 { return m_type; }
  inline qint64 date() const              { return m_date; }
  inline qint64 key() const               { return m_key; }
  inline QVariantMap& data()              { return m_data; }

  bool setId(const QByteArray &id);
  bool setName(const QString &name);
  inline virtual void setKey(qint64 key)       { m_key = key; }
  inline void setData(const QVariantMap &data) { m_data = data; }
  inline void setSynced(bool synced)           { m_synced = synced; }
  void setDate(qint64 date = 0);

  inline Channels& channels()             { return m_channels; }
  inline const Channels& channels() const { return m_channels; }
  inline const Feeds& feeds() const       { return m_feeds; }
  inline const Gender& gender() const     { return m_gender; }
  inline const Status& status() const     { return m_status; }
  inline Feeds& feeds()                   { return m_feeds; }
  inline Gender& gender()                 { return m_gender; }
  inline Status& status()                 { return m_status; }

  static bool isValidName(const QString &name);
  static int isCompatibleId(const QByteArray &id);

  FeedPtr feed(const QString &name, bool create = true, bool save = true);

protected:
  Channels m_channels;             ///< Список каналов.

private:
  bool m_synced;                   ///< true если канал синхронизирован.
  Feeds m_feeds;                   ///< Таблица фидов.
  Gender m_gender;                 ///< Пол и цвет иконки.
  int m_type;                      ///< Тип канала, соответствует типу идентификатора канала \sa SimpleID::Types.
  QByteArray m_id;                 ///< Уникальный идентификатор канала.
  qint64 m_date;                   ///< Дата модификации.
  qint64 m_key;                    ///< Ключ в таблице базы данных.
  QString m_name;                  ///< Имя канала.
  QVariantMap m_data;              ///< JSON данные канала.
  Status m_status;                 ///< Статус пользователя.
};

typedef QSharedPointer<Channel> ClientChannel;

#endif /* CHANNEL_H_ */
