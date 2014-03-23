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

#include <QDir>
#include <QHostInfo>

#include "Account.h"
#include "Channel.h"
#include "DateTime.h"
#include "feeds/FeedStorage.h"
#include "net/SimpleID.h"
#include "sglobal.h"
#include "text/HtmlFilter.h"

/*!
 * Добавление идентификатора в список каналов.
 */
bool Channels::add(const QByteArray &id)
{
  if (!Channel::isCompatibleId(id))
    return false;

  if (m_channels.contains(id))
    return false;

  m_channels.append(id);
  return true;
}


QByteArray Channels::join() const
{
  if (m_channels.isEmpty())
    return QByteArray();

  QByteArray out;
  out.reserve(m_channels.size() * SimpleID::DefaultSize);

  foreach (const QByteArray &id, m_channels)
    out.append(id);

  return out;
}


QList<QByteArray> Channels::all(int type) const
{
  QList<QByteArray> out;
  foreach (const QByteArray &id, m_channels) {
    if (SimpleID::typeOf(id) == type)
      out.append(id);
  }

  return out;
}


void Channels::restore(const QByteArray &channels)
{
  if (channels.size() > SimpleID::DefaultSize) {
    const int size = channels.size() / SimpleID::DefaultSize;
#   if QT_VERSION >= 0x040700
    m_channels.reserve(size * SimpleID::DefaultSize);
#   endif
    for (int i = 0; i < size; ++i) {
      add(channels.mid(i * SimpleID::DefaultSize, SimpleID::DefaultSize));
    }
  }
  else
    add(channels);
}


Channels& Channels::operator+=(const QByteArray &id)
{
  add(id);
  return *this;
}


Channels& Channels::operator=(const QList<QByteArray> &channels)
{
  set(channels);
  return *this;
}



Channel::Channel()
  : m_account(0)
  , m_synced(false)
  , m_type(SimpleID::InvalidId)
  , m_date(0)
  , m_key(0)
{
}


Channel::Channel(const QByteArray &id, const QString &name)
  : m_account(0)
  , m_synced(false)
  , m_type(SimpleID::InvalidId)
  , m_date(0)
  , m_key(0)
{
  setId(id);
  setName(name);
}


Channel::~Channel()
{
  if (m_account)
    delete m_account;
}


bool Channel::isValid() const
{
  if (m_name.isEmpty())
    return false;

  if (m_id.isEmpty())
    return false;

  return true;
}


/*!
 * Установка идентификатора канала.
 */
bool Channel::setId(const QByteArray &id)
{
  const int type = Channel::isCompatibleId(id);
  if (type == 0)
    return false;

  m_id = id;
  m_type = type;
  m_feeds.setChannel(this);
  return true;
}


/*!
 * Установка имени канала.
 *
 * \param name Имя канала.
 * \return \b true если имя было установлено.
 */
bool Channel::setName(const QString &name)
{
  if (name.isEmpty() || name == LS("*")) {
    if (type() == SimpleID::ServerId) {
      m_name = LS("*");
      return true;
    }

    return false;
  }

  const QString tmp = name.simplified().left(MaxNameLength);
  if (tmp.size() < MinNameLength)
    return false;

  m_name = tmp;
  return true;
}


void Channel::setAccount(Account *account)
{
  if (!m_account)
    m_account = new Account();

  if (account)
    *m_account = *account;
}


void Channel::setDate(qint64 date)
{
  if (!date)
    date = DateTime::utc();

  m_date = date;
}


/*!
 * Проверка имени канала на корректность.
 */
bool Channel::isValidName(const QString &name)
{
  const int size = name.simplified().size();
  if (size > MaxNameLength || size < MinNameLength)
    return false;

  return true;
}


/*!
 * Проверка идентификатора на принадлежность к допустимому типу.
 *
 * \param id Проверяемый идентификатор.
 * \return Тип идентификатора или 0 если идентификатор не совместим или не корректен.
 */
int Channel::isCompatibleId(const QByteArray &id)
{
  const int type = SimpleID::typeOf(id);
  if (type == SimpleID::ChannelId || type == SimpleID::UserId || type == SimpleID::ServerId)
    return type;

  return 0;
}


QString Channel::defaultName()
{
  return QDir::home().dirName();
}


QString Channel::defaultName(const QString &tpl)
{
  QString out = tpl;

  if (out.contains(LS("%USER%")))
    out.replace(LS("%USER%"), QDir::home().dirName());

  if (out.contains(LS("%HOST%")))
    out.replace(LS("%HOST%"), QHostInfo::localHostName());

  return out;
}


/*!
 * Получение фида по имени, эта функция позволяет автоматически создать фид, если он не существует.
 *
 * \param name   Имя фида.
 * \param create \b true если необходимо создать фид, если он не существует.
 * \param save   \b true если необходимо сохранить фид после создания.
 */
FeedPtr Channel::feed(const QString &name, bool create, bool save)
{
  FeedPtr feed = feeds().all().value(name);
  if (feed || !create)
    return feed;

  feeds().add(FeedStorage::create(name), save);
  return this->feed(name, false);
}
