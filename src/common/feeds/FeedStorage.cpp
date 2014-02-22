/* $Id: FeedStorage.cpp 3420 2013-01-19 05:23:36Z IMPOMEZIA $
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

#include "DateTime.h"
#include "feeds/FeedStorage.h"
#include "net/packets/Notice.h"
#include "feeds/FeedCreator.h"

FeedStorage *FeedStorage::m_self = 0;
QMap<QString, FeedCreator*> FeedStorage::m_feeds;

FeedStorage::FeedStorage(QObject *parent)
  : QObject(parent)
{
  if (!m_self)
    m_self = this;
  else
    add(this);
}


FeedStorage::~FeedStorage()
{
  if (m_self == this) {
    m_self = 0;
    qDeleteAll(m_feeds);
    m_feeds.clear();
  }
}


/*!
 * Создание фида по имени.
 */
Feed* FeedStorage::create(const QString &name)
{
  if (!m_feeds.contains(name))
    return new Feed(name, DateTime::utc());

  return m_feeds.value(name)->create(name);
}


/*!
 * Восстановление фида из JSON данных.
 */
Feed* FeedStorage::load(const QString &name, const QVariantMap &data)
{
  if (!m_feeds.contains(name))
    return new Feed(name, data);

  return m_feeds.value(name)->load(name, data);
}


/*!
 * Сохранение фида.
 *
 * \param feed Фид.
 * \param date Дата модификации фида, если 0 будет использована текущая дата.
 *
 * \return Notice::OK если фид сохранён успешно.
 */
int FeedStorage::save(FeedPtr feed, qint64 date)
{
  int status = Notice::OK;
  if (date == 0)
    date = DateTime::utc();

  foreach (FeedStorage *hook, m_self->m_hooks) {
    status = hook->saveImpl(feed, date);
    if (status != Notice::OK)
      return status;
  }

  return status;
}


void FeedStorage::add(FeedCreator *creator)
{
  if (!creator)
    return;

  const QString name = creator->name();

  if (m_feeds.contains(name)) {
    FeedCreator *exists = m_feeds.value(name);
    delete exists;
    m_feeds.remove(name);
  }

  m_feeds[name] = creator;
}


int FeedStorage::revertImpl(FeedPtr feed, const QVariantMap &data)
{
  if (m_self != this)
    return Notice::OK;

  foreach (FeedStorage *hook, m_hooks) {
    int status = hook->revertImpl(feed, data);
    if (status != Notice::OK)
      return status;
  }

  return Notice::OK;
}


int FeedStorage::saveImpl(FeedPtr /*feed*/, qint64 /*date*/)
{
  return Notice::OK;
}


void FeedStorage::cloneImpl(FeedPtr feed)
{
  if (m_self != this)
    return;

  foreach (FeedStorage *hook, m_hooks) {
    hook->cloneImpl(feed);
  }
}


void FeedStorage::loadImpl(Channel *channel)
{
  if (m_self != this)
    return;

  foreach (FeedStorage *hook, m_hooks) {
    hook->loadImpl(channel);
  }
}


void FeedStorage::removeImpl(FeedPtr feed)
{
  if (m_self != this)
    return;

  foreach (FeedStorage *hook, m_hooks) {
    hook->removeImpl(feed);
  }
}
