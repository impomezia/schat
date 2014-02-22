/* $Id: StateHandler.h 2906 2012-07-23 15:50:48Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2012 IMPOMEZIA <schat@impomezia.com>
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

#ifndef STATEHANDLER_H_
#define STATEHANDLER_H_

#include "AuthState.h"
#include "HandlerCreator.h"

class StateHandler : public QObject
{
  Q_OBJECT

public:
  StateHandler(const QByteArray &cookie, const QByteArray &state, const QByteArray &secret, Tufao::HttpServerResponse *response);
  static void serve(const QByteArray &cookie, const QByteArray &secret, Tufao::HttpServerResponse *response, AuthStatePtr data);

private slots:
  void added(const QByteArray &state, AuthStatePtr data);

private:
  QByteArray m_cookie;                   ///< Base32 кодированный идентификатор cookie.
  QByteArray m_secret;                   ///< Base32 кодированный секретный идентификатор.
  QByteArray m_state;                    ///< Base32 кодированный идентификатор состояния.
  Tufao::HttpServerResponse *m_response; ///< Ответ на запрос.
};


/*!
 * Запрос "/state/идентификатор состояния".
 */
class StateHandlerCreator : public HandlerCreator
{
public:
  StateHandlerCreator() : HandlerCreator() {}
  bool serve(const QUrl &url, const QString &path, Tufao::HttpServerRequest *request, Tufao::HttpServerResponse *response, QObject *parent);
};

#endif /* STATEHANDLER_H_ */
