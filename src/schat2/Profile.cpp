/* $Id: Profile.cpp 3416 2013-01-18 03:48:52Z IMPOMEZIA $
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

#include <QDebug>

#include <QApplication>

#include "client/ChatClient.h"
#include "client/ClientFeeds.h"
#include "Profile.h"
#include "sglobal.h"
#include "Tr.h"

QMap<int, QString> Profile::m_fields;
Profile *Profile::m_self = 0;

class ProfileTr : public Tr
{
  Q_DECLARE_TR_FUNCTIONS(ProfileTr)

public:
  ProfileTr() : Tr() { m_prefix = LS("field-"); }

protected:
  QString valueImpl(const QString &key) const
  {
    if (key == LS("name"))                return tr("Full name");
    else if (key == LS("gender"))         return tr("Gender");
    else if (key == LS("gender-male"))    return tr("Male");
    else if (key == LS("gender-female"))  return tr("Female");
    else if (key == LS("gender-ghost"))   return tr("Ghost");
    else if (key == LS("gender-bot"))     return tr("Bot");
    else if (key == LS("gender-unknown")) return tr("Unknown");

    return QString();
  }
};


Profile::Profile(QObject *parent)
  : QObject(parent)
{
  if (m_self)
    add(this);
  else
    m_self = this;

  m_tr = new ProfileTr();
  addField(LS("name"), 1000);

  connect(ChatClient::i(), SIGNAL(ready()), SLOT(ready()));
}


Profile::~Profile()
{
  if (m_self == this)
    m_self = 0;

  delete m_tr;
}


/*!
 * Перевод имени поля.
 */
QString Profile::translate(const QString &field)
{
  return Tr::value(LS("field-") + field);
}


/*!
 * Возвращает поля доступные для добавления.
 */
QStringList Profile::available()
{
  FeedPtr feed = ChatClient::channel()->feed(FEED_NAME_PROFILE, false);
  if (!feed)
    return QStringList();

  QStringList filled = Profile::filled();
  QStringList out = fields();
  foreach (QString field, filled) {
    out.removeAll(field);
  }

  return out;
}


QStringList Profile::fields()
{
  return m_fields.values();
}


/*!
 * Получение списка заполненных полей.
 */
QStringList Profile::filled()
{
  FeedPtr feed = ChatClient::channel()->feed(FEED_NAME_PROFILE, false);
  if (!feed)
    return QStringList();

  QStringList keys = feed->data().keys();
  keys.removeAll(LS("head"));
  if (keys.isEmpty())
    return QStringList();

  QStringList out;
  for (int i = 0; i < keys.size(); ++i) {
    QVariant data = feed->data().value(keys.at(i));
    if (data.type() == QVariant::String && data != QString())
      out.append(keys.at(i));

    else if (data.type() == QVariant::LongLong && data != 0)
      out.append(keys.at(i));

    else if (data.type() == QVariant::List && data != QVariantList())
      out.append(keys.at(i));

    else if (data.type() == QVariant::Map && data != QVariantMap())
      out.append(keys.at(i));
  }

  return out;
}


void Profile::addField(const QString &name, int weight)
{
  m_fields[weight] = name;
}


QString Profile::translateImpl(const QString & /*field*/) const
{
  return QString();
}


void Profile::ready()
{
  FeedPtr feed = ChatClient::channel()->feed(FEED_NAME_PROFILE, false);
  if (!feed)
    ClientFeeds::request(ChatClient::id(), FEED_METHOD_GET, FEED_NAME_PROFILE);
}
