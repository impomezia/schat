/* $Id: FeedHandler.cpp 3442 2013-01-25 19:58:38Z IMPOMEZIA $
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

#include "handlers/FeedHandler.h"
#include "JSON.h"
#include "Tufao/httpserverrequest.h"
#include "Tufao/httpserverresponse.h"

FeedHandler::FeedHandler()
  : RestHandler()
{
}


/*!
 * Универсальный метод для отдачи тела фида.
 */
bool FeedHandler::serveFeed(ChatChannel channel, const QString &feedName)
{
  if (!ifMethodAllowed())
    return true;

  if (!channel || !channel->feeds().all().contains(feedName)) {
    setNoStore();
    m_response->writeHead(Tufao::HttpServerResponse::NOT_FOUND);
    m_response->end();
    return true;
  }

  FeedPtr feed = channel->feed(feedName);
  qint64 date  = feed->head().date();

  RestReplyCache &cache = m_cache[channel->id() + feedName.toUtf8()];
  if (cache.date != date) {
    cache.date = date;
    cache.etag = etag(date, m_path.toUtf8());

    cache.body = JSON::generate(feed->feed());
  }

  setLastModified(date);
  setETag(cache.etag);
  setNoCache();

  if (!ifModified(cache.etag)) {
    m_response->writeHead(Tufao::HttpServerResponse::NOT_MODIFIED);
    m_response->end();
    return true;
  }

  m_response->writeHead(Tufao::HttpServerResponse::OK);
  if (m_request->method() != "HEAD") {
    setContentLength(cache.body.size());
    m_response->end(cache.body);
  }
  else
    m_response->end();

  return true;
}
