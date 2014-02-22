/* $Id: ProvidersHandler.cpp 3761 2013-08-01 08:16:32Z IMPOMEZIA $
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

#include <QStringList>
#include <QUrl>

#include "AuthCore.h"
#include "handlers/ProvidersHandler.h"
#include "JSON.h"
#include "oauth2/OAuthData.h"
#include "Settings.h"
#include "sglobal.h"
#include "Tufao/headers.h"
#include "Tufao/httpserverrequest.h"
#include "Tufao/httpserverresponse.h"
#include "UrlQuery.h"

bool ProvidersHandler::serve(const QUrl &url, const QString &path, Tufao::HttpServerRequest *request, Tufao::HttpServerResponse *response, QObject *)
{
  if (path == LS("/providers")) {
    response->writeHead(Tufao::HttpServerResponse::OK);
    response->headers().replace("Content-Type", "application/json");

    QByteArray body;
    QVariantMap list;
    const QMap<QString, OAuthData *> &providers = AuthCore::i()->providers();

    QMapIterator<QString, OAuthData *> i(providers);
    while (i.hasNext()) {
      i.next();
      list[i.key()] = i.value()->toJSON(QUrlQuery(url).queryItemValue(LS("state")).toLatin1());
    }

    if (m_order.isEmpty()) {
      m_order = AuthCore::settings()->value(LS("Order")).toStringList();
      QMutableStringListIterator j(m_order);
      while (j.hasNext()) {
        QString &name = j.next();
        if (!providers.contains(name))
          j.remove();
      }
    }

    QVariantMap data;
    data[LS("providers")] = list;
    data[LS("order")]     = m_order;
    body = JSON::generate(data);

    response->end(body);
    return true;
  }
  else if (path == LS("/providers.json")) {
    response->headers().replace("Content-Type", "application/json");

    if (!m_date.isNull() && request->headers().contains("If-Modified-Since")) {
      if (Tufao::Headers::toDateTime(request->headers().value("If-Modified-Since")) <= m_date) {
        response->writeHead(Tufao::HttpServerResponse::NOT_MODIFIED);
        response->end();
        return true;
      }
    }

    response->writeHead(Tufao::HttpServerResponse::OK);

    if (m_providers.isEmpty()) {
      m_date = QDateTime::currentDateTime();
      const QMap<QString, OAuthData *> &providers = AuthCore::i()->providers();
      const QStringList order = AuthCore::settings()->value(LS("Order")).toStringList();

      QVariantList list;
      foreach (const QString &id, order) {
        if (providers.contains(id))
          list.push_back(providers.value(id)->toList());
      }

      m_providers = JSON::generate(list);
    }

    response->headers().insert("Last-Modified", Tufao::Headers::fromDateTime(m_date));
    response->end(m_providers);
    return true;
  }

  return false;
}
