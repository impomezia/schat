/* $Id: ChatDNS.cpp 2559 2012-04-20 13:48:38Z IMPOMEZIA $
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

#include <QHostAddress>
#include <QHostInfo>
#include <QStringList>

#include "net/dns/ChatDNS.h"
#include "net/Protocol.h"
#include "sglobal.h"

#if !defined(SCHAT_NO_QDNS)
# include "net/dns/qdnslookup.h"
#endif

ChatDNS::ChatDNS(QObject *parent)
  : QObject(parent)
{
# if !defined(SCHAT_NO_QDNS)
  m_dns = new QDnsLookup(this);
  m_dns->setType(QDnsLookup::SRV);
  connect(m_dns, SIGNAL(finished()), SLOT(handleServers()));
# endif
}


void ChatDNS::open(const QUrl &url)
{
  m_url = url;
  m_a.clear();
  m_srv.clear();

  // Для ip адреса нет необходимости в DNS запросах.
  if (!QHostAddress(url.host()).isNull()) {
    m_a[url] = url;
    done();
    return;
  }

  // Если указан порт нет необходимости в получении SRV записей.
  if (url.port() != -1) {
    m_srv.append(url);
    a();
    return;
  }

# if defined(SCHAT_NO_QDNS)
  m_url.setPort(Protocol::DefaultPort);
  m_srv.append(m_url);
# endif

  srv();
}


void ChatDNS::handleServers()
{
# if !defined(SCHAT_NO_QDNS)
  if (m_dns->type() == QDnsLookup::SRV) {
    if (m_dns->error() == QDnsLookup::NoError) {
      foreach (const QDnsServiceRecord &record, m_dns->serviceRecords()) {
        QUrl url = m_url;
        url.setHost(record.target());
        url.setPort(record.port());
        m_srv.append(url);
      }
    }
    else {
      QUrl url = m_url;
      url.setPort(Protocol::DefaultPort);
      m_srv.append(url);
    }

    a();
  }
# endif
}


/*!
 * Завершение получения информации об A записях.
 */
void ChatDNS::lookedUp(const QHostInfo &host)
{
  foreach (const QHostAddress &address, host.addresses()) {
    QUrl url = m_current;
    url.setHost(address.toString());
    m_a[url] = m_current;
  }

  a();
}


void ChatDNS::a()
{
  if (m_srv.isEmpty()) {
    done();
    return;
  }

  m_current = m_srv.takeFirst();
  QHostInfo::lookupHost(m_current.host(), this, SLOT(lookedUp(QHostInfo)));
}


void ChatDNS::done()
{
  if (m_a.isEmpty())
    failback();
  else
    store();

  emit finished();
}


void ChatDNS::srv()
{
# if !defined(SCHAT_NO_QDNS)
  m_dns->setName(LS("_schat._tcp.") + m_url.host());
  m_dns->lookup();
# else
  a();
# endif
}


/*!
 * Преобразование текущего адреса в ключ в кэше.
 */
QString ChatDNS::toKey() const
{
  if (!QHostAddress(m_url.host()).isNull())
    return QString();

  QString out = m_url.host();
  if (m_url.port() != -1)
    out += LS(":") + QString::number(m_url.port());

  return out;
}


void ChatDNS::failback()
{
  QString key = toKey();
  if (key.isEmpty())
    return;

  if (!m_cache.contains(key))
    return;

  QVariantList list = m_cache.value(key).toList();
  if (list.isEmpty())
    return;

  for (int i = 0; i < list.size(); ++i) {
    QStringList record = list.at(i).toString().split(':');
    if (record.size() != 2)
      continue;

    QUrl url = m_url;
    url.setHost(record.at(0));
    url.setPort(record.at(1).toInt());
    if (url.isValid())
      m_a[url] = url;
  }
}


void ChatDNS::store()
{
  QString key = toKey();
  if (key.isEmpty())
    return;

  QVariantList map;
  QMapIterator<QUrl, QUrl> i(m_a);
  while (i.hasNext()) {
    i.next();
    map.append(i.key().host() + LS(":") + QString::number(i.key().port(Protocol::DefaultPort)));
  }

  m_cache[key] = map;
}
