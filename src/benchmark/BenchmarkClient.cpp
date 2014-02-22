/* $Id: BenchmarkClient.cpp 3246 2012-11-01 07:23:25Z IMPOMEZIA $
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

#include "BenchmarkClient.h"
#include "client/SimpleClient.h"
#include "net/packets/auth.h"
#include "net/packets/ChannelNotice.h"
#include "net/SimpleID.h"
#include "sglobal.h"

BenchmarkClient::BenchmarkClient(const QString &nick, QObject *parent)
  : QObject(parent)
{
  m_client = new SimpleClient(this);
  m_client->setNick(nick);
  m_client->setAuthType(AuthRequest::Anonymous);
  m_client->setUniqueId(SimpleID::randomId(SimpleID::UniqueUserId));

  connect(m_client, SIGNAL(setup()), SLOT(setup()));
}


BenchmarkClient::BenchmarkClient(const QVariantMap &auth, QObject *parent)
: QObject(parent)
{
  m_client = new SimpleClient(this);
  m_client->setNick(auth.value("nick").toString());
  m_client->setAuthType(AuthRequest::Cookie);
  m_client->setUniqueId(SimpleID::decode(auth.value("uniqueId").toByteArray()));

  m_cookie = SimpleID::decode(auth.value("cookie").toByteArray());

  connect(m_client, SIGNAL(setup()), SLOT(setup()));
}


bool BenchmarkClient::open(const QString &url)
{
  return m_client->openUrl(url, m_cookie);
}


void BenchmarkClient::setup()
{
  m_client->send(ChannelNotice::request(m_client->channelId(), QByteArray(), LS("join"), LS("Main")));
}
