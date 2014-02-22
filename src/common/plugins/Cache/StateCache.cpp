/* $Id: StateCache.cpp 3698 2013-06-17 13:41:51Z IMPOMEZIA $
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

#include <QTimer>

#include "ChatCore.h"
#include "ChatSettings.h"
#include "client/ChatClient.h"
#include "client/ClientChannels.h"
#include "feeds/ServerFeed.h"
#include "net/SimpleID.h"
#include "sglobal.h"
#include "StateCache.h"
#include "ui/TabBar.h"
#include "ui/tabs/AbstractTab.h"
#include "ui/tabs/ChannelBaseTab.h"
#include "ui/tabs/ServerTab.h"
#include "ui/TabWidget.h"

StateCache::StateCache(QObject *parent)
  : QObject(parent)
  , m_settings(ChatCore::settings())
  , m_key(SETTINGS_PINNED_TABS)
{
  m_settings->setDefault(m_key, QStringList());

  connect(m_settings, SIGNAL(changed(QString,QVariant)), SLOT(settingsChanged(QString,QVariant)));
  connect(m_settings, SIGNAL(synced()), SLOT(synced()));
  connect(ChatClient::i(), SIGNAL(ready()), SLOT(ready()));

  QTimer::singleShot(0, this, SIGNAL(start()));
}


void StateCache::pinned(AbstractTab *tab)
{
  const QString id = encode(tab->id());
  if (!m_tabs.contains(id)) {
    m_tabs.append(id);

    if (ChatClient::channels()->mainId() == tab->id())
      m_settings->setValue(SETTINGS_AUTO_JOIN, true);

    save();
  }
}


/*!
 * Обработка подключения клиента к серверу.
 */
void StateCache::ready()
{
  m_prefix = SimpleID::encode(ChatClient::serverId()) + LC('/');
  m_settings->setLocalDefault(m_prefix + LS("LastTalk"), QString());
}


/*!
 * Сохранение списка закреплённых вкладок и их порядка.
 */
void StateCache::save()
{
  if (!m_settings->isSynced())
    return;

  TabWidget *tabs = TabWidget::i();

  for (int i = tabs->count() - 1; i >= 0; --i) {
    AbstractTab *tab = tabs->widget(i);
    if (tab->options() & AbstractTab::Pinned) {
      const QString id = encode(tab->id());
      m_tabs.removeAll(id);
      m_tabs.prepend(id);
    }
  }

  if (ChatClient::channels()->policy() & ServerFeed::ForcedJoinPolicy) {
    const QString mainId = SimpleID::encode(ChatClient::channels()->mainId());
    m_tabs.removeAll(mainId);
    m_tabs.prepend(mainId);
  }

  m_settings->setValue(m_key, m_tabs);
}


void StateCache::settingsChanged(const QString &key, const QVariant &value)
{
  if (key == m_key)
    m_tabs = value.toStringList();
}


void StateCache::start()
{
  TabWidget *tabs = TabWidget::i();
  if (tabs) {
    connect(tabs, SIGNAL(pinned(AbstractTab*)), SLOT(pinned(AbstractTab*)));
    connect(tabs, SIGNAL(unpinned(AbstractTab*)), SLOT(unpinned(AbstractTab*)));
    connect(tabs, SIGNAL(currentChanged(int)), SLOT(tabIndexChanged(int)));
    connect(tabs->tabBar(), SIGNAL(tabMoved(int,int)), SLOT(save()));
  }
}


/*!
 * Обработка успешной синхронизации настроек с сервером.
 *
 * Открываются и закрепляются все ранее закреплённые разговоры.
 */
void StateCache::synced()
{
  m_tabs = m_settings->value(m_key).toStringList();
  const int policy = ChatClient::channels()->policy();

  if (policy & ServerFeed::MainChannelPolicy && m_tabs.isEmpty() && m_settings->value(SETTINGS_AUTO_JOIN).toBool())
    m_tabs.append(SimpleID::encode(ChatClient::channels()->mainId()));

  TabWidget *tabs = TabWidget::i();
  if (!tabs)
    return;

  foreach (const QString &text, m_tabs) {
    const QByteArray id = decode(text);
    if (Channel::isCompatibleId(id))
      join(id);
  }

  if (!m_tabs.isEmpty() && restoreLastTalk())
    return;

  if (policy & ServerFeed::ForcedJoinPolicy)
    return;

  if (!tabs->channelsCount())
    tabs->tab("list");

  tabs->closePage("progress");
  tabs->closePage("welcome");
}


/*!
 * Обработка изменения текущей активной вкладки.
 */
void StateCache::tabIndexChanged(int index)
{
  if (index == -1 || !m_settings->isSynced())
    return;

  AbstractTab *tab = TabWidget::i()->widget(index);
  if (!tab || !Channel::isCompatibleId(tab->id()))
    return;

  const QString id = SimpleID::encode(tab->id());

  if (tab->options() & AbstractTab::Pinned)
    m_settings->setValue(m_prefix + LS("LastTalk"), QString(id));
}


void StateCache::unpinned(AbstractTab *tab)
{
  const QString id = encode(tab->id());
  if (m_tabs.contains(id)) {
    m_tabs.removeAll(id);

    if (ChatClient::channels()->mainId() == tab->id())
      m_settings->setValue(SETTINGS_AUTO_JOIN, false);

    save();
  }
}


/*!
 * Установка последнего разговора в качестве текущей вкладки.
 *
 * Функция вызывается после синхронизации настроек с сервером и открытия закреплённых вкладок,
 * если разговор не был кэширован, ничего не происходит.
 */
bool StateCache::restoreLastTalk()
{
  const QString encoded = m_settings->value(m_prefix + LS("LastTalk")).toString();
  if (!m_tabs.contains(encoded))
    return false;

  const QByteArray id = SimpleID::decode(encoded);
  if (!Channel::isCompatibleId(id))
    return false;

  TabWidget::i()->channelTab(id, false, true);
  return true;
}


QByteArray StateCache::decode(const QString &id) const
{
  if (id.size() == 34)
    return SimpleID::decode(id);

  return id.toUtf8();
}


QString StateCache::encode(const QByteArray &id) const
{
  if (SimpleID::typeOf(id) != SimpleID::InvalidId)
    return SimpleID::encode(id);

  return id;
}


/*!
 * \deprecated Начиная с версии 2.0.6 нет необходимости отправлять запрос на вход в обычный канал,
 * т.к. сервер теперь делает это автоматически без запроса от клиента.
 */
void StateCache::join(const QByteArray &id)
{
  TabWidget *tabs = TabWidget::i();

  if (SimpleID::typeOf(id) == SimpleID::ServerId) {
    ServerTab *serverTab = tabs->serverTab();
    if (tabs->indexOf(serverTab) == -1) {
      tabs->addTab(serverTab, QString());
      serverTab->setOnline();
      serverTab->setText(ChatClient::serverName());
    }

    serverTab->pin();
    return;
  }

  ChannelBaseTab *tab = tabs->channelTab(id, true, false);
  if (tab && !(tab->options() & AbstractTab::Pinned))
    tab->pin();

  if (!tab || SimpleID::typeOf(id) == SimpleID::ChannelId)
    ChatClient::channels()->join(id);
}
