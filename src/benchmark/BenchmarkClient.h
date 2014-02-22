/* $Id: BenchmarkClient.h 2131 2012-01-11 14:24:20Z IMPOMEZIA $
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

#ifndef BENCHMARKCLIENT_H_
#define BENCHMARKCLIENT_H_

#include <QObject>
#include <QVariant>

class SimpleClient;

class BenchmarkClient : public QObject
{
  Q_OBJECT

public:
  BenchmarkClient(const QString &nick, QObject *parent = 0);
  BenchmarkClient(const QVariantMap &auth, QObject *parent = 0);
  bool open(const QString &url);
  inline SimpleClient *io() { return m_client; }

private slots:
  void setup();

private:
  QByteArray m_cookie;    ///< Cookie;
  SimpleClient *m_client; ///< Клиент чата.
};

#endif /* BENCHMARKCLIENT_H_ */
