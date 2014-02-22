/* $Id: Feed.h 3750 2013-07-13 00:04:11Z IMPOMEZIA $
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

#ifndef FEED_H_
#define FEED_H_

#include <QSharedPointer>
#include <QVariant>

#include "feeds/FeedHeader.h"
#include "feeds/FeedStrings.h"

class FeedReply
{
public:
  FeedReply(int status, qint64 date = 0)
  : status(status)
  , date(date)
  {}

  FeedReply(int status, const QVariantMap &json, qint64 date = 0)
  : status(status)
  , date(date)
  , json(json)
  {}

  int status;                ///< Статус операции.
  qint64 date;               ///< Дата модификации, если 0 будет определена автоматически.
  QList<QByteArray> packets; ///< Дополнительные пакеты с ответом на запрос.
  QVariantMap json;          ///< JSON ответ на запрос.
  QByteArray blob;           ///< Дополнительные бинарные данные.
};


/*!
 * Базовый класс для фидов.
 */
class SCHAT_EXPORT Feed
{
public:
  /// Опции модификации тела фида.
  enum Options {
    NoOptions = 0, ///< Нет специальных опций.
    Echo      = 1, ///< Необходимо отослать обратно установленное значение.
    Share     = 2, ///< Необходимо отослать ответ всем подключениям этого клиента.
    Broadcast = 4, ///< Отослать информацию о том что канал изменился всем подписчикам.
    ShareAll  = 8  ///< Тоже самое что и Share, но ответ будет разослан всем подписчикам если запрос успешный. Требует Share. \since 2.1.0.
  };

  Feed(const QString &name, const QVariantMap &data);
  Feed(const QString &name = QString(), qint64 date = 0);
  virtual ~Feed() {}

  virtual bool isValid() const;
  virtual FeedReply del(const QString &path, Channel *channel = 0, const QByteArray &blob = QByteArray());
  virtual FeedReply get(const QString &path, const QVariantMap &json = QVariantMap(), Channel *channel = 0, const QByteArray &blob = QByteArray()) const;
  virtual FeedReply post(const QString &path, const QVariantMap &json, Channel *channel = 0, const QByteArray &blob = QByteArray());
  virtual FeedReply put(const QString &path, const QVariantMap &json, Channel *channel = 0, const QByteArray &blob = QByteArray());
  virtual QVariantMap feed(Channel *channel = 0) const;
  virtual QVariantMap save() const;

  virtual bool can(Channel *channel, int acl) const;
  virtual void setChannel(Channel *channel);

  inline const FeedHeader& head() const    { return m_header; }
  inline const QVariantMap& data() const   { return m_data; }
  inline FeedHeader& head()                { return m_header; }
  inline QVariantMap& data()               { return m_data; }

  static bool merge(const QString &key, QVariantMap &out, const QVariantMap &in);
  static QVariantMap merge(const QString &key, const QVariantMap &in);
  static void merge(QVariantMap &out, const QVariantMap &in);

protected:
  FeedHeader m_header;  ///< Заголовок фида.
  QVariantMap m_data;   ///< JSON данные фида.
};

typedef QSharedPointer<Feed> FeedPtr;

#endif /* FEED_H_ */
