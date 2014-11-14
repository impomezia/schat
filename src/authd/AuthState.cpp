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
#include <QNetworkReply>
#include <QNetworkRequest>

#include "AuthState.h"
#include "id/ChatId.h"
#include "JSON.h"

AuthStateData::AuthStateData(const QByteArray &state, const QByteArray &error)
  : error(error)
  , state(state)
{
}


AuthStateData::AuthStateData(const QByteArray &state, const QByteArray &provider, const QByteArray &id, const QVariantMap &raw, const User &user)
  : id(id)
  , provider(provider)
  , state(state)
  , raw(raw)
  , user(user)
{
  cookie = ChatId(ChatId::CookieId).toBase32();
}


AuthState::AuthState(QObject *parent)
  : QObject(parent)
{
  m_net = new QNetworkAccessManager(this);
}


void AuthState::add(AuthStateData *data)
{
  AuthStatePtr state = AuthStatePtr(data);

  if (!state->error.isEmpty())
    return addState(state);

  if (!m_token.isEmpty()) {
    m_pending.insert(state->state, state);

    QNetworkRequest request(m_url + "/" + state->id);
    request.setRawHeader("Content-Type",  "application/json");
    request.setRawHeader("Authorization", "Bearer " + m_token);

    QNetworkReply *reply = m_net->post(request, JSON::generate(state->user.toMap()));
    reply->setProperty("id", state->state);

    connect(reply, SIGNAL(finished()), SLOT(onFinished()));
  }
  else {
    addState(state);
  }
}


void AuthState::onFinished()
{
  QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
  if (!reply)
    return;

  AuthStatePtr state = m_pending.take(reply->property("id").toByteArray());
  if (!state) {
    reply->deleteLater();
    return;
  }

  const int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

  if (reply->error() || status != 200) {
    state->error = reply->errorString().toUtf8();
  }

  addState(state);
  reply->deleteLater();
}


void AuthState::addState(AuthStatePtr state)
{
  m_states[state->state] = state;
  emit added(state->state, state);
}
