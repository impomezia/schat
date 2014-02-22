/* $Id: RestApiCore.cpp 3722 2013-07-02 23:05:19Z IMPOMEZIA $
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

#include <QTimer>
#include <QUrl>

#include "RestApiCore.h"
#include "NodeLog.h"
#include "RestHandler.h"
#include "Settings.h"
#include "sglobal.h"
#include "Storage.h"
#include "Tufao/headers.h"
#include "Tufao/httpserver.h"
#include "Tufao/httpserverrequest.h"
#include "Tufao/httpserverresponse.h"
#include "Tufao/url.h"

RestApiCore *RestApiCore::m_self = 0;

RestApiCore::RestApiCore(QObject *parent)
  : QObject(parent)
  , m_prefixSize(0)
  , m_settings(Storage::settings())
{
  m_self = this;

  m_settings->setDefault(LS("RestApi/Listen"), QStringList("http://0.0.0.0:7669"));
  m_settings->setDefault(LS("RestApi/Prefix"), LS("/api"));

  QTimer::singleShot(0, this, SLOT(start()));
}


RestApiCore::~RestApiCore()
{
  qDeleteAll(m_handlers);
}


bool RestApiCore::add(const QUrl &url)
{
  if (!m_self)
    return false;

  Tufao::HttpServer *server = new Tufao::HttpServer(m_self);

  if (server->listen(QHostAddress(url.host()), url.port())) {
    connect(server, SIGNAL(requestReady(Tufao::HttpServerRequest*,Tufao::HttpServerResponse*)), m_self, SLOT(handleRequest(Tufao::HttpServerRequest*,Tufao::HttpServerResponse*)));
    m_self->m_servers.append(server);
    return true;
  }

  server->deleteLater();
  return false;
}


bool RestApiCore::add(RestHandler *handler)
{
  if (!handler)
    return false;

  if (!m_self) {
    delete handler;
    return false;
  }

  m_self->m_handlers.append(handler);
  return true;
}


void RestApiCore::start()
{
  m_prefixSize = m_settings->value(LS("RestApi/Prefix")).toString().size();

  QStringList listen = m_settings->value(LS("RestApi/Listen")).toStringList();
  foreach (const QString &url, listen) {
    if (add(QUrl(url))) {
      LOG_INFO("R1010", "REST/Core", "listen:" << url);
    }
    else {
      LOG_INFO("R1011", "REST/Core", "failed to listen:" << url);
    }
  }
}


/*!
 * Обработка запроса.
 */
void RestApiCore::handleRequest(Tufao::HttpServerRequest *request, Tufao::HttpServerResponse *response)
{
  QUrl url = QString(Tufao::Url::url(request));
  QString path(QUrl::fromPercentEncoding(url.path().toUtf8()));
  if (m_prefixSize)
    path.remove(0, m_prefixSize);

  response->headers().replace("Content-Type", "application/json");
  response->headers().insert("Access-Control-Allow-Origin", "*");

  foreach (RestHandler *handler, m_handlers) {
    if (handler->serve(url, path, request, response))
      return;
  }

  response->writeHead(Tufao::HttpServerResponse::NOT_FOUND);
  response->headers().insert("Cache-Control", "no-cache, no-store, max-age=0, must-revalidate");
  response->headers().insert("Pragma", "no-cache");
  response->headers().insert("Expires", "Fri, 01 Jan 1990 00:00:00 GMT");
  response->end();
}
