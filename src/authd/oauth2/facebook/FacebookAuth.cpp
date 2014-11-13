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

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "AuthCore.h"
#include "AuthState.h"
#include "JSON.h"
#include "net/SimpleID.h"
#include "NodeLog.h"
#include "oauth2/facebook/FacebookAuth.h"
#include "oauth2/OAuthData.h"
#include "sglobal.h"
#include "UrlQuery.h"

FacebookAuth::FacebookAuth(const QUrl &url, const QString &path, Tufao::HttpServerRequest *request, Tufao::HttpServerResponse *response, const QString &successRedirect, QObject *parent)
  : OAuthHandler(LS("facebook"), QUrlQuery(url).queryItemValue(LS("state")).toLatin1(), url, path, request, response, successRedirect, parent)
{
}


void FacebookAuth::dataReady()
{
  OAUTH_PREPARE_REPLY
  OAUTH_BAD_STATUS

  QVariantMap data = JSON::parse(raw).toMap();
  QByteArray uid = data.value(LS("id")).toByteArray();
  if (uid.isEmpty())
    return setError("invalid_uid");

  User user;
  user.nativeId = uid;
  user.provider = LS("facebook");
  user.gender   = data.value(LS("gender")).toString();
  user.name     = data.value(LS("name")).toString();
  user.email    = data.value(LS("email")).toString();
  user.link     = data.value(LS("link")).toString();

  QStringList birthday = data.value(LS("birthday")).toString().split(LC('/'));
  if (birthday.size() == 3)
    user.birthday = birthday.at(2) + LC('-') + birthday.at(0) + LC('-') + birthday.at(1);

  QByteArray id = SimpleID::encode(SimpleID::make("facebook:" + uid, SimpleID::UserId));
  AuthCore::state()->add(new AuthStateData(m_state, "facebook", id, data, user));

  log(NodeLog::InfoLevel, "Data is successfully received, id:" + id + ", uid:" + uid);
  deleteLater();
}


void FacebookAuth::tokenReady()
{
  OAUTH_PREPARE_REPLY

  if (status != 200) {
    QVariantMap data = JSON::parse(raw).toMap();
    return setError("token_error: " + data.value(LS("error")).toMap().value(LS("message")).toByteArray());
  }

  const QByteArray token = QUrlQuery(QUrl(LC('?') + raw)).queryItemValue(LS("access_token")).toUtf8();
  log(NodeLog::InfoLevel, "Token is successfully received");

  QNetworkRequest request(QUrl(LS("https://graph.facebook.com/me?access_token=") + token));
  QNetworkReply *reply = m_manager->get(request);
  connect(reply, SIGNAL(finished()), SLOT(dataReady()));
}


void FacebookAuth::getToken()
{
  QNetworkRequest request(QUrl(LS("https://graph.facebook.com/oauth/access_token?client_id=") +
      m_provider->id +
      LS("&redirect_uri=")  + m_provider->redirect +
      LS("&client_secret=") + m_provider->secret +
      LS("&code="           + m_code)));

  QNetworkReply *reply = m_manager->get(request);
  connect(reply, SIGNAL(finished()), SLOT(tokenReady()));
}


bool FacebookAuthCreator::serve(const QUrl &url, const QString &path, Tufao::HttpServerRequest *request, Tufao::HttpServerResponse *response, QObject *parent)
{
  if (path == LS("/oauth2/facebook")) {
    new FacebookAuth(url, path, request, response, m_successRedirect, parent);
    return true;
  }

  return false;
}
