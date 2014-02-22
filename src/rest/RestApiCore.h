/* $Id: RestApiCore.h 2992 2012-08-11 09:55:11Z IMPOMEZIA $
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

#ifndef RESTAPICORE_H_
#define RESTAPICORE_H_

#include <QObject>

#include "schat.h"

namespace Tufao {
  class HttpServer;
  class HttpServerRequest;
  class HttpServerResponse;
}

class Settings;
class QUrl;
class RestHandler;

class SCHAT_REST_EXPORT RestApiCore : public QObject
{
  Q_OBJECT

public:
  RestApiCore(QObject *parent = 0);
  ~RestApiCore();

  inline static RestApiCore *i() { return m_self; }
  static bool add(const QUrl &url);
  static bool add(RestHandler *handler);

private slots:
  void start();
  void handleRequest(Tufao::HttpServerRequest *request, Tufao::HttpServerResponse *response);

private:
  int m_prefixSize;                     ///< Длина префикса пути.
  QList<RestHandler *> m_handlers;      ///< Обработчики запросов.
  QList<Tufao::HttpServer *> m_servers; ///< HTTP или HTTPS сервера ожидающие подключений.
  Settings *m_settings;                 ///< Настройки сервера.
  static RestApiCore *m_self;           ///< Указатель на себя.
};

#endif /* RESTAPICORE_H_ */
