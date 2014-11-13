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

#include <QCryptographicHash>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include "AuthCore.h"
#include "AuthState.h"
#include "JSON.h"
#include "net/SimpleID.h"
#include "NodeLog.h"
#include "oauth2/mail_ru/MailRuAuth.h"
#include "oauth2/OAuthData.h"
#include "sglobal.h"
#include "UrlQuery.h"

MailRuAuth::MailRuAuth(const QUrl &url, const QString &path, Tufao::HttpServerRequest *request, Tufao::HttpServerResponse *response, const QString &successRedirect, QObject *parent)
  : OAuthHandler(LS("mail_ru"), QUrlQuery(url).queryItemValue(LS("state")).toLatin1(), url, path, request, response, successRedirect, parent)
{
}


void MailRuAuth::dataReady()
{
  OAUTH_PREPARE_REPLY

  if (status != 200) {
    QByteArray error = JSON::parse(raw).toMap().value(LS("error")).toMap().value(LS("error_msg")).toByteArray();
    return setError("data_error: " + error);
  }

  QVariantList list = JSON::parse(raw).toList();
  if (list.isEmpty())
    return setError("invalid_data");

  QVariantMap data = list.first().toMap();
  QByteArray uid = data.value(LS("uid")).toByteArray();
  if (uid.isEmpty())
    return setError("invalid_uid");

  User user;
  user.nativeId = uid;
  user.provider = LS("mail_ru");
  user.gender   = data.value(LS("gender")).toString();
  user.name     = data.value(LS("nick")).toString();
  user.email    = data.value(LS("email")).toString();
  user.link     = data.value(LS("link")).toString();

  QStringList birthday = data.value(LS("birthday")).toString().split(LC('.'));
  if (birthday.size() == 3)
    user.birthday = birthday.at(2) + LC('-') + birthday.at(1) + LC('-') + birthday.at(0);

  QByteArray id = SimpleID::encode(SimpleID::make("mail_ru:" + uid, SimpleID::UserId));
  AuthCore::state()->add(new AuthStateData(m_state, "mail_ru", id, data, user));

  log(NodeLog::InfoLevel, "Data is successfully received, id:" + id + ", uid:" + uid);
  deleteLater();
}


void MailRuAuth::tokenReady()
{
  OAUTH_PREPARE_REPLY

  QVariantMap data = JSON::parse(raw).toMap();
  if (status != 200)
    return setError("token_error: " + data.value(LS("error")).toByteArray());

  QByteArray token = data.value(LS("access_token")).toByteArray();
  log(NodeLog::InfoLevel, "Token is successfully received");

  QByteArray sign = QCryptographicHash::hash("app_id=" + m_provider->id + "method=users.getInfosecure=1session_key=" + token + m_provider->secret, QCryptographicHash::Md5).toHex();
  QNetworkRequest request(QUrl(LS("https://www.appsmail.ru/platform/api?method=users.getInfo&secure=1&app_id=" + m_provider->id + "&session_key=" + token + "&sig=" + sign)));
  QNetworkReply *reply = m_manager->get(request);
  connect(reply, SIGNAL(finished()), SLOT(dataReady()));
}


void MailRuAuth::getToken()
{
  QNetworkRequest request(QUrl(LS("https://connect.mail.ru/oauth/token")));
  request.setHeader(QNetworkRequest::ContentTypeHeader, LS("application/x-www-form-urlencoded"));

  QByteArray body = "client_id=" + m_provider->id;
  body += "&client_secret="      + m_provider->secret;
  body += "&grant_type=authorization_code";
  body += "&code="               + m_code;
  body += "&redirect_uri="       + m_provider->redirect;

  QNetworkReply *reply = m_manager->post(request, body);
  connect(reply, SIGNAL(finished()), SLOT(tokenReady()));
}


bool MailRuAuthCreator::serve(const QUrl &url, const QString &path, Tufao::HttpServerRequest *request, Tufao::HttpServerResponse *response, QObject *parent)
{
  if (path == LS("/oauth2/mail_ru")) {
    new MailRuAuth(url, path, request, response, m_successRedirect, parent);
    return true;
  }

  return false;
}
