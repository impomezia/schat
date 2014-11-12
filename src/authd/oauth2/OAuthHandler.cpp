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

#include <QFile>
#include <QNetworkReply>
#include <QTimer>

#include "AuthCore.h"
#include "AuthHandler.h"
#include "AuthState.h"
#include "id/ChatId.h"
#include "NodeLog.h"
#include "oauth2/OAuthData.h"
#include "oauth2/OAuthHandler.h"
#include "sglobal.h"
#include "Tufao/headers.h"
#include "Tufao/httpserverrequest.h"
#include "Tufao/httpserverresponse.h"
#include "UrlQuery.h"

OAuthHandler::OAuthHandler(const QString &provider, const QByteArray &state, const QUrl &url, const QString &path, Tufao::HttpServerRequest *request, Tufao::HttpServerResponse *response, const QString &successRedirect, QObject *parent)
  : QObject(parent)
  , m_path(path)
  , m_url(url)
  , m_manager(0)
  , m_reply(0)
  , m_successRedirect(successRedirect)
  , m_request(request)
  , m_response(response)
{
  m_provider = AuthCore::provider(provider);
  if (!m_provider) {
    AuthHandler::setError(response, Tufao::HttpServerResponse::INTERNAL_SERVER_ERROR);
    deleteLater();
    return;
  }

  setState(state);
  const QUrlQuery query(url);
  if (query.hasQueryItem(LS("error")) || !query.hasQueryItem(LS("code"))) {
    serveError();
    return;
  }

  serveOk();

  m_manager = new QNetworkAccessManager(this);
  m_code = QUrlQuery(url).queryItemValue(LS("code")).toUtf8();
  log(NodeLog::InfoLevel, "Start receiving token, code:" + m_code);

  QTimer::singleShot(0, this, SLOT(getToken()));
}


/*!
 * Получение содержимого файла, если файл находится в WWW директории сервера будет использоваться он
 * в противном случае будет использоваться файл из ресурсов. Это позволяет переопределить стандартные страницы.
 */
QByteArray OAuthHandler::page(const QString &name)
{
  QByteArray out;
  QFile file(AuthCore::root() + LC('/') + name);
  if (file.exists() && file.open(QIODevice::ReadOnly))
    out = file.readAll();

  if (out.isEmpty()) {
    file.setFileName(LS(":/www/") + name);
    if (file.open(QIODevice::ReadOnly))
      out = file.readAll();
  }

  return out;
}


/*!
 * Установка состояния ошибки.
 */
void OAuthHandler::setError(const QByteArray &error)
{
  if (m_reply) {
    m_reply->deleteLater();
    m_reply = 0;
  }

  log(NodeLog::ErrorLevel, "error: " + error);
  AuthCore::state()->add(new AuthStateData(m_state, error));
  deleteLater();
}


void OAuthHandler::log(int level, const QByteArray &text)
{
  if (NodeLog::level() < level)
    return;

  NodeLog::Helper(static_cast<NodeLog::Level>(level)).stream() << ("[" + m_provider->provider + "/" + m_state + "] " + text).constData();
}


/*!
 * Отображение страницы с ошибкой, если пользователь отменил авторизацию.
 */
void OAuthHandler::serveError()
{
  m_response->writeHead(Tufao::HttpServerResponse::OK);
  m_response->headers().replace("Content-Type", "text/html");

  QByteArray data = page(LS("error.html"));
  QByteArray lang = m_request->headers().value("Accept-Language").left(2);
  if (lang == "ru")
    data.replace("${ERROR_TEXT}", "<b>Авторизация была отменена вами</b>");
  else
    data.replace("${ERROR_TEXT}", "<b>Authorization has been canceled by you</b>");

  m_response->end(data);
  setError("access_denied");

  deleteLater();
}


void OAuthHandler::serveOk()
{
  if (m_redirect.isEmpty()) {
    m_response->writeHead(Tufao::HttpServerResponse::OK);
    m_response->headers().replace("Content-Type", "text/html");
    QByteArray data = page(LS("result.html"));
    data.replace("${LANGUAGE}", m_request->headers().value("Accept-Language").left(2));
    data.replace("${STATE_ID}", m_state);
    data.replace("${SUCCESS_REDIRECT}", m_successRedirect.toUtf8());
    m_response->end(data);
  }
  else {
    m_response->writeHead(Tufao::HttpServerResponse::FOUND);
    m_response->headers().replace("Location", m_redirect.toUtf8());
    m_response->end();
  }
}


/*!
 * Установка или генерация идентификатора состояния.
 */
void OAuthHandler::setState(const QByteArray &state)
{
  ChatId s(state.left(ChatId::kEncodedSize));
  if (s.isNull())
    s.init(ChatId::MessageId);

  m_redirect = ChatId::fromBase32(state.mid(ChatId::kEncodedSize));
  m_state = s.toBase32();
}
