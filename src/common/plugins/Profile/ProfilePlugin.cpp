/* $Id: ProfilePlugin.cpp 3741 2013-07-09 23:49:56Z IMPOMEZIA $
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

#include <QApplication>
#include <QFile>
#include <QtPlugin>

#include "ChatCore.h"
#include "ChatNotify.h"
#include "client/ChatClient.h"
#include "client/ClientChannels.h"
#include "client/ClientFeeds.h"
#include "CountryField.h"
#include "JSON.h"
#include "Profile.h"
#include "ProfileChatView.h"
#include "ProfilePlugin.h"
#include "ProfilePlugin_p.h"
#include "sglobal.h"
#include "Tr.h"
#include "Translation.h"

QString ProfilePluginImpl::Countries::m_lang;
QVariantMap ProfilePluginImpl::Countries::m_cache;


void ProfilePluginImpl::Countries::reload()
{
  const QString lang = ChatCore::translation()->name().left(2);
  if (m_lang != lang) {
    QFile file(LS(":/json/Profile/countries_") + lang + LS(".json"));
    if (!file.open(QFile::ReadOnly)) {
      file.setFileName(LS(":/json/Profile/countries_en.json"));
      file.open(QFile::ReadOnly);
    }

    m_cache = JSON::parse(file.readAll()).toMap();
    m_lang  = lang;
  }
}


class ProfilePluginTr : public Tr
{
  Q_DECLARE_TR_FUNCTIONS(ProfilePluginTr)

public:
  ProfilePluginTr() : Tr() { m_prefix = LS("field-"); }

protected:
  QString valueImpl(const QString &key) const
  {
    if (key == LS("country"))    return tr("Country");
    else if (key == LS("city"))  return tr("City");
    else if (key == LS("site"))  return tr("Site");
    else if (key == LS("email")) return tr("E-Mail");
    else if (key == LS("isp"))   return tr("ISP");
    else if (key == LS("tz"))    return tr("Time");
    return QString();
  }
};


class CountryTr : public Tr
{
  Q_DECLARE_TR_FUNCTIONS(CountryTr)

public:
  CountryTr() : Tr() { m_prefix = LS("country-"); }

protected:
  QString valueImpl(const QString &key) const
  {
    if (key.size() != 2)
      return QString();

    return ProfilePluginImpl::Countries::name(key);
  }
};


ProfilePluginImpl::ProfilePluginImpl(QObject *parent)
  : ChatPlugin(parent)
{
  m_tr = new ProfilePluginTr();
  m_country = new CountryTr();
  new ProfileChatView(this);

  Profile::addField(LS("country"), 1900);
  Profile::addField(LS("city"),    2000);
  Profile::addField(LS("site"),    4000);
  Profile::addField(LS("email"),   4050);

  ProfileFieldFactory::add(new CountryFieldCreator());

  ChatCore::translation()->addOther(LS("profile"));

  connect(ChatNotify::i(), SIGNAL(notify(Notify)), SLOT(notify(Notify)));
  connect(ChatClient::i(), SIGNAL(ready()), SLOT(ready()));
}


ProfilePluginImpl::~ProfilePluginImpl()
{
  delete m_tr;
  delete m_country;
}


/*!
 * Обновление информации об группах пользователя.
 */
void ProfilePluginImpl::notify(const Notify &notify)
{
  if (notify.type() == Notify::FeedData) {
    const FeedNotify &n = static_cast<const FeedNotify &>(notify);
    if (n.feed() != FEED_NAME_USER)
      return;

    ClientChannel user = ChatClient::channels()->get(n.channel());
    if (!user)
      return;

    FeedPtr feed = user->feed(FEED_NAME_USER, false);
    if (!feed)
      return;
  }
}


void ProfilePluginImpl::ready()
{
  FeedPtr feed = ChatClient::channel()->feed(FEED_NAME_USER, false);
  if (!feed)
    ClientFeeds::request(ChatClient::id(), FEED_METHOD_GET, FEED_NAME_USER);
}


ChatPlugin *ProfilePlugin::create()
{
  m_plugin = new ProfilePluginImpl(this);
  return m_plugin;
}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2(Profile, ProfilePlugin);
#endif
