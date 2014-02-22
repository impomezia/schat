/* $Id: ChatId.h 3728 2013-07-05 22:24:27Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2013 Alexander Sedov <support@schat.me>
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

#ifndef CHATID_H_
#define CHATID_H_

#include <string.h>

#include <QString>

#include "ObjectId.h"
#include "schat.h"

#if QT_VERSION >= 0x050000
class QUuid;
#else
struct QUuid;
#endif

#pragma pack(1)
class SCHAT_EXPORT ChatId
{
public:
  /// Значения по умолчанию.
  enum {
    kIdSize       = 21, ///< Размер идентификатора в байтах.
    kEncodedSize  = 34  ///< Размер идентификатора кодированного в Base32.
  };

  /// Типы идентификаторов.
  enum Type {
    InvalidId    = 0,      ///<     0x00 Неверный идентификатор.
    UniqueUserId = 85,     ///< 'U' Уникальный идентификатор пользователя, использующийся при авторизации.
    UserId       = 117,    ///< 'u' Стандартный идентификатор пользователя.
    ServerId     = 83,     ///< 'S' Идентификатор сервера.
    CookieId     = 67,     ///< 'C' Идентификатор Cookie.
    ChannelId    = 99,     ///< 'c' Идентификатор канала.
    MessageId    = 109,    ///< 'm' 0x6D Идентификатор сообщения.
    PasswordId   = 112,    ///< 'p' Идентификатор пароля.
    NormalizedId = 110,    ///< 'n' Идентификатор нормализированного имени канала.
    HostId       = 72      ///< 'H' Идентификатор хоста.
  };

  inline ChatId() : a(0), b(0), c(0), d(0)                        {}
  inline ChatId(const QByteArray &data, Type type)                { init(data, type); }
  inline ChatId(const QByteArray &id)                             { init(id); }
  inline ChatId(const QString &id)                                { init(id.toLatin1()); }
  inline ChatId(Type type, const QByteArray &salt = QByteArray()) { init(type, salt); }
  inline ChatId(const ObjectId &oid, qint64 modified = 0, const uchar type = MessageId ) { init(oid, modified, type); }

  ChatId &init(const ObjectId &oid, qint64 modified = 0, const uchar type = MessageId);
  ChatId &init(const QByteArray &data, Type type);
  ChatId &init(const QByteArray &id);
  ChatId &init(Type type, const QByteArray &salt = QByteArray());

  inline bool isNull() const                { return !a && !b && !c && !d; }
  inline const QByteArray byteArray() const { return QByteArray::fromRawData(reinterpret_cast<const char *>(m_data), kIdSize); }
  inline const uchar *data() const          { return m_data; }
  inline ObjectId oid() const               { return ObjectId(m_oid); }
  inline QByteArray toByteArray() const     { return QByteArray(reinterpret_cast<const char *>(m_data), kIdSize); }
  inline QString toString() const           { return toBase32(); }
  inline uchar type() const                 { return m_type; }
  inline void clear()                       { a = 0; b = 0; c = 0; d = 0; }
  inline void setDate(qint64 date)          { m_date = date; }

  inline bool operator!=(const ChatId& other) const { return a != other.a || b != other.b || c != other.c || d != other.d; }
  inline bool operator<(const ChatId& other) const  { return memcmp(m_data , other.m_data , kIdSize ) < 0; }
  inline bool operator==(const ChatId& other) const { return a == other.a && b == other.b && c == other.c && d == other.d; }

  inline qint64 date() const {
    qint64 date = m_date;
    ((short *) &date)[3] = 0; // 2 старших байта, зарезервированы и обнуляются при преобразовании в дату.
    return date;
  }

  bool hasOid() const;
  QByteArray toBase32() const;

  static QByteArray fromBase32(const QByteArray &base32);
  static QByteArray toBase32(const QByteArray &data);

private:
# if QT_VERSION < 0x040800
  QByteArray toRfc4122(const QUuid uuid) const;
# endif

  union {
    struct {
      uchar m_oid[ObjectId::kOIDSize];
      qint64 m_date;
      uchar m_type;
    };

    struct {
      quint64 a;
      quint32 b;
      quint64 c;
      uchar d;
    };

    uchar m_data[kIdSize];
  };
};
#pragma pack()

#endif /* CHATID_H_ */
