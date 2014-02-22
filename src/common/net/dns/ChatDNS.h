/* $Id: ChatDNS.h 2555 2012-04-16 19:08:26Z IMPOMEZIA $
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

#ifndef CHATDNS_H_
#define CHATDNS_H_

#include <QObject>
#include <QUrl>
#include <QVariantMap>

#include "schat.h"

class QDnsLookup;
class QHostInfo;

class SCHAT_EXPORT ChatDNS : public QObject
{
  Q_OBJECT

public:
  ChatDNS(QObject *parent = 0);
  inline const QUrl& url() const                 { return m_url; }
  inline const QVariantMap& cache() const        { return m_cache; }
  inline QList<QUrl> urls() const                { return m_a.keys(); }
  inline void setCache(const QVariantMap &cache) { m_cache = cache; }
  void open(const QUrl &url);

signals:
  void finished();

private slots:
  void handleServers();
  void lookedUp(const QHostInfo &host);

private:
  void a();
  void done();
  void srv();

  QString toKey() const;
  void failback();
  void store();

  QDnsLookup *m_dns;    ///< Объект для DNS запросов.
  QList<QUrl> m_srv;    ///< SRV записи.
  QMap<QUrl, QUrl> m_a; ///< A записи.
  QUrl m_current;       ///< Текущий адрес.
  QUrl m_url;           ///< Основной URL.
  QVariantMap m_cache;  ///< Кэш успешних запросов.
};

#endif /* CHATDNS_H_ */
