/* $Id: AuthClient.h 3652 2013-04-21 20:25:14Z IMPOMEZIA $
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

#ifndef AUTHCLIENT_H_
#define AUTHCLIENT_H_

#include <QObject>
#include <QVariant>

#include "schat.h"

class QBasicTimer;
class QNetworkAccessManager;
class QNetworkReply;
class QSslError;

class SCHAT_EXPORT AuthClient : public QObject
{
  Q_OBJECT

public:
  AuthClient(QObject *parent = 0);
  ~AuthClient();
  inline const QByteArray& state() const { return m_state; }
  void cancel();
  void start(const QString &url);

signals:
  void error(int errorCode);
  void forbidden();
  void providersReady(const QVariantMap &data);
  void ready(const QString &provider, const QByteArray &id, const QByteArray &cookie, const QVariantMap &data);
  void timeout();

protected:
  void timerEvent(QTimerEvent *event);

private slots:
  void clientStateChanged(int state);
  void getState();
  void providersReady();
  void sslErrors();
  void stateReady();

private:
  void deleteReply();
  void invalidState();
  void setError(int errorCode);

  int m_retry;                      ///< Счётчик запросов состояния.
  QBasicTimer *m_timer;             ///< Таймер для прерывания запроса.
  QByteArray m_secret;              ///< Base32 кодированный секретный идентификатор.
  QByteArray m_state;               ///< Base32 кодированный идентификатор состояния.
  QNetworkAccessManager *m_manager; ///< Менеджер доступа к сети.
  QNetworkReply *m_reply;           ///< Текущий HTTP запрос.
  QString m_url;                    ///< Адрес сервера.
};

#endif /* AUTHCLIENT_H_ */
