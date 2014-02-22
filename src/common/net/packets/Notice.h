/* $Id: Notice.h 3744 2013-07-10 20:58:58Z IMPOMEZIA $
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

#ifndef NOTICE_H_
#define NOTICE_H_

#include <QVariant>
#include <QSharedPointer>

#include "schat.h"

class PacketReader;
class PacketWriter;

/*!
 * Универсальное уведомление, содержит данные в JSON формате, текстовый тип, отметку времени и уникальный идентификатор.
 * Этот пакет является универсальным высокоуровневым транспортом чата, для расширений протокола.
 */
class SCHAT_EXPORT Notice
{
public:
  enum Type {
    GenericType = 103,  ///< 'g'
    ChannelType = 99,   ///< 'c'
    MessageType = 109,  ///< 'm'
    FeedType    = 102   ///< 'f'
  };

  /// Дополнительные поля данных.
  enum Fields {
    NoFields  = 0, ///< Нет дополнительных полей.
    IdField   = 1, ///< Содержит идентификатор сообщения \sa m_id.
    JSonField = 2, ///< Содержит JSON данные \sa m_data, m_raw.
    TextField = 4, ///< Содержит сырые текстовые данные \sa m_text.
    BlobField = 8  ///< Содержит бинарные данные. \sa m_blob.
  };

  ///< Коды состояния.
  enum StatusCodes {
    OK                  = 200, ///< OK.
    Found               = 300, ///< Found, сообщение из журнала, доставленное во время активного разговора.
    Unread              = 301, ///< Unread, не прочитанное офлайн сообщение.
    Read                = 302, ///< Read, прочитанное офлайн сообщение.
    NotModified         = 303, ///< Not Modified.
    BadRequest          = 400, ///< Bad Request.
    Unauthorized        = 401, ///< Unauthorized.
    Forbidden           = 402, ///< Forbidden.
    NotFound            = 403, ///< Not Found.
    ObjectAlreadyExists = 404, ///< Object Already Exists.
    ObjectNotExists     = 405, ///< Object Not Exists.
    NickAlreadyUse      = 406, ///< Nick Already In Use.
    ChannelOffline      = 407, ///< Channel Offline.
    Conflict            = 408, ///< Conflict.
    InternalError       = 500, ///< Internal Error.
    NotImplemented      = 501, ///< Not Implemented.
    BadGateway          = 502, ///< Bad Gateway.
    ServiceUnavailable  = 503, ///< Service Unavailable.
    GatewayTimeout      = 504, ///< Gateway Timeout.
    UserStatus          = 600, ///< Начало диапазона пользовательских статусных кодов.
  };

  /// Направление пакета.
  enum Direction {
    Client2Server = 115, ///< 's'
    Server2Client = 99,  ///< 'c'
    Client2Client = 67,  ///< 'C'
    Server2Server = 83,  ///< 'S'
    Internal      = 73   ///< 'I' Аналогично Client2Client, но не должно кешироватся. \since 1.99.32.
  };

  Notice(const QByteArray &sender, const QByteArray &dest, const QString &command, quint64 date = 0, const QByteArray &id = QByteArray(), const QVariantMap &data = QVariantMap());
  Notice(quint16 type, PacketReader *reader);
  virtual ~Notice() {}

  virtual bool isValid() const;

  inline const QByteArray& id() const      { return m_id; }
  inline const QByteArray& raw() const     { return m_raw; }
  inline const QByteArray& sender() const  { return m_sender; }
  inline const QString& command() const    { return m_command; }
  inline const QString& text() const       { return m_text; }
  inline const QVariantMap& json() const   { return m_data; }
  inline const QByteArray& blob() const    { return m_blob; }
  inline int direction() const             { return m_direction; }
  inline int fields() const                { return m_fields; }
  inline int status() const                { return m_status; }
  inline int type() const                  { return m_type; }
  inline int version() const               { return m_version; }
  inline QByteArray dest() const           { return m_dest; }
  inline qint64 date() const               { return m_date; }
  inline QVariantMap& json()               { return m_data; }
  QByteArray data(QDataStream *stream, bool echo = false) const;

  static QString status(int status);

  inline void setCommand(const QString &command)     { m_command = command; }
  inline void setData(const QVariantMap &data)       { m_data = data; }
  inline void setDate(qint64 date)                   { m_date = date; }
  inline void setDest(const QByteArray &dest)        { m_dest = dest; }
  inline void setDirection(int direction)            { m_direction = direction; }
  inline void setId(const QByteArray &id)            { m_id = id; }
  inline void setStatus(int status)                  { m_status = status; }
  inline void setText(const QString &text)           { m_text = text; }
  inline void setBlob(const QByteArray &blob)        { m_blob = blob; }

protected:
  virtual void write(PacketWriter *writer) const { Q_UNUSED(writer) }

  QByteArray m_sender;        ///< Идентификатор отправителя.
  QByteArray m_dest;          ///< Идентификаторы получателей.
  quint16 m_type;             ///< Тип пакета Notice::Type.
  mutable quint8 m_direction; ///< Направление пакета.
  mutable quint8 m_fields;    ///< Дополнительные поля данных.
  quint8 m_version;           ///< Версия пакета, обязательное поле.
  quint16 m_status;           ///< Статус \sa StatusCodes, обязательное поле.
  qint64 m_date;              ///< Отметка времени, обязательное поле.
  QByteArray m_id;            ///< Идентификатор сообщения, не обязательное поле.
  QString m_command;          ///< Текстовая команда, обязательное поле.
  QVariantMap m_data;         ///< JSON данные пакета, не обязательное поле.
  QByteArray m_raw;           ///< Сырые JSON данные.
  QString m_text;             ///< Сырой текст, не обязательное поле.
  QByteArray m_blob;          ///< Сырые бинарные данные, не обязательное поле.
};

typedef QSharedPointer<Notice> Packet;

#endif /* NOTICE_H_ */
