/*   Copyright (C) 2013-2014 Alexander Sedov <imp@schat.me>
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

#ifndef SJMPPACKET_H_
#define SJMPPACKET_H_

#include <QVariant>
#include <QDebug>

#include "schat.h"

class SCHAT_EXPORT SJMPPacket
{
public:
  enum Type {
    Unknown  = 0,
    Request  = '<',
    Reply    = '>',
    Modified = '=',
    Added    = '+',
    Deleted  = '-'
  };

  SJMPPacket();
  SJMPPacket(const QByteArray &packet);
  bool deserialize(const QByteArray &packet);
  inline const QString &id() const                                        { return m_id; }
  inline const QString &method() const                                    { return m_method; }
  inline const QString &resource() const                                  { return m_resource; }
  inline const QVariant &body() const                                     { return m_body; }
  inline const QVariantMap &headers() const                               { return m_headers; }
  inline int status() const                                               { return m_status; }
  inline qint64 date() const                                              { return m_date; }
  inline QVariant header(const QString &key) const                        { return m_headers.value(key); }
  inline SJMPPacket &removeHeader(const QString &key)                     { m_headers.remove(key); return *this; }
  inline SJMPPacket &setBody(const QVariant &body)                        { m_body = body; return *this; }
  inline SJMPPacket &setDate(qint64 date)                                 { m_date = date; return *this; }
  inline SJMPPacket &setHeader(const QString &key, const QVariant &value) { m_headers.insert(key, value); return *this; }
  inline SJMPPacket &setHeaders(const QVariantMap &headers)               { m_headers = headers; return *this; }
  inline SJMPPacket &setId(const QString &id)                             { m_id = id; return *this; }
  inline SJMPPacket &setMethod(const QString &method)                     { m_method = method.toLower(); return *this; }
  inline SJMPPacket &setResource(const QString& resource)                 { m_resource = resource; return *this; }
  inline SJMPPacket &setStatus(int status)                                { m_status = status; return *this; }
  inline SJMPPacket &setType(Type type)                                   { m_type = type; return *this; }
  inline Type type() const                                                { return m_type; }
  QByteArray serialize() const;
  SJMPPacket &setDate(const QDateTime &dateTime);

private:
  Type m_type;
  QString m_method;
  int m_version;
  QString m_resource;
  int m_status;
  mutable QString m_id;
  qint64 m_date;
  QVariantMap m_headers;
  QVariant m_body;
};

QDebug SCHAT_EXPORT operator<<(QDebug dbg, const SJMPPacket &packet);

#endif // SJMPPACKET_H_
