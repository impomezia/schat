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
#include "AuthHandler.h"
#include "AuthState.h"
#include "id/ChatId.h"
#include "JSON.h"
#include "net/SimpleID.h"
#include "NodeLog.h"
#include "oauth2/OAuthData.h"
#include "oauth2/vkontakte/VkontakteAuth.h"
#include "sglobal.h"
#include "Tufao/httpserverresponse.h"

VkontakteAuth::VkontakteAuth(const QByteArray &state, const QUrl &url, const QString &path, Tufao::HttpServerRequest *request, Tufao::HttpServerResponse *response, const QString &successRedirect, QObject *parent)
  : OAuthHandler(LS("vkontakte"), state, url, path, request, response, successRedirect, parent)
{
}


void VkontakteAuth::dataReady()
{
  OAUTH_PREPARE_REPLY
  OAUTH_BAD_STATUS

  QVariantMap data = JSON::parse(raw).toMap();
  if (data.contains(LS("error")))
    return setError("data_error: " + data.value(LS("error")).toMap().value(LS("error_msg")).toByteArray());

  QVariantList list = data.value(LS("response")).toList();
  if (list.isEmpty())
    return setError("invalid_data");

  QVariantMap response = list.first().toMap();
  QByteArray uid = response.value(LS("uid")).toByteArray();
  if (uid.isEmpty())
    return setError("invalid_uid");

  User user;
  user.name = response.value(LS("first_name")).toString() + LC(' ') + response.value(LS("last_name")).toString();
  if (user.name == LS(" "))
    user.name = QString();

  QString screen_name = response.value(LS("screen_name")).toString();
  if (screen_name.isEmpty())
    user.link = LS("http://vk.com/id") + uid;
  else
    user.link = LS("http://vk.com/") + screen_name;

  QStringList birthday = response.value(LS("bdate")).toString().split(LC('.'));
  if (birthday.size() == 3) {
    user.birthday = birthday.at(2) + LC('-');
    if (birthday.at(1).size() == 1)
      user.birthday += LC('0');

    user.birthday += birthday.at(1) + LC('-');

    if (birthday.at(0).size() == 1)
      user.birthday += LC('0');

    user.birthday += birthday.at(0);
  }

  QByteArray id = SimpleID::encode(SimpleID::make("vkontakte:" + uid, SimpleID::UserId));
  AuthCore::state()->add(new AuthStateData(m_state, "vkontakte", id, response, user));

  log(NodeLog::InfoLevel, "Data is successfully received, id:" + id + ", uid:" + uid);
  deleteLater();
}


void VkontakteAuth::tokenReady()
{
  OAUTH_PREPARE_REPLY

  QVariantMap data = JSON::parse(raw).toMap();
  if (status != 200)
    return setError("token_error: " + data.value(LS("error")).toByteArray());

  QByteArray token = data.value(LS("access_token")).toByteArray();
  QByteArray user  = data.value(LS("user_id")).toByteArray();
  log(NodeLog::InfoLevel, "Token is successfully received");

  QNetworkRequest request(QUrl(LS("https://api.vk.com/method/users.get?uids=") + user +
      "&fields=uid,first_name,last_name,nickname,screen_name,sex,bdate,city,country,timezone,photo,photo_medium,photo_big" +
      "&access_token=" + token));

  QNetworkReply *reply = m_manager->get(request);
  connect(reply, SIGNAL(finished()), SLOT(dataReady()));
}


void VkontakteAuth::getToken()
{
  QNetworkRequest request(QUrl(LS("https://oauth.vk.com/access_token?client_id=") +
      m_provider->id +
      LS("&client_secret=") + m_provider->secret +
      LS("&code="           + m_code)));

  QNetworkReply *reply = m_manager->get(request);
  connect(reply, SIGNAL(finished()), SLOT(tokenReady()));
}


bool VkontakteAuthCreator::serve(const QUrl &url, const QString &path, Tufao::HttpServerRequest *request, Tufao::HttpServerResponse *response, QObject *parent)
{
  if (path == LS("/oauth2/vkontakte")) {
    AuthHandler::setError(response, Tufao::HttpServerResponse::BAD_REQUEST);
  }
  else if (path.startsWith(LS("/oauth2/vkontakte/"))) {
    const QByteArray state = path.section(LC('/'), 3, 3).toLatin1();
    if (ChatId(state.left(ChatId::kEncodedSize)).type() != ChatId::MessageId) {
      AuthHandler::setError(response, Tufao::HttpServerResponse::BAD_REQUEST);
      return true;
    }

    new VkontakteAuth(state, url, path, request, response, m_successRedirect, parent);
    return true;
  }

  return false;
}
