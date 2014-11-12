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
#include "JSON.h"
#include "net/SimpleID.h"
#include "NodeLog.h"
#include "oauth2/github/GithubAuth.h"
#include "oauth2/OAuthData.h"
#include "sglobal.h"
#include "UrlQuery.h"

GithubAuth::GithubAuth(const QUrl &url, const QString &path, Tufao::HttpServerRequest *request, Tufao::HttpServerResponse *response, const QString &successRedirect, QObject *parent)
  : OAuthHandler(LS("github"), QUrlQuery(url).queryItemValue(LS("state")).toLatin1(), url, path, request, response, successRedirect, parent)
{
}


void GithubAuth::dataReady()
{
  OAUTH_PREPARE_REPLY

  QVariantMap data = JSON::parse(raw).toMap();
  QByteArray uid = data.value(LS("id")).toByteArray();
  if (uid.isEmpty())
    return setError("invalid_uid");

  User user;
  user.link  = data.value(LS("html_url")).toString();
  user.name  = data.value(LS("name")).toString();
  user.email = data.value(LS("email")).toString();
  user.site  = data.value(LS("blog")).toString();

  QByteArray id = SimpleID::encode(SimpleID::make("github:" + uid, SimpleID::UserId));
  AuthCore::state()->add(new AuthStateData(m_state, "github", id, data, user));

  log(NodeLog::InfoLevel, "Data is successfully received, id:" + id + ", uid:" + uid);
}


void GithubAuth::tokenReady()
{
  OAUTH_PREPARE_REPLY

  QVariantMap data = JSON::parse(raw).toMap();
  QByteArray token = data.value(LS("access_token")).toByteArray();
  if (token.isEmpty())
    return setError("token_error: " + data.value(LS("error")).toByteArray());

  log(NodeLog::InfoLevel, "Token is successfully received");

  QNetworkRequest request(QUrl(LS("https://api.github.com/user?access_token=") + token));
  QNetworkReply *reply = m_manager->get(request);
  connect(reply, SIGNAL(finished()), SLOT(dataReady()));
}


void GithubAuth::getToken()
{
  QNetworkRequest request(QUrl(LS("https://github.com/login/oauth/access_token?client_id=") + m_provider->id +
      LS("&client_secret=") + m_provider->secret +
      LS("&code=") + m_code +
      LS("&state=") + m_state));

  request.setHeader(QNetworkRequest::ContentTypeHeader, LS("application/x-www-form-urlencoded"));
  request.setRawHeader("Accept", "application/json");

  QNetworkReply *reply = m_manager->post(request, QByteArray());
  connect(reply, SIGNAL(finished()), SLOT(tokenReady()));
}


bool GithubAuthCreator::serve(const QUrl &url, const QString &path, Tufao::HttpServerRequest *request, Tufao::HttpServerResponse *response, QObject *parent)
{
  if (path == LS("/oauth2/github")) {
    new GithubAuth(url, path, request, response, m_successRedirect, parent);
    return true;
  }

  return false;
}
