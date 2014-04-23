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

#ifndef NODEMESSAGES_H_
#define NODEMESSAGES_H_

#include "NodeNoticeReader.h"

class FeedEvent;
class MessageNotice;

class NodeMessages : public NodeNoticeReader
{
public:
  /// Версия сообщений.
  enum Version {
    V1, /// Первоначальная версия, не поддерживает редактирование или удаление сообщений, поддерживается для обеспечения обратной совместимости.
    V2  /// \since 2.1.0 Новая версия.
  };

  NodeMessages(Core *core);

protected:
  bool read(PacketReader *reader) Q_DECL_OVERRIDE;

private:
  bool cleanup();
  FeedEvent *createEvent();
  int filter();
  void pong(qint64 date);
  void reject(int status);

  ChatChannel m_dest;      ///< Канал получателя.
  ChatChannel m_sender;    ///< Канал отправителя.
  ChatId m_id;             ///< Идентификатор сообщения.
  int m_version;           ///< Версия для определения схемы обработки сообщений.
  MessageNotice *m_packet; ///< Прочитанный пакет.
};

#endif /* NODEMESSAGES_H_ */
