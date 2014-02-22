/* $Id: StateCache.h 3693 2013-06-14 19:12:11Z IMPOMEZIA $
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

#ifndef STATECACHE_H_
#define STATECACHE_H_

#include <QObject>
#include <QStringList>

class AbstractTab;
class ChatSettings;

/*!
 * Обеспечивает восстановление закреплённых вкладок после перезапуска клиента.
 */
class StateCache : public QObject
{
  Q_OBJECT

public:
  StateCache(QObject *parent = 0);

private slots:
  void pinned(AbstractTab *tab);
  void ready();
  void save();
  void settingsChanged(const QString &key, const QVariant &value);
  void start();
  void synced();
  void tabIndexChanged(int index);
  void unpinned(AbstractTab *tab);

private:
  bool restoreLastTalk();
  QByteArray decode(const QString &id) const;
  QString encode(const QByteArray &id) const;
  void join(const QByteArray &id);

  ChatSettings *m_settings; ///< Настройки.
  const QString m_key;      ///< Ключ настроек.
  QString m_prefix;         ///< Префикс специфичных для данного сервера локальных настроек.
  QStringList m_tabs;       ///< Список закреплённых вкладок.
};

#endif /* STATECACHE_H_ */
