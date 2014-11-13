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

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>

#include "AuthCore.h"
#include "AuthState.h"
#include "JSON.h"
#include "net/SimpleID.h"
#include "NodeLog.h"
#include "oauth2/OAuthData.h"
#include "oauth2/stackoverflow/StackOverflowAuth.h"
#include "sglobal.h"
#include "UrlQuery.h"
#include "User.h"

StackOverflowAuth::StackOverflowAuth(const QUrl &url, const QString &path, Tufao::HttpServerRequest *request, Tufao::HttpServerResponse *response, const QString &successRedirect, QObject *parent)
  : OAuthHandler(LS("stackoverflow"), QUrlQuery(url).queryItemValue(LS("state")).toLatin1(), url, path, request, response, successRedirect, parent)
{
}


void StackOverflowAuth::dataReady()
{
  OAUTH_PREPARE_REPLY
  OAUTH_BAD_STATUS

  const QVariantList list = JSON::parse(raw).toMap().value(LS("items")).toList();
  if (list.isEmpty())
    return setError("bad_reply");

  const QVariantMap data = list.at(0).toMap();
  const QByteArray uid   = data.value(LS("user_id")).toByteArray();
  if (uid.isEmpty())
    return setError("invalid_uid");

  User user;
  user.nativeId = uid;
  user.provider = LS("stackoverflow");
  user.name     = data.value(LS("display_name")).toString();
  user.link     = data.value(LS("link")).toString();
  user.site     = data.value(LS("website_url")).toString();

  const QByteArray id = SimpleID::encode(SimpleID::make("stackoverflow:" + uid, SimpleID::UserId));
  AuthCore::state()->add(new AuthStateData(m_state, "stackoverflow", id, data, user));

  log(NodeLog::InfoLevel, "Data is successfully received, id:" + id + ", uid:" + uid);
  deleteLater();
}


void StackOverflowAuth::tokenReady()
{
  OAUTH_PREPARE_REPLY

  const QByteArray token = QUrlQuery(QUrl(LC('?') + raw)).queryItemValue(LS("access_token")).toUtf8();
  log(NodeLog::InfoLevel, "Token is successfully received");

  QNetworkRequest request(QUrl(LS("https://api.stackexchange.com/2.1/me?order=desc&sort=reputation&site=stackoverflow&access_token=") + token + LS("&key=") + m_provider->publicKey));
  QNetworkReply *reply = m_manager->get(request);
  connect(reply, SIGNAL(finished()), SLOT(dataReady()));
}


void StackOverflowAuth::getToken()
{
  QNetworkRequest request(QUrl(LS("https://stackexchange.com/oauth/access_token?client_id=") + m_provider->id +
      LS("&client_secret=") + m_provider->secret +
      LS("&code=") + m_code +
      LS("&redirect_uri=") + m_provider->redirect));

  request.setHeader(QNetworkRequest::ContentTypeHeader, LS("application/x-www-form-urlencoded"));

  QNetworkReply *reply = m_manager->post(request, QByteArray());
  connect(reply, SIGNAL(finished()), SLOT(tokenReady()));
}


bool StackOverflowAuthCreator::serve(const QUrl &url, const QString &path, Tufao::HttpServerRequest *request, Tufao::HttpServerResponse *response, QObject *parent)
{
  if (path == LS("/oauth2/stackoverflow")) {
    new StackOverflowAuth(url, path, request, response, m_successRedirect, parent);
    return true;
  }

  return false;
}
