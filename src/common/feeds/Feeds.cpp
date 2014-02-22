/* $Id: Feeds.cpp 3325 2012-12-16 01:22:27Z IMPOMEZIA $
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

#include <QDebug>
#include "JSON.h"

#include "feeds/Feeds.h"
#include "feeds/FeedStorage.h"
#include "net/packets/Notice.h"
#include "sglobal.h"

/*!
 * Базовая функция добавления нового фида.
 *
 * \param feed Указатель на фид.
 * \param save \b true если необходимо сохранить фид как новую ревизию.
 *
 * \return \b true если фид был добавлен.
 */
bool Feeds::add(FeedPtr feed, bool save)
{
  if (!m_channel)
    return false;

  if (!feed)
    return false;

  feed->setChannel(m_channel);

  if (!feed->isValid())
    return false;

  m_feeds[feed->head().name()] = feed;

  if (save)
    FeedStorage::save(feed);

  return true;
}


/*!
 * Получение компактных заголовков фидов для одиночного пользователя.
 * Такие заголовки содержат только имя и время последней модификации фида.
 *
 * \param channel Канал-пользователь, используется для проверки прав, пользователь не получит список фидов если у него нет прав на чтение.
 */
QVariantMap Feeds::f(Channel *channel) const
{
  QVariantMap json;

  QMapIterator<QString, FeedPtr> i(m_feeds);
  while (i.hasNext()) {
    i.next();
    if (i.value()->can(channel, Acl::Read))
      json[i.key()] = i.value()->head().date();
  }

  return Feed::merge(LS("f"), json);
}


/*!
 * Получение заголовков фидов для одиночного пользователя.
 *
 * \param channel Канал-пользователь, используется для проверки прав, пользователь не получит список фидов если у него нет прав на чтение.
 */
QVariantMap Feeds::headers(Channel *channel) const
{
  QVariantMap json;

  QMapIterator<QString, FeedPtr> i(m_feeds);
  while (i.hasNext()) {
    i.next();
    Feed::merge(i.key(), json, i.value()->head().get(channel));
  }

  return Feed::merge(LS("feeds"), json);
}
