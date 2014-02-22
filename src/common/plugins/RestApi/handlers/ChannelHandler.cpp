/* $Id: ChannelHandler.cpp 3020 2012-08-20 22:29:16Z IMPOMEZIA $
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

#include "Ch.h"
#include "DateTime.h"
#include "handlers/ChannelHandler.h"
#include "JSON.h"
#include "net/SimpleID.h"
#include "sglobal.h"
#include "Tufao/httpserverrequest.h"
#include "Tufao/httpserverresponse.h"

ChannelHandler::ChannelHandler()
  : RestHandler()
{
}


bool ChannelHandler::serve()
{
  if (!m_path.startsWith(LS("/v1/channel/")))
    return false;

  QString encodedId = m_path.section(LC('/'), 3, 3);
  if (encodedId.isEmpty() || encodedId.size() != 34)
    return badRequest();

  QByteArray id = SimpleID::decode(encodedId);
  if (!Channel::isCompatibleId(id))
    return badRequest();

  ChatChannel channel = Ch::channel(id, SimpleID::typeOf(id));
  if (!channel) {
    setNoStore();
    m_response->writeHead(Tufao::HttpServerResponse::NOT_FOUND);
    m_response->end();
    return true;
  }

  QString request = m_path.section(LC('/'), 4, 4);
  if (request.isEmpty())
    this->channel(channel);
  else
    return false;

  Ch::gc(channel);
  return true;
}


bool ChannelHandler::badRequest()
{
  setNoStore();
  m_response->writeHead(Tufao::HttpServerResponse::BAD_REQUEST);
  m_response->end();
  return true;
}


bool ChannelHandler::channel(ChatChannel channel)
{
  if (!ifMethodAllowed())
    return true;

  qint64 date = channel->date();
  if (!date) {
    date = DateTime::utc();
    channel->setDate(date);
  }

  RestReplyCache &cache = m_cache[channel->id()];

  if (cache.date != date) {
    cache.date = date;
    cache.etag = etag(date, "/v1/channel");

    QVariantMap data;
    data[LS("name")] = channel->name();
    data[LS("type")] = SimpleID::typeName(channel->type());

    if (channel->type() == SimpleID::UserId) {
      data[LS("status")] = channel->status().toString();
      data[LS("gender")] = channel->gender().raw();
      data[LS("color")]  = channel->gender().toString();
    }

    cache.body = JSON::generate(data, true);
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
