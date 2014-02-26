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

#ifndef CLIENTFEEDS_H_
#define CLIENTFEEDS_H_

#include <QObject>
#include <QVariant>

#include "Channel.h"
#include "net/NetRequest.h"

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
  static bool req(const NetRequest &request);
  static bool request(ClientChannel channel, const QString &command, const QString &name, const QVariantMap &json = QVariantMap());
  static bool request(const QByteArray &id, const QString &command, const QString &name, const QVariantMap &json = QVariantMap());
  static int match(ClientChannel channel, ClientChannel user = ClientChannel());

private slots:
  void json();
  void notice(int type);

private:
  FeedNotice *m_packet;            ///< Текущий прочитанный пакет.
  Hooks::Feeds *m_hooks;           ///< Хуки.
  QMap<QString, NetRequest> m_req; ///< Отправленные запросы.
  static ClientFeeds *m_self;      ///< Указатель на себя.
};

#endif /* CLIENTFEEDS_H_ */
