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

#include <QNetworkReply>

#include "AuthCore.h"
#include "AuthState.h"
#include "id/ChatId.h"
#include "JSON.h"
#include "net/SimpleID.h"
#include "NodeLog.h"
#include "oauth2/OAuthData.h"
#include "sglobal.h"
#include "SimpleIdAuth.h"
#include "UrlQuery.h"

SimpleIdAuth::SimpleIdAuth(const QUrl &url, const QString &path, Tufao::HttpServerRequest *request, Tufao::HttpServerResponse *response, const QString &successRedirect, QObject *parent)
  : OAuthHandler(LS("simpleid"), QUrlQuery(url).queryItemValue(LS("state")).toLatin1(), url, path, request, response, successRedirect, parent)
{
}


void SimpleIdAuth::dataReady()
{
  OAUTH_PREPARE_REPLY

  const QVariantMap data = JSON::parse(raw).toMap().value(LS("data")).toMap();
  const QByteArray uid   = data.value(LS("id")).toByteArray();
  if (uid.isEmpty())
    return setError("invalid_uid");

  User user;
  user.link  = data.value(LS("link")).toString();
  user.name  = data.value(LS("name")).toString();
  user.email = data.value(LS("email")).toString();
  user.site  = data.value(LS("site")).toString();

  const QByteArray id = SimpleID::encode(SimpleID::make("simpleid:" + uid, SimpleID::UserId));
  AuthCore::state()->add(new AuthStateData(m_state, "simpleid", id, data, user));

  log(NodeLog::InfoLevel, "Data is successfully received, id:" + id + ", uid:" + uid);
}


void SimpleIdAuth::tokenReady()
{
  OAUTH_PREPARE_REPLY

  const QVariantMap data = JSON::parse(raw).toMap();
  const QByteArray token = data.value(LS("access_token")).toByteArray();
  if (token.isEmpty())
    return setError("token_error: " + data.value(LS("error")).toByteArray());

  log(NodeLog::InfoLevel, "Token is successfully received");

  QNetworkRequest request(QUrl(LS("https://api.schat.me/1/user")));
  request.setRawHeader("Authorization", "Bearer " + token);
  QNetworkReply *reply = m_manager->get(request);
  connect(reply, SIGNAL(finished()), SLOT(dataReady()));
}


void SimpleIdAuth::getToken()
{
  QNetworkRequest request(QUrl(LS("https://id.schat.me/oauth/access_token")));
  request.setHeader(QNetworkRequest::ContentTypeHeader, LS("application/x-www-form-urlencoded"));

  QByteArray body = "code=" + m_code;
  body += "&client_id="     + m_provider->id;
  body += "&client_secret=" + m_provider->secret;
  body += "&redirect_uri="  + m_provider->redirect;
  body += "&grant_type=authorization_code";

  QNetworkReply *reply = m_manager->post(request, body);
  connect(reply, SIGNAL(finished()), SLOT(tokenReady()));
}


bool SimpleIdAuthCreator::serve(const QUrl &url, const QString &path, Tufao::HttpServerRequest *request, Tufao::HttpServerResponse *response, QObject *parent)
{
  if (path == LS("/oauth2/simpleid")) {
    SimpleIdAuth *auth = new SimpleIdAuth(url, path, request, response, m_successRedirect, parent);
    return true;
  }

  return false;
}
