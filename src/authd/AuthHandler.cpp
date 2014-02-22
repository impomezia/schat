/* $Id: AuthHandler.cpp 2908 2012-07-23 18:23:54Z IMPOMEZIA $
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

#include <QDebug>

#include <QAbstractSocket>
#include <QDir>
#include <QUrl>

#include "AuthHandler.h"
#include "HandlerCreator.h"
#include "HandlerRoute.h"
#include "oauth2/OAuthHandler.h"
#include "sglobal.h"
#include "Tufao/headers.h"
#include "Tufao/httpserverrequest.h"
#include "Tufao/priv/reasonphrase.h"
#include "Tufao/url.h"

AuthHandler::AuthHandler(QObject *parent)
  : QObject(parent)
{
  m_route = new HandlerRoute();
}


AuthHandler::~AuthHandler()
{
  delete m_route;
}


/*!
 * Установка HTTP ошибки.
 * Используется минимальный HTML шаблон с текстом ошибки.
 */
void AuthHandler::setError(Tufao::HttpServerResponse *response, int statusCode)
{
  response->writeHead(statusCode);
  response->headers().replace("Content-Type", "text/html");

  QByteArray text = QByteArray::number(statusCode) + ' ' +  Tufao::reasonPhrase(statusCode);
  response->end("<html><head><title>" + text + "</title></head><body bgcolor=\"white\"><center><h1>" + text + "</h1><hr></body></html>");
}


/*!
 * Обработка запросов.
 */
void AuthHandler::handleRequest(Tufao::HttpServerRequest *request, Tufao::HttpServerResponse *response)
{
  QUrl url = QString(Tufao::Url::url(request));
  QString path(QUrl::fromPercentEncoding(url.path().toUtf8()));
  QString fileName(QDir::cleanPath(m_root + path));

  if (path == LS("/")) {
    response->writeHead(Tufao::HttpServerResponse::OK);
    response->headers().replace("Content-Type", "text/html");
    QByteArray data = OAuthHandler::page(LS("index.html"));
    response->end(data);
    return;
  }

  // Плохой запрос, файл не находится внутри корневой директории сервера.
  if (!fileName.startsWith(m_root)) {
    setError(response, Tufao::HttpServerResponse::BAD_REQUEST);
    return;
  }

  QFileInfo fileInfo(fileName);
  // Если локальный файл не найден, но запрос был обработан, выходим из функции.
  bool exists = fileInfo.exists();
  if (!exists && serve(url, path, request, response))
    return;

  // Если файл не найден возвращаем "404 Not Found".
  if (!exists) {
    setError(response, Tufao::HttpServerResponse::NOT_FOUND);
    return;
  }

  // Если файл является папкой или не доступен для чтения возвращаем "403 Forbidden".
  if (fileInfo.isDir() || !fileInfo.isReadable()) {
    setError(response, Tufao::HttpServerResponse::FORBIDDEN);
    return;
  }

  serveFile(fileName, request, response);
}


/*!
 * Обработка внутренних запросов.
 */
bool AuthHandler::serve(const QUrl &url, const QString &path, Tufao::HttpServerRequest *request, Tufao::HttpServerResponse *response)
{
  for (int i = 0; i < HandlerRoute::routes.size(); ++i) {
    if (HandlerRoute::routes.at(i)->serve(url, path, request, response, this))
      return true;
  }

  return false;
}


void AuthHandler::serveFile(const QString &fileName, Tufao::HttpServerRequest *request, Tufao::HttpServerResponse *response)
{
  const QByteArray method(request->method());
  Tufao::Headers &headers = response->headers();

  if (method != "GET" && method != "HEAD") {
    response->writeHead(Tufao::HttpServerResponse::METHOD_NOT_ALLOWED);
    headers.insert("Allow", "GET, HEAD");
    response->end();
    return;
  }

  QFileInfo fileInfo(fileName);
  response->writeHead(Tufao::HttpServerResponse::OK);
  headers.insert("Date", Tufao::Headers::fromDateTime(QDateTime::currentDateTime()));
  headers.insert("Last-Modified", Tufao::Headers::fromDateTime(fileInfo.lastModified()));

  if (request->method() == "HEAD") {
    response->writeHead(Tufao::HttpServerResponse::OK);
    response->end();
    return;
  }

  QFile file(fileName);
  file.open(QIODevice::ReadOnly);

  while (!file.atEnd()) {
    (*response) << file.read(1024 * 8);
    request->socket()->flush();
  }

  response->end();
}
