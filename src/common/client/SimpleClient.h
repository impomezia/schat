/* $Id: SimpleClient.h 3496 2013-02-12 05:44:20Z IMPOMEZIA $
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

#ifndef SIMPLECLIENT_H_
#define SIMPLECLIENT_H_

#include "client/AbstractClient.h"

class AbstractNotice;
class SimpleClientPrivate;

#define CLIENT_PROP_ANONYMOUS     QLatin1String("anonymous")
#define CLIENT_PROP_AUTH_SERVER   QLatin1String("authServer")
#define CLIENT_PROP_CHANNEL       QLatin1String("channel")
#define CLIENT_PROP_ERROR         QLatin1String("error")
#define CLIENT_PROP_ERROR_AUTH    QLatin1String("auth")
#define CLIENT_PROP_ERROR_DATA    QLatin1String("data")
#define CLIENT_PROP_ERROR_STATUS  QLatin1String("status")
#define CLIENT_PROP_ERROR_TYPE    QLatin1String("type")
#define CLIENT_PROP_HOST          QLatin1String("host")
#define CLIENT_PROP_HOST_ID       QLatin1String("hostId")
#define CLIENT_PROP_ID            QLatin1String("id")
#define CLIENT_PROP_POLICY        QLatin1String("policy")

class SCHAT_EXPORT SimpleClient : public AbstractClient
{
  Q_OBJECT

public:
  explicit SimpleClient(QObject *parent = 0);
  ~SimpleClient();
  void leave();
  void setAuthType(int authType);

signals:
  void notice(int type);

protected:
  SimpleClient(SimpleClientPrivate &dd, QObject *parent);

protected slots:
  void requestAuth();

protected:
  void newPacketsImpl();

private:
  Q_DECLARE_PRIVATE(SimpleClient);
};

#endif /* SIMPLECLIENT_H_ */
