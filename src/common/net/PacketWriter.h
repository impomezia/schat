/* $Id: PacketWriter.h 3703 2013-06-22 18:50:02Z IMPOMEZIA $
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

#ifndef PACKETWRITER_H_
#define PACKETWRITER_H_

#include <QByteArray>
#include <QDataStream>

#include "id/ChatId.h"
#include "JSON.h"
#include "net/Protocol.h"

/*!
 * Класс выполняющий запись виртуального пакета.
 */
class PacketWriter
{
public:
  /*!
   * Создание виртуального пакета и запись заголовка.
   *
   * \param stream   output stream.
   * \param type     Тип пакета.
   */
  inline PacketWriter(QDataStream *stream, quint16 type)
    : m_stream(stream)
    , m_option(Protocol::BasicHeader)
  {
    m_device = stream->device();
    m_device->seek(0);

    *stream << type << quint8(0) << m_option;
  }

  /*!
   * Создание виртуального пакета и запись заголовка.
   *
   * \param stream   Поток записи.
   * \param type     Тип пакета.
   * \param sender   Идентификатор отправителя.
   * \param dest     Идентификатор получателя.
   * \param channel  Идентификатор канала.
   * \param echo     true для включения опции Protocol::EnableEcho.
   */
  inline PacketWriter(QDataStream *stream, quint16 type, const ChatId &sender, const ChatId &dest = ChatId(), bool echo = false, const ChatId &channel = ChatId())
    : m_stream(stream)
    , m_option(Protocol::BasicHeader)
  {
    m_device = stream->device();
    m_device->seek(0);

    if (!sender.isNull())
      m_option |= Protocol::SenderField;

    if (!dest.isNull())
      m_option |= Protocol::DestinationField;

    if (!channel.isNull())
      m_option |= Protocol::ChannelField;

    if (echo)
      m_option |= Protocol::EnableEcho;

    *stream << type << quint8(0) << m_option;

    if (m_option & Protocol::SenderField)
      put(sender);

    if (m_option & Protocol::ChannelField)
      put(channel);

    if (m_option & Protocol::DestinationField)
      put(QList<ChatId>() << dest);
  }

  /*!
   * Возвращает результат работы класса, тело пакета.
   */
  inline QByteArray data() const {
    const int size = m_device->pos();
    m_device->seek(0);
    return m_device->peek(size);
  }

  inline void put(const ChatId &id)        { m_device->write(id.byteArray()); }
  inline void put(const QString &text)     { *m_stream << text.toUtf8(); }
  inline void put(const QVariant &data)    { *m_stream << JSON::generate(data); }
  inline void put(const QVariantMap &data) { *m_stream << JSON::generate(data); }

  inline void put(const QList<ChatId> &ids) {
    put<quint32>(ids.size());
    for (int i = 0; i < ids.size(); ++i)
      put(ids.at(i));
  }

  template<class T>
  inline void put(const T &data) {
    *m_stream << data;
  }

  /// Запись Id.
  /// \deprecated Запись идентификатора в виде QByteArray является устаревшая и должна быть заменена на ChatId.
  inline void putId(const QByteArray &data)
  {
    put(ChatId(data));
  }

  /// Запись Id.
  /// \deprecated Запись идентификатора в виде QByteArray является устаревшая и должна быть заменена на ChatId.
  inline void putId(const QList<QByteArray> &data) {
    put<quint32>(data.size());
    for (int i = 0; i < data.size(); ++i)
      put(ChatId(data.at(i)));
  }

  inline bool is(int option) { return m_option & option; }

protected:
  QDataStream *m_stream; ///< output stream.
  QIODevice *m_device;   ///< output device.
  quint16 m_option;      ///< Опция заголовка.
};

#endif /* PACKETWRITER_H_ */
