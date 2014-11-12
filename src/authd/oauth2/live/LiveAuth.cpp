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
#include "oauth2/live/LiveAuth.h"
#include "oauth2/OAuthData.h"
#include "sglobal.h"
#include "UrlQuery.h"

LiveAuth::LiveAuth(const QUrl &url, const QString &path, Tufao::HttpServerRequest *request, Tufao::HttpServerResponse *response, const QString &successRedirect, QObject *parent)
  : OAuthHandler(LS("live"), QUrlQuery(url).queryItemValue(LS("state")).toLatin1(), url, path, request, response, successRedirect, parent)
{
}


void LiveAuth::dataReady()
{
  OAUTH_PREPARE_REPLY

  QVariantMap data = JSON::parse(raw).toMap();
  QByteArray uid = data.value(LS("id")).toByteArray();
  if (uid.isEmpty())
    return setError("invalid_uid");

  User user;
  user.name  = data.value(LS("name")).toString();
  user.email = data.value(LS("emails")).toMap().value(LS("account")).toString();
  user.link  = data.value(LS("link")).toString();

  Birthday birthday(data.value(LS("birth_day")).toInt(), data.value(LS("birth_month")).toInt(), data.value(LS("birth_year")).toInt());
  if (birthday.isValid())
    user.birthday = birthday.toString();

  QByteArray id = SimpleID::encode(SimpleID::make("live:" + uid, SimpleID::UserId));
  AuthCore::state()->add(new AuthStateData(m_state, "live", id, data, user));

  log(NodeLog::InfoLevel, "Data is successfully received, id:" + id + ", uid:" + uid);
}


void LiveAuth::tokenReady()
{
  OAUTH_PREPARE_REPLY

  QVariantMap data = JSON::parse(raw).toMap();
  QByteArray token = data.value(LS("access_token")).toByteArray();
  if (token.isEmpty())
    return setError("token_error: " + data.value(LS("error")).toByteArray());

  log(NodeLog::InfoLevel, "Token is successfully received");

  QNetworkRequest request(QUrl(LS("https://apis.live.net/v5.0/me?access_token=") + token));
  QNetworkReply *reply = m_manager->get(request);
  connect(reply, SIGNAL(finished()), SLOT(dataReady()));
}

void LiveAuth::getToken()
{
  QNetworkRequest request(QUrl(LS("https://login.live.com/oauth20_token.srf")));
  request.setHeader(QNetworkRequest::ContentTypeHeader, LS("application/x-www-form-urlencoded"));

  QByteArray body = "client_id="      + m_provider->id +
                    "&redirect_uri="  + m_provider->redirect +
                    "&client_secret=" + m_provider->secret +
                    "&code="          + m_code +
                    "&grant_type=authorization_code";

  QNetworkReply *reply = m_manager->post(request, body);
  connect(reply, SIGNAL(finished()), SLOT(tokenReady()));
}


bool LiveAuthCreator::serve(const QUrl &url, const QString &path, Tufao::HttpServerRequest *request, Tufao::HttpServerResponse *response, QObject *parent)
{
  if (path == LS("/oauth2/live")) {
    new LiveAuth(url, path, request, response, m_successRedirect, parent);
    return true;
  }

  return false;
}
