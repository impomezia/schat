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

#ifndef OAUTHHANDLER_H_
#define OAUTHHANDLER_H_

#include <QUrl>

#include "HandlerCreator.h"
#include "schat.h"

class OAuthData;
class QNetworkAccessManager;
class QNetworkReply;

class OAuthHandler : public QObject
{
  Q_OBJECT

public:
  OAuthHandler(const QString &provider, const QByteArray &state, const QUrl &url, const QString &path, Tufao::HttpServerRequest *request, Tufao::HttpServerResponse *response, QObject *parent = 0);
  static QByteArray page(const QString &name);

protected slots:
  virtual void getToken() {}

protected:
  virtual void setError(const QByteArray &error = "unknown_error");
  void log(int level, const QByteArray &text);
  void serveError();
  void serveOk();
  void setState(const QByteArray &state);

  const QString &m_path;                 ///< Относительный путь.
  const QUrl &m_url;                     ///< Полный URL адрес запроса.
  OAuthData *m_provider;                 ///< Информация о провайдере.
  QByteArray m_code;                     ///< Авторизационный код.
  QByteArray m_state;                    ///< Base32 кодированный идентификатор состояния.
  QNetworkAccessManager *m_manager;      ///< Менеджер доступа к сети.
  QNetworkReply *m_reply;                ///< Текущий завершённый HTTP запрос.
  QString m_redirect;                    ///< Адрес для редиректа клиента.
  Tufao::HttpServerRequest *m_request;   ///< HTTP запрос.
  Tufao::HttpServerResponse *m_response; ///< HTTP ответ.
};

#define OAUTH_PREPARE_REPLY \
    m_reply = qobject_cast<QNetworkReply*>(sender());                                      \
    if (!m_reply)                                                                          \
      return;                                                                              \
                                                                                           \
    if (m_reply->error())                                                                  \
      return setError("network_error: " + m_reply->errorString().toUtf8());                \
                                                                                           \
    QByteArray raw = m_reply->readAll();                                                   \
    int status     = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(); \
                                                                                           \
    m_reply->deleteLater();                                                                \
    m_reply = 0;                                                                           \
    Q_UNUSED(status)                                                                       \

#define OAUTH_BAD_STATUS if (status != 200) return setError("bad_status: " + QByteArray::number(status));

#endif /* OAUTHHANDLER_H_ */
