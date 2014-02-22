/* $Id: AuthHandler.h 2850 2012-07-09 03:06:13Z IMPOMEZIA $
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

#ifndef MAINHANDLER_H_
#define MAINHANDLER_H_

#include <QObject>

namespace Tufao {
  class HttpServerRequest;
  class HttpServerResponse;
}

class HandlerRoute;
class QUrl;

class AuthHandler : public QObject
{
  Q_OBJECT

public:
  AuthHandler(QObject *parent = 0);
  ~AuthHandler();

  inline QString root() const              { return m_root; }
  inline void setRoot(const QString &root) { m_root = root; }
  static void setError(Tufao::HttpServerResponse *response, int statusCode);

public slots:
  void handleRequest(Tufao::HttpServerRequest *request, Tufao::HttpServerResponse *response);

private:
  bool serve(const QUrl &url, const QString &path, Tufao::HttpServerRequest *request, Tufao::HttpServerResponse *response);
  void serveFile(const QString &fileName, Tufao::HttpServerRequest *request, Tufao::HttpServerResponse *response);

  HandlerRoute *m_route; ///< Список обработчиков.
  QString m_root;        ///< Корневая директория для файлов.
};

#endif /* MAINHANDLER_H_ */
