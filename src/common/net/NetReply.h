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

#ifndef NETREPLY_H_
#define NETREPLY_H_

#include <QVariant>

#include "schat.h"

class SCHAT_EXPORT NetReply
{
public:
  enum StatusCode {
    // 1xx Informational
    CONTINUE                        = 100,
    SWITCHING_PROTOCOLS             = 101,
    PROCESSING                      = 102,
    CHECKPOINT                      = 103,
    // 2xx Successful
    OK                              = 200,
    CREATED                         = 201,
    ACCEPTED                        = 202,
    NON_AUTHORITATIVE_INFORMATION   = 203,
    NO_CONTENT                      = 204,
    RESET_CONTENT                   = 205,
    PARTIAL_CONTENT                 = 206,
    MULTI_STATUS                    = 207,
    ALREADY_REPORTED                = 208,
    IM_USED                         = 226,
    // 3xx Redirection
    MULTIPLE_CHOICES                = 300,
    MOVED_PERMANENTLY               = 301,
    FOUND                           = 302,
    SEE_OTHER                       = 303,
    NOT_MODIFIED                    = 304,
    USE_PROXY                       = 305,
    SWITCH_PROXY                    = 306,
    TEMPORARY_REDIRECT              = 307,
    RESUME_INCOMPLETE               = 308,
    // 4xx Client Error
    BAD_REQUEST                     = 400,
    UNAUTHORIZED                    = 401,
    PAYMENT_REQUIRED                = 402,
    FORBIDDEN                       = 403,
    NOT_FOUND                       = 404,
    METHOD_NOT_ALLOWED              = 405,
    NOT_ACCEPTABLE                  = 406,
    PROXY_AUTHENTICATION_REQUIRED   = 407,
    REQUEST_TIMEOUT                 = 408,
    CONFLICT                        = 409,
    GONE                            = 410,
    LENGTH_REQUIRED                 = 411,
    PRECONDITION_FAILED             = 412,
    REQUEST_ENTITY_TOO_LARGE        = 413,
    REQUEST_URI_TOO_LONG            = 414,
    UNSUPPORTED_MEDIA_TYPE          = 415,
    REQUESTED_RANGE_NOT_SATISFIABLE = 416,
    EXPECTATION_FAILED              = 417,
    I_AM_A_TEAPOT                   = 418,
    UNPROCESSABLE_ENTITY            = 422,
    LOCKED                          = 423,
    FAILED_DEPENDENCY               = 424,
    UNORDERED_COLLECTION            = 425,
    UPGRADE_REQUIRED                = 426,
    PRECONDITION_REQUIRED           = 428,
    TOO_MANY_REQUESTS               = 429,
    REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
    NO_RESPONSE                     = 444,
    RETRY_WITH                      = 449,
    CLIENT_CLOSED_REQUEST           = 499,
    // 5xx Internal Server Error
    INTERNAL_SERVER_ERROR           = 500,
    NOT_IMPLEMENTED                 = 501,
    BAD_GATEWAY                     = 502,
    SERVICE_UNAVAILABLE             = 503,
    GATEWAY_TIMEOUT                 = 504,
    HTTP_VERSION_NOT_SUPPORTED      = 505,
    VARIANT_ALSO_NEGOTIATES         = 506,
    INSUFFICIENT_STORAGE            = 507,
    LOOP_DETECTED                   = 508,
    BANDWIDTH_LIMIT_EXCEEDED        = 509,
    NOT_EXTENDED                    = 510
  };

  NetReply(const QString &id, int status = OK);
  NetReply(const QVariantList &list);
  bool isValid() const;
  QByteArray toJSON() const;

  QString id;          ///< Уникальный идентификатор сообщения.
  qint64 date;         ///< Отметка времени.
  int status;          ///< Статусный код.
  QVariantMap headers; ///< Дополнительные заголовки в виде JSON объекта.
  QVariant data;       ///< Данные запроса.
};

#endif /* NETREPLY_H_ */
