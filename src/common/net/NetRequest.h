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

#ifndef NETREQUEST_H_
#define NETREQUEST_H_

#include <QVariant>

#include "schat.h"

class SCHAT_EXPORT NetRequest
{
public:
  static const QString AUTH;
  static const QString DEL;
  static const QString GET;
  static const QString POST;
  static const QString PUB;
  static const QString PUT;
  static const QString SUB;
  static const QString UNSUB;

  NetRequest();
  NetRequest(const QVariantList &list);
  bool isValid() const;
  QByteArray toJSON() const;
  static QString genId();

  QString id;          ///< Уникальный идентификатор сообщения, формируется клиентом, сервер должен отправить его обратно в неизменном виде.
  qint64 date;         ///< Отметка времени.
  QString method;      ///< Метод запроса, например GET, PUT, POST, DEL и AUTH.
  QString request;     ///< Имя ресурса к которому производится запрос.
  QVariantMap headers; ///< Дополнительные заголовки в виде JSON объекта.
  QVariant data;       ///< Данные запроса.
};

#endif /* NETREQUEST_H_ */
