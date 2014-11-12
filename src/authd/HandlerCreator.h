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

#ifndef HANDLERCREATOR_H_
#define HANDLERCREATOR_H_

#include <QObject>

namespace Tufao {
  class HttpServerRequest;
  class HttpServerResponse;
}

class QUrl;

class HandlerCreator
{
public:
  HandlerCreator() {}
  virtual ~HandlerCreator() {}
  inline void setSuccessRedirect(const QString &url) { m_successRedirect = url; }
  virtual bool serve(const QUrl &url, const QString &path, Tufao::HttpServerRequest *request, Tufao::HttpServerResponse *response, QObject *parent) = 0;

protected:
  QString m_successRedirect;
};

#endif /* HANDLERCREATOR_H_ */
