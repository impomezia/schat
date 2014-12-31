/* Simple Chat
 * Copyright (c) 2008-2015 Alexander Sedov <imp@schat.me>
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

#ifndef AUTH_H_
#define AUTH_H_

#include "net/packets/Notice.h"
#include "net/PacketWriter.h"

class Channel;
class PacketReader;
class User;

/*!
 * Ответ сервера на запрос авторизации.
 * Заголовок пакета:
 * - \b sender  - Идентификатор сервера.
 * - \b dest    - Идентификатор, выданный сервером клиенту, в случае если статус \p status не Notice::OK это поле отсутствует.
 *
 * Поля данных:
 *  - \b quint8    - Битовая маска AuthReply::Fields дополнительный полей пакета.
 *  - \b quint16   - Статус Notice::StatusCodes.
 *  - \b id        - Уникальный идентификатор авторизации, поле совпадает с таковым из AuthRequest.
 *    - \b id      - Cookie (\p status == Notice::OK).
 *    - \b quint32 - Зарезервировано (\p status == Notice::OK).
 *    - \b quint8  - Зарезервировано (\p status & Notice::OK).
 *    - \b utf8    - Имя сервера (\p status & Notice::OK).
 *    - \b utf8    - Имя аккаунта пользователя если пользователь зарегистрирован или пустая строка (\p status & Notice::OK).
 *  - \b json      - JSON данные (\p status & AuthReply::JSonField).
 */
class SCHAT_EXPORT AuthReply
{
public:
  /// Поля данных.
  enum Fields {
    BasicFields  = 0, ///< Поля по умолчанию.
    JSonField    = 1, ///< Присутствует поле с JSON данными.
    HostField    = 4, ///< Адрес с которого был подключен клиент. \since 1.99.32.
    ChannelField = 8  ///< Идентификатор основного канала и политика канала. \since 1.99.56.
  };

  AuthReply()
  : fields(0)
  , status(Notice::InternalError)
  , flags(0)
  {}

  AuthReply(PacketReader *reader);
  QByteArray data(QDataStream *stream) const;

  QByteArray serverId;   ///< Идентификатор сервера.
  QByteArray userId;     ///< Идентификатор пользователя, передаётся в заголовке пакета как адрес получателя.
  mutable quint8 fields; ///< Битовая маска дополнительный полей пакета.
  quint16 status;        ///< Статус \sa Notice::StatusCodes, обязательное поле.
  QByteArray cookie;     ///< Cookie.
  QByteArray id;         ///< Уникальный идентификатор авторизации.
  quint8 flags;          ///< Дополнительные флаги.
  QString serverName;    ///< Имя сервера.
  QString provider;      ///< Авторизационный провайдер пользователя.
  QVariantMap json;      ///< JSON данные.
  QString host;          ///< IP адрес клиента.
  QByteArray hostId;     ///< Идентификатор хоста клиента.
  QByteArray channel;    ///< Идентификатор главного канала сервера или запрощенного канала.
  quint8 policy;         ///< Политика главного канала.
};


/*!
 * Данные запроса авторизации.
 *
 * Поля данных:
 *  - \b quint8    - Битовая маска AuthRequest::fields дополнительный полей пакета.
 *  - \b quint8    - Тип авторизации AuthRequest::authType.
 *  - \b id        - Уникальный идентификатор клиента AuthRequest::uniqueId.
 *  - \b id        - Уникальный идентификатор авторизации AuthRequest::id.
 *  - \b quint8    - Пол и цвет пользователя AuthRequest::gender.
 *  - \b quint8    - Статус пользователя AuthRequest::status.
 *  - \b utf8      - Адрес по которому клиент подключается к серверу AuthRequest::host.
 *  - \b utf8      - Ник AuthRequest::nick.
 *  - \b utf8      - Идентификатор клиента пользователя AuthRequest::userAgent.
 *    - \b utf8    - AuthRequest::privateId, (\p authType == AuthRequest::SlaveNode).
 *    - \b utf8    - AuthRequest::account, (\p authType == AuthRequest::Password).
 *    - \b id      - AuthRequest::password, (\p authType == AuthRequest::Password).
 *    - \b id      - AuthRequest::cookie, (\p authType == AuthRequest::Cookie или \p authType == AuthRequest::Password).
 *    - \b json    - AuthRequest::json, (AuthRequest::fields & AuthRequest::JSonField)
 */
class SCHAT_EXPORT AuthRequest
{
public:
  /// Тип авторизации.
  enum AuthType {
    Anonymous = 97,   ///< 'a' Анонимная авторизация по уникальному идентификатору.
    Cookie    = 99,   ///< 'c' Сохранённая анонимная авторизация по ранее присвоеному долговременному Cookie.
    Discovery = 68,   ///< 'D' Определение доступных методов авторизации, если сервер не поддерживает внешнюю авторизацию, эквивалентно Anonymous.
    External  = 69    ///< 'E' Авторизация с помощью внешнего сервера.
  };

  /// Поля данных.
  enum Fields {
    BasicFields    = 0, ///< Стандартные поля данных.
    JSonField      = 1, ///< Дополнительно передаются произвольные JSON данные с максимальным размером MaxJSONSize.
    ExtraInfoField = 2  ///< Дополнительная информация о типе ос, версии клиента и локальном имени хоста.
  };

  /// Ограничения
  enum Limits {
    MaxJSONSize = 1024
  };

  AuthRequest(PacketReader *reader);
  bool isValid() const;
  QVariantMap toJSON() const;

public:
  quint8 fields;           ///< Битовая маска дополнительный полей пакета.
  quint8 authType;         ///< Тип авторизации.
  QByteArray uniqueId;     ///< Уникальный идентификатор клиента.
  QByteArray id;           ///< Уникальный идентификатор авторизации.
  quint8 gender;           ///< Пол и цвет пользователя.
  quint8 status;           ///< Статус пользователя.
  QString host;            ///< Адрес по которому клиент подключается к серверу.
  QString nick;            ///< Ник.
  QString userAgent;       ///< Идентификатор клиента пользователя.
  QByteArray cookie;       ///< Cookie, только для типа авторизации AuthRequest::Cookie.

  // JSonField.
  QVariantMap json;        ///< JSON данные.

  // ExtraInfoField.
  quint8 os;               ///< Базовый тип операционной системы.
  quint32 version;         ///< Версия клиента.
  qint32 tz;               ///< Смещение времени относительно UTC.
  QString hostName;        ///< Локальное имя хоста клиента.

private:
  void setStatus(quint8 status);
};

#endif /* AUTH_H_ */
