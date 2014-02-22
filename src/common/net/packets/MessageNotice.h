/* $Id: MessageNotice.h 3744 2013-07-10 20:58:58Z IMPOMEZIA $
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

#ifndef MESSAGENOTICE_H_
#define MESSAGENOTICE_H_

#include <QStringList>

#include "id/ChatId.h"
#include "net/packets/Notice.h"

struct SCHAT_EXPORT MessageRecord
{
  MessageRecord()
  : id(0)
  , status(200)
  , date(0)
  {}

  qint64 id;
  QByteArray messageId;
  QByteArray senderId;
  QByteArray destId;
  qint64 status;
  qint64 date;
  QString command;
  QString text;
  QString plain;
  QByteArray data;
};


/*!
 * Информация о сообщении.
 *
 * \since 2.1.0.
 */
struct SCHAT_EXPORT MessageRecordV2
{
  MessageRecordV2()
  : id(0)
  , status(200)
  , date(0)
  {}

  qint64 id;
  ChatId oid;      ///< Идентификатор сообщения.
  ChatId sender;   ///< Идентификатор отправителя.
  ChatId dest;     ///< Идентификатор получателя.
  qint64 status;   ///< Статус сообщения.
  qint64 date;     ///< Дата создания сообщения.
  qint64 mdate;    ///< Дата модификации сообщения, может быть 0 если модификация не поддерживается.
  QString cmd;     ///< Команда сообщения.
  QString text;    ///< Текст сообщения.
  QByteArray data; ///< Дополнительные опциональные JSON данные сообщения.
  QByteArray blob; ///< Дополнительные опциональные бинарные данные сообщения.
};


class SCHAT_EXPORT MessageNotice : public Notice
{
public:
  enum XStatusCodes {
    Removed = 600
  };

  MessageNotice();
  MessageNotice(const MessageRecord &record, bool parse = false);
  MessageNotice(const MessageRecordV2 &record, bool parse = false);
  MessageNotice(const QByteArray &sender, const QByteArray &dest, const QString &text, quint64 date = 0, const QByteArray &id = QByteArray());
  MessageNotice(quint16 type, PacketReader *reader);
  inline const QByteArray& internalId() const     { return m_internalId; }
  inline void setInternalId(const QByteArray &id) { m_internalId = id; }
  QByteArray toId() const;
  static QByteArray toTag(const QStringList &messages);
  static QList<QByteArray> decode(const QStringList &ids);
  static QStringList encode(const QList<ChatId> &ids);
  static QStringList encode(const QList<QByteArray> &ids);

  ObjectId oid;
  qint64 mdate;

private:
  QByteArray m_internalId; ///< Внутренний идентификатор сообщения.
};

typedef QSharedPointer<MessageNotice> MessagePacket;

#endif /* MESSAGENOTICE_H_ */
