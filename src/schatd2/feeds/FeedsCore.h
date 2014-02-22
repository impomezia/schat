/* $Id: FeedsCore.h 3470 2013-02-06 09:57:57Z IMPOMEZIA $
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

#ifndef FEEDSCORE_H_
#define FEEDSCORE_H_

#include <QObject>
#include <QVariant>

#include "feeds/Feed.h"

class FeedEvent;
class FeedEvents;
class ServerChannel;

class SCHAT_EXPORT FeedsCore : public QObject
{
  Q_OBJECT

public:
  FeedsCore(QObject *parent = 0);

  static FeedReply post(const QString &name, const QVariant &value = QVariant(), int options = 0);
  static FeedReply post(const QString &name, ServerChannel *sender, const QVariant &value = QVariant(), int options = 0);
  static FeedReply post(ServerChannel *channel, const QString &name, ServerChannel *sender, const QVariant &value = QVariant(), int options = 0);

  static FeedReply put(const QString &name, const QVariant &value = QVariant(), int options = 0);
  static FeedReply put(const QString &name, ServerChannel *sender, const QVariant &value = QVariant(), int options = 0);
  static FeedReply put(ServerChannel *channel, const QString &name, ServerChannel *sender, const QVariant &value = QVariant(), int options = 0);

  static FeedReply del(const QString &name, int options = 0);
  static FeedReply del(const QString &name, ServerChannel *sender, int options = 0);
  static FeedReply del(ServerChannel *channel, const QString &name, ServerChannel *sender, int options = 0);

  static FeedReply get(const QString &name, const QVariantMap &data = QVariantMap());
  static FeedReply get(const QString &name, ServerChannel *sender, const QVariantMap &data = QVariantMap());
  static FeedReply get(ServerChannel *channel, const QString &name, ServerChannel *sender, const QVariantMap &data = QVariantMap());

  static FeedReply request(ServerChannel *channel, const QString &method, const QString &name, ServerChannel *sender, const QVariantMap &json = QVariantMap());
  static void sub(const QString &feedName);

private slots:
  void notify(const FeedEvent &event);

private:
  /// Числовое представление методов.
  enum Methods {
    Unknown,
    Get,
    Post,
    Put,
    Delete
  };

  static FeedReply done(FeedEvent *event, const FeedReply &reply);
  static int methodToInt(const QString &method);
  void broadcast(const FeedEvent &event);

  FeedEvents *m_events;                             ///< События фидов.
  QMap<QString, QList<QByteArray> > m_subscription; ///< Таблица подписки на фиды сервера.
  static FeedsCore *m_self;                         ///< Указатель на себя.
};

#endif /* FEEDSCORE_H_ */
