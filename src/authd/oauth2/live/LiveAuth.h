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

#ifndef LIVEAUTH_H_
#define LIVEAUTH_H_

#include "oauth2/OAuthHandler.h"

class LiveAuth : public OAuthHandler
{
  Q_OBJECT

public:
  LiveAuth(const QUrl &url, const QString &path, Tufao::HttpServerRequest *request, Tufao::HttpServerResponse *response, const QString &successRedirect, QObject *parent = 0);

private:
  void dataReady() Q_DECL_OVERRIDE;
  void tokenReady() Q_DECL_OVERRIDE;
  void getToken() Q_DECL_OVERRIDE;
};


class LiveAuthCreator : public HandlerCreator
{
public:
  LiveAuthCreator() : HandlerCreator() {}
  bool serve(const QUrl &url, const QString &path, Tufao::HttpServerRequest *request, Tufao::HttpServerResponse *response, QObject *parent);
};

#endif /* LIVEAUTH_H_ */
