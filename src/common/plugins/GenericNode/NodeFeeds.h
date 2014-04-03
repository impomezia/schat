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

#ifndef NODEFEEDS_H_
#define NODEFEEDS_H_

#include "NodeNoticeReader.h"

class FeedEvent;
class FeedNotice;
class ServerChannel;

class NodeFeeds : public NodeNoticeReader
{
public:
  NodeFeeds(Core *core);

protected:
  bool read(PacketReader *reader);

private:
  class CheckResult
  {
  public:
    CheckResult(const QString &text);

    FeedPtr feed; ///< Фид.
    int status;   ///< Статус.
    QString name; ///< Имя фида.
    QString path; ///< Путь запроса.
  };

private:
  FeedReply add();
  FeedReply del(const CheckResult &result);
  FeedReply post(CheckResult &result);
  int get();
  int get(FeedPtr feed, const QString &request);
  int headers();
  int query(const QString &verb);

  CheckResult check(int acl);
  void broadcast(FeedPtr feed, bool echo = true);
  void reply(int status);

  ChatChannel m_channel; ///< Канал фида.
  ChatChannel m_user;    ///< Отправитель пакета.
  FeedEvent *m_event;    ///< Текущее событие.
  FeedNotice *m_packet;  ///< Текущий пакет.
};

#endif /* NODEFEEDS_H_ */
