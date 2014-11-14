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

#ifndef AUTHSTATE_H_
#define AUTHSTATE_H_

#include <QHash>
#include <QObject>
#include <QSharedPointer>
#include <QVariant>

#include "User.h"

class QNetworkAccessManager;

class AuthStateData
{
public:
  AuthStateData(const QByteArray &state, const QByteArray &error);
  AuthStateData(const QByteArray &state, const QByteArray &provider, const QByteArray &id, const QVariantMap &raw, const User &user = User());

  QByteArray cookie;   ///< Авторизационный токен пользователя.
  QByteArray error;    ///< Текстовый код ошибки.
  QByteArray id;       ///< Глобальный идентификатор пользователя.
  QByteArray provider; ///< Кодовое имя OAuth2 провайдера.
  QByteArray state;    ///< Идентификатор состояния.
  QVariantMap raw;     ///< Сырые не обработанные данные пользователя.
  User user;           ///< Профиль пользователя.
};

typedef QSharedPointer<AuthStateData> AuthStatePtr;


class AuthState : public QObject
{
  Q_OBJECT

public:
  AuthState(QObject *parent = 0);
  inline AuthStatePtr get(const QByteArray &state) const { return m_states.value(state); }
  inline void setToken(const QString &token)             { m_token = token.toLatin1(); }
  inline void setUrl(const QString &url)                 { m_url = url; }
  void add(AuthStateData *data);

signals:
  void added(const QByteArray &state, AuthStatePtr data);

private slots:
  void onFinished();

private:
  void addState(AuthStatePtr state);

  QByteArray m_token;
  QMap<QByteArray, AuthStatePtr> m_pending;
  QMap<QByteArray, AuthStatePtr> m_states;  ///< Таблица состояний.
  QNetworkAccessManager *m_net;
  QString m_url;
};

#endif /* AUTHSTATE_H_ */
