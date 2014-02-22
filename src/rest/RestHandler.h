/* $Id: RestHandler.h 3018 2012-08-19 19:35:34Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2012 IMPOMEZIA <schat@impomezia.com>
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

#ifndef RESTHANDLER_H_
#define RESTHANDLER_H_

#include <QStringList>
#include <QUrl>

#include "schat.h"

namespace Tufao {
  class HttpServerRequest;
  class HttpServerResponse;
  struct Headers;
}

struct RestReplyCache
{
  RestReplyCache()
  : date(0)
  {}

  QByteArray body; ///< Тело ответа.
  QByteArray etag; ///< ETag заголовок.
  qint64 date;     ///< Дата последнего обновления.
};


class SCHAT_REST_EXPORT RestHandler
{
public:
  RestHandler()
  : m_request(0)
  , m_response(0)
  {}

  virtual ~RestHandler() {}
  bool serve(const QUrl &url, const QString &path, Tufao::HttpServerRequest *request, Tufao::HttpServerResponse *response);
  static QByteArray etag(qint64 date, const QByteArray &salt);

protected:
  virtual bool serve();

  bool ifMethodAllowed(const QStringList &methods = QStringList() << "GET" << "HEAD");
  bool ifModified(const QByteArray &etag);
  void setContentLength(qint64 size);
  void setETag(const QByteArray &etag);
  void setLastModified(qint64 date);
  void setNoCache();
  void setNoStore();

  QString m_path;                        ///< Путь API.
  QUrl m_url;                            ///< Полный url адрес запроса.
  Tufao::HttpServerRequest *m_request;   ///< HTTP запрос.
  Tufao::HttpServerResponse *m_response; ///< HTTP ответ.
};

#endif /* RESTHANDLER_H_ */
