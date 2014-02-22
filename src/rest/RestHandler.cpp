/* $Id: RestHandler.cpp 3018 2012-08-19 19:35:34Z IMPOMEZIA $
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

#include <QCryptographicHash>

#include "DateTime.h"
#include "net/SimpleID.h"
#include "RestHandler.h"
#include "sglobal.h"
#include "Tufao/headers.h"
#include "Tufao/httpserverrequest.h"
#include "Tufao/httpserverresponse.h"

bool RestHandler::serve(const QUrl &url, const QString &path, Tufao::HttpServerRequest *request, Tufao::HttpServerResponse *response)
{
  m_url      = url;
  m_path     = path;
  m_request  = request;
  m_response = response;
  return serve();
}


/*!
 * Формирование ETag строки на основе даты и соли.
 *
 * Дата приводится к строковому виду и хешируется вместе с солью с помощью SHA1, затем кодируется с помощью Base64, из результата удаляется дополняющий символ.
 */
QByteArray RestHandler::etag(qint64 date, const QByteArray &salt)
{
  return '"' + QCryptographicHash::hash(QByteArray::number(date) + salt, QCryptographicHash::Sha1).toBase64().replace('=', QByteArray()) + '"';
}


bool RestHandler::serve()
{
  return false;
}


bool RestHandler::ifMethodAllowed(const QStringList &methods)
{
  if (methods.contains(m_request->method()))
    return true;

  setNoStore();
  m_response->writeHead(Tufao::HttpServerResponse::METHOD_NOT_ALLOWED);
  m_response->headers().insert("Allow", methods.join(LS(", ")).toLatin1());
  m_response->end();
  return false;
}


bool RestHandler::ifModified(const QByteArray &etag)
{
  QByteArray tag = m_request->headers().value("If-None-Match");
  if (tag.isEmpty() || tag != etag)
    return true;

  return false;
}


void RestHandler::setContentLength(qint64 size)
{
  m_response->headers().insert("Content-Length", QByteArray::number(size));
}


void RestHandler::setETag(const QByteArray &etag)
{
  m_response->headers().insert("ETag", etag);
}


void RestHandler::setLastModified(qint64 date)
{
  m_response->headers().insert("Last-Modified", Tufao::Headers::fromDateTime(DateTime::toDateTime(date)));
}


void RestHandler::setNoCache()
{
  Tufao::Headers &headers = m_response->headers();
  headers.insert("Cache-Control", "no-cache, must-revalidate");
  headers.insert("Pragma", "no-cache");
}


void RestHandler::setNoStore()
{
  Tufao::Headers &headers = m_response->headers();
  headers.insert("Cache-Control", "no-cache, no-store, max-age=0, must-revalidate");
  headers.insert("Pragma", "no-cache");
  headers.insert("Expires", "Fri, 01 Jan 1990 00:00:00 GMT");
}
