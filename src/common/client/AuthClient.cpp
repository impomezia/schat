/* $Id: AuthClient.cpp 3707 2013-06-23 22:38:01Z IMPOMEZIA $
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

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslError>
#include <QTimer>
#include <QUrl>
#include <QTimerEvent>

#include "client/AuthClient.h"
#include "client/ChatClient.h"
#include "client/SimpleClient.h"
#include "JSON.h"
#include "net/SimpleID.h"
#include "sglobal.h"

AuthClient::AuthClient(QObject *parent)
  : QObject(parent)
  , m_retry(0)
  , m_reply(0)
{
  m_timer = new QBasicTimer();
  m_manager = new QNetworkAccessManager(this);
  connect(ChatClient::io(), SIGNAL(clientStateChanged(int,int)), SLOT(clientStateChanged(int)));
}


AuthClient::~AuthClient()
{
  delete m_timer;
}


/*!
 * Отмена авторизации.
 *
 * \sa AuthBridge::cancel();
 */
void AuthClient::cancel()
{
  m_timer->stop();
  deleteReply();
}


void AuthClient::start(const QString &url)
{
  m_timer->stop();
  deleteReply();

  m_retry = 1;
  m_url    = url;
  m_secret = ChatId(ChatId::PasswordId).toBase32();
  m_state  = ChatId(QUrl(url).host().toUtf8() + m_secret, ChatId::MessageId).toBase32();

  QNetworkRequest request(QUrl(url + LS("/providers?state=") + m_state));

  m_reply = m_manager->get(request);
  connect(m_reply, SIGNAL(finished()), SLOT(providersReady()));
  connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), SLOT(sslErrors()));
}


void AuthClient::timerEvent(QTimerEvent *event)
{
  if (event->timerId() == m_timer->timerId()) {
    m_timer->stop();
    if (m_reply)
      m_reply->abort();
  }

  QObject::timerEvent(event);
}


void AuthClient::clientStateChanged(int state)
{
  if (state == ChatClient::Online)
    cancel();
}


/*!
 * Запрос состояния авторизации.
 *
 * Используется HTTP запрос вида {m_url}/state/{m_state}.
 */
void AuthClient::getState()
{
  if (m_retry == 13) {
    emit timeout();
    return;
  }

  QNetworkRequest request(QUrl(m_url + LS("/state/") + m_state + LS("?retry=") + QString::number(m_retry)));
  request.setRawHeader("X-SChat-Secret", m_secret);

  m_retry++;
  m_reply = m_manager->get(request);
  connect(m_reply, SIGNAL(finished()), SLOT(stateReady()));
  connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), SLOT(sslErrors()));

  m_timer->start(25000, this);
}


/*!
 * Слот вызывается тогда когда получен список провайдеров.
 */
void AuthClient::providersReady()
{
  if (m_reply->error())
    return setError(m_reply->error());

  const QVariantMap data = JSON::parse(m_reply->readAll()).toMap();
  if (data.isEmpty())
    return setError(QNetworkReply::UnknownContentError);

  deleteReply();
  emit providersReady(data);

  QTimer::singleShot(0, this, SLOT(getState()));
}


void AuthClient::sslErrors()
{
  m_reply->ignoreSslErrors();
}


/*!
 * Получение ответа на запрос состояния.
 */
void AuthClient::stateReady()
{
  int error = m_reply->error();
  if (error) {
    deleteReply();

    if (error == QNetworkReply::ContentOperationNotPermittedError)
      emit forbidden();
    else
      invalidState();

    return;
  }

  const QVariantMap data  = JSON::parse(m_reply->readAll()).toMap();
  deleteReply();

  if (data.isEmpty())
    return invalidState();

  const QByteArray id     = SimpleID::decode(data.value(LS("id")).toByteArray());
  const QByteArray cookie = SimpleID::decode(data.value(LS("cookie")).toByteArray());
  const QString provider  = data.value(LS("provider")).toString();

  if (SimpleID::typeOf(id) != SimpleID::UserId || SimpleID::typeOf(cookie) != SimpleID::CookieId || provider.isEmpty())
    return invalidState();

  emit ready(provider, id, cookie, data);
}


void AuthClient::setError(int errorCode)
{
  deleteReply();
  emit error(errorCode);
}


void AuthClient::deleteReply()
{
  if (!m_reply)
    return;

  m_reply->deleteLater();
  m_reply = 0;
}


void AuthClient::invalidState()
{
  QTimer::singleShot(500, this, SLOT(getState()));
}
