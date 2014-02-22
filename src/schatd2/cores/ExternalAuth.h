/* $Id: ExternalAuth.h 3496 2013-02-12 05:44:20Z IMPOMEZIA $
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

#ifndef EXTERNALAUTH_H_
#define EXTERNALAUTH_H_

#include "cores/AnonymousAuth.h"
#include "net/packets/auth.h"

class QBasicTimer;
class QNetworkAccessManager;
class QNetworkReply;

class ExternalAuth : public AnonymousAuth
{
public:
  ExternalAuth(Core *core);
  AuthResult auth(const AuthRequest &data);
  int type() const;
};


class ExternalAuthTask : public QObject
{
  Q_OBJECT

public:
  ExternalAuthTask(const AuthRequest &data, const QString &host, QObject *parent = 0);
  ~ExternalAuthTask();

protected:
  void timerEvent(QTimerEvent *event);

private slots:
  void ready();
  void sslErrors();
  void start();

private:
  AuthResult auth(const QVariantMap &data);
  void done(const QVariantMap &data);
  void setError(int errorCode);

  AuthRequest m_data;                            ///< Копия авторизационного запроса.
  QBasicTimer *m_timer;                          ///< Таймер для автоматического удаления объекта.
  QNetworkAccessManager *m_manager;              ///< Менеджер доступа к сети.
  QNetworkReply *m_reply;                        ///< Текущий HTTP запрос.
  QString m_host;                                ///< IP адрес пользователя.
  quint64 m_socket;                              ///< Сокет пользователя.
  static QMap<QByteArray, QVariantMap> m_cache; ///< Кэшированные ответы от сервера авторизации, для того чтобы не запрашивать информацию с сервера при разрешении коллизий ников.
};

#endif /* EXTERNALAUTH_H_ */
