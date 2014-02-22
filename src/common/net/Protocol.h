/* $Id: Protocol.h 3760 2013-07-25 03:59:53Z IMPOMEZIA $
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

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

class Protocol
{
public:
  /// Protocol version.
  enum Version {
    V3   = 0x3,  ///< Version 3 (obsolete).
    V4   = 0x4,  ///< Version 4.
    V4_0 = 0x0,  ///< Version 4.0.
    V4_1 = 0x1   ///< Version 4.1. \since 2.1.0.
  };

  /// Значения по умолчанию.
  enum Defaults {
    DefaultPort          = 7667,    ///< Порт по умолчанию.
    MaxProtocolVersion   = 0x0,     ///< Максимальная поддерживаемая версия протколола.
    MaxAuthPacketVersion = 0x1,     ///< Максимальная версия авторизационного пакета.
    ConnectTime          = 20000,   ///< Время ожидания перехода в состояние QAbstractSocket::ConnectedState для клиента.
    HandshakeTime        = 20000,   ///< Время для авторизации.
    IdleTime             = 20000,   ///< Время простоя после, которого клиент пошлёт пакет для проверки соединения.
    ReplyTime            = 15000,   ///< Время в течении которого будет ожидаться подтверждение доставки пакета.
    MaxServerIdleTime    = 40000,   ///< Время по истечении, которого сервер разорвёт соединение если не происходит обмена данных.
    FastReconnectTime    = 1000,    ///< Задержка после разрыва соединения для быстрой попытки восстановить связь.
    MaxFastReconnects    = 2,       ///< Максимальное количество попыток для использования FastReconnectTime.
    NormalReconnectTime  = 6000,    ///< Стандартный интервал попыток восстановить соединение.
    MaxNormalReconnects  = 8,       ///< Число попыток восстановить соединение с нормальным интервалом.
    MaxVirtualPackets    = 1000000, ///< Максимальное число виртуальный пакетов, внутри одного транспортного пакета.
    MaxPacketSize        = 5242880  ///< Максимальный размер транспортного пакета.
  };

  /*!
  Transport packet types.
  Structure of transport packet:
    - 04 bytes - size of packet, without 4 bytes.
    - 01 byte  - type, this enum.
    - 01 byte  - packet version \sa Version.
    - 01 byte  - packet subversion \sa Version.
    - 01 byte  - packet options \sa Options.
    - 08 bytes - sequence.
    - 08 bytes - timestamp (is set TimeStamp).
    - 04 bytes - count of virtual packets.
    - 02 bytes ... list of sizes of virtual packets, or 4 bytes is used option HugePackets.
    - ...      - packet data.
  */
  enum TransportPackets {
    GenericTransport = 0x0D ///< Generic Transport Packet.
  };

  /// Transport packet options.
  enum Options {
    NoOptions              = 0, ///< No packet options.
    ContainsInternalPacket = 1, ///< Содержит виртуальный пакет для внутреннего использования на транспортном уровне.
    HugePackets            = 2, ///< Поддержка больших виртуальных пакетов, для указания размера используется quint32.
    TimeStamp              = 4  ///< Содержит время в виде числа миллисекунд с 1970-01-01T00:00:00.000 UTC.
  };

  /*!
    Типы виртуальных пакетов.

    Structure of virtual packet:
      - 02 bytes - type.
      - 01 byte  - subtype, reserved.
      - 01 byte  - reserved.
      - 01 byte  - header option.
      - 21 bytes - sender (SHA1 hash).
      - 21 bytes - destination (SHA1 hash).
      - ...      - packet data.
    */
  enum VirtualPackets {
    /// Отчёт о доставке пакетов, ("dc").
    /// - 02 bytes - reserved.
    /// - ...      - список пакетов QList<quint64>.
    DeliveryConfirmationPacket  = 0x7563, // "dc" or 30051
    /// Проверка возможности установки безопасного соединения.
    /// - 02 bytes - option \sa ProbeSecureConnectionOption.
    ProbeSecureConnectionPacket = 0x7073, // "ps" or 28787
    /// Запрос авторизации.
    AuthRequestPacket           = 0x6152, // "aR" or 24914
    /// Ответ на запрос авторизации.
    AuthReplyPacket             = 0x6172, // "ar" or 24946
    NoticePacket                = 0x6e // "n" or 110
  };

  /// Опция заголовка виртуального пакета.
  enum HeaderOption {
    BasicHeader      = 0, ///< Базовый пакет, не содержит идентификатора отправителя и назначения.
    SenderField      = 1, ///< Пакет содержит идентификатор отправителя.
    DestinationField = 2, ///< Пакет содержит идентификатор получателя.
    EnableEcho       = 8, ///< Пакет будет отослан обратно отправителю.
    ChannelField     = 16 ///< Пакет содержит идентификатор канала.
  };

  /// Опции виртуального пакета ProbeSecureConnection.
  enum ProbeSecureConnectionOption {
    SecureConnectionRequest     = 0x29A, ///< Запрос клиентом безопасного подключения.
    SecureConnectionUnavailable = 0x309, ///< Безопасное соединение не доступно.
    SecureConnectionAvailable   = 0x2A   ///< Безопасное соединение доступно.
  };

  inline Protocol() {}
};

#endif /* PROTOCOL_H_ */
