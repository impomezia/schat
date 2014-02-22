/* $Id: ServerHandler.cpp 3442 2013-01-25 19:58:38Z IMPOMEZIA $
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

#include "Ch.h"
#include "feeds/FeedStrings.h"
#include "handlers/ServerHandler.h"
#include "JSON.h"
#include "RestApi.h"
#include "sglobal.h"
#include "Tufao/httpserverrequest.h"
#include "Tufao/httpserverresponse.h"

ServerHandler::ServerHandler()
  : FeedHandler()
{
}


bool ServerHandler::serve()
{
  if (!m_path.startsWith(LS(REST_API_SERVER)))
    return false;

  if (m_path == LS(REST_API_SERVER))
    return serveFeed(Ch::server(), FEED_NAME_SERVER);

  else if (m_path == LS(REST_API_SERVER_USERS))
    return serveFeed(Ch::server(), FEED_NAME_USERS);

  else if (m_path == LS(REST_API_SERVER_UPTIME))
    return uptime();

  return false;
}


/*!
 * Обработка запроса REST_API_SERVER_UPTIME.
 */
bool ServerHandler::uptime()
{
  if (!ifMethodAllowed())
    return true;

  FeedPtr feed = Ch::server()->feed(FEED_NAME_SERVER);

  setNoStore();

  m_response->writeHead(Tufao::HttpServerResponse::OK);
  if (m_request->method() != "HEAD") {
    QByteArray body = JSON::generate(feed->get(LS("uptime")).json);
    setContentLength(body.size());
    m_response->end(body);
  }
  else
    m_response->end();

  return true;
}
