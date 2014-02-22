/* $Id: ClientFeeds.h 3590 2013-03-23 21:46:19Z IMPOMEZIA $
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

#ifndef CLIENTFEEDS_H_
#define CLIENTFEEDS_H_

#include <QObject>
#include <QVariant>

#include "Channel.h"

class FeedNotice;

namespace Hooks
{
  class Feeds;
}

class SCHAT_EXPORT ClientFeeds : public QObject
{
  Q_OBJECT

public:
  ClientFeeds(QObject *parent = 0);
  inline Hooks::Feeds *hooks() const { return m_hooks; }
  static bool del(const QByteArray &id, const QString &name, int options = 0);
  static bool get(const QByteArray &id, const QString &name, const QVariant &value = QVariant());
  static bool post(const QByteArray &id, const QString &name, const QVariant &value = QVariant(), int options = 0);
  static bool put(const QByteArray &id, const QString &name, const QVariant &value = QVariant(), int options = 0);
  static bool request(ClientChannel channel, const QString &command, const QString &name, const QVariantMap &json = QVariantMap());
  static bool request(const QByteArray &id, const QString &command, const QString &name, const QVariantMap &json = QVariantMap());
  static int match(ClientChannel channel, ClientChannel user = ClientChannel());

private slots:
  void notice(int type);

private:
  FeedNotice *m_packet;     ///< Текущий прочитанный пакет.
  Hooks::Feeds *m_hooks;    ///< Хуки.
};

#endif /* CLIENTFEEDS_H_ */
