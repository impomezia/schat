/* $Id: ChatSettings.cpp 3770 2013-08-17 15:19:27Z IMPOMEZIA $
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

#include <QFile>

#include "Channel.h"
#include "ChatSettings.h"
#include "client/ChatClient.h"
#include "client/ClientFeeds.h"
#include "client/SimpleClient.h"
#include "sglobal.h"
#include "ChatNotify.h"

ChatSettings::ChatSettings(const QString &fileName, const QString &defaultFile, QObject *parent)
  : Settings(fileName, parent)
  , m_synced(false)
  , m_settings(0)
{
  if (QFile::exists(defaultFile)) {
    m_settings = new QSettings(defaultFile, QSettings::IniFormat, this);
    m_settings->setIniCodec("UTF-8");
  }

  setLocalDefault(SETTINGS_AUTO_CONNECT,          true);
  setLocalDefault(SETTINGS_AUTO_SCROLL,           true);
  setLocalDefault(SETTINGS_DEFAULT_SERVER,        LS("schat://schat.me"));
  setLocalDefault(SETTINGS_LABS_DEVELOPER_EXTRAS, false);
  setLocalDefault(SETTINGS_LABS_DISABLE_UI,       false);
  setLocalDefault(SETTINGS_MAXIMIZED,             false);
  setLocalDefault(SETTINGS_NETWORKS,              QStringList());
  setLocalDefault(SETTINGS_PROFILE_GENDER,        0);
  setLocalDefault(SETTINGS_PROFILE_NICK,          Channel::defaultName());
  setLocalDefault(SETTINGS_PROFILE_STATUS,        1);
  setLocalDefault(SETTINGS_WINDOWS_AERO,          true);

  setDefault(SETTINGS_AUTO_JOIN,                  true);
  setDefault(SETTINGS_DISPLAY_SECONDS,            false);
  setDefault(SETTINGS_DISPLAY_SERVICE,            false);
  setDefault(SETTINGS_LABS_STATIC_TRAY_ALERTS,    false);
  setDefault(SETTINGS_TOOLBAR_ACTIONS,            QStringList());
  setDefault(SETTINGS_TRANSLATION,                LS("auto"));

  setValue(SETTINGS_AUTO_SCROLL, true);
}


QVariant ChatSettings::setDefaultAndRead(const QString &key, const QVariant &value)
{
  setDefault(key, value);
  return this->value(key);
}


void ChatSettings::init()
{
  connect(ChatClient::i(), SIGNAL(ready()), SLOT(ready()));
  connect(ChatClient::i(), SIGNAL(offline()), SLOT(offline()));
  connect(ChatNotify::i(), SIGNAL(notify(Notify)), SLOT(notify(Notify)));
}


void ChatSettings::setDefault(const QString &key, const QVariant &value)
{
  if (m_settings)
    Settings::setDefault(key, m_settings->value(key, value));
  else
    Settings::setDefault(key, value);
}


void ChatSettings::setLocalDefault(const QString &key, const QVariant &value)
{
  setDefault(key, value);

  if (!m_local.contains(key))
    m_local.append(key);
}


void ChatSettings::setValue(const QString &key, const QVariant &value, bool notify, bool local)
{
  if (QSettings::value(key, m_default.value(key)) == value)
    return;

  Settings::setValue(key, value, notify);
  if (local || m_local.contains(key))
    return;

  if (ChatClient::state() == ChatClient::Online)
    set(key, value);
}


void ChatSettings::notify(const Notify &notify)
{
  if (notify.type() == Notify::FeedData) {
    const FeedNotify &n = static_cast<const FeedNotify &>(notify);
    if (n.name() == LS("settings") && n.channel() == ChatClient::id())
      set(n.status() == Notice::Found);
  }
}


void ChatSettings::offline()
{
  m_synced = false;
}


void ChatSettings::ready()
{
  FeedPtr feed = ChatClient::channel()->feed(LS("settings"), false);

  if (!feed) {
    const QByteArray id = ChatClient::id();
    ChatClientLocker locker(ChatClient::io());
    ClientFeeds::post(id, FEED_NAME_SETTINGS);
    ClientFeeds::put(id, LS("settings/head/mask"), 0700);
    ClientFeeds::request(id, FEED_METHOD_GET, FEED_NAME_SETTINGS);
  }
  else if (!m_synced)
    ClientFeeds::request(ChatClient::channel(), FEED_METHOD_GET, FEED_NAME_SETTINGS);
}


/*!
 * Синхронизация настроек с сервером.
 *
 * \param offline \b true если используется кэшированная версия фида.
 */
void ChatSettings::set(bool offline)
{
  FeedPtr feed = ChatClient::channel()->feed(FEED_NAME_SETTINGS, false);
  if (!feed)
    return;

  QStringList keys = feed->data().keys();
  keys.removeAll(LS("head"));

  if (!m_synced) {
    QMapIterator<QString, QVariant> i(m_default);
    while (i.hasNext()) {
      i.next();
      if (!m_local.contains(i.key()))
        Settings::setValue(i.key(), keys.contains(i.key()) ? feed->data().value(i.key()) : i.value());
    }
  }
  else {
    foreach (const QString &key, keys) {
      if (!m_local.contains(key))
        Settings::setValue(key, feed->data().value(key));
    }
  }

  sync();

  if (!offline && ChatClient::state() == ChatClient::Online && !m_synced) {
    m_synced = true;
    emit synced();
  }
}


void ChatSettings::set(const QString &key, const QVariant &value)
{
  if (key.isEmpty())
    return;

  ClientFeeds::post(ChatClient::id(), LS("settings/") + key, value, Feed::Echo | Feed::Share);
}
