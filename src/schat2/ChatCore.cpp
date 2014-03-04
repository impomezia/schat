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

#include <QDateTime>
#include <QDesktopServices>
#include <QPainter>
#include <QTextDocument>
#include <QThreadPool>
#include <QTimer>
#include <QWidget>

#include "ChatAlerts.h"
#include "ChatCore.h"
#include "ChatHooks.h"
#include "ChatNotify.h"
#include "ChatPlugins.h"
#include "ChatSettings.h"
#include "ChatUrls.h"
#include "client/ChatClient.h"
#include "client/ClientMessages.h"
#include "client/SimpleClient.h"
#include "Extensions.h"
#include "feeds/FeedStorage.h"
#include "hooks/ChannelMenu.h"
#include "hooks/ChannelMenuImpl.h"
#include "hooks/ChannelsImpl.h"
#include "hooks/ChatViewHooks.h"
#include "hooks/ClientFeedsImpl.h"
#include "hooks/ClientImpl.h"
#include "hooks/CommandsImpl.h"
#include "hooks/MessagesImpl.h"
#include "hooks/ServerMenuImpl.h"
#include "hooks/UserMenuImpl.h"
#include "net/SimpleID.h"
#include "NetworkManager.h"
#include "Path.h"
#include "Profile.h"
#include "ServiceThread.h"
#include "sglobal.h"
#include "text/HtmlFilter.h"
#include "text/PlainTextFilter.h"
#include "text/TokenFilter.h"
#include "Translation.h"
#include "ui/ChatIcons.h"
#include "ui/profile/ProfileFieldFactory.h"
#include "ui/tabs/SettingsTabHook.h"
#include "WebBridge.h"

bool ChatCore::m_ready     = false;
ChatCore *ChatCore::m_self = 0;


ChatCore::ChatCore(QObject *parent)
  : QObject(parent)
  , m_extensions(0)
{
  m_self = this;

  SCHAT_DEBUG_CODE(QTime t; t.start();)

  qsrand(QTime(0,0,0).msecsTo(QTime::currentTime()) ^ reinterpret_cast<quintptr>(this));

  m_pool = new QThreadPool(this);
  m_pool->setMaxThreadCount(1);

  m_service = new ServiceThread(this);

  new ChatUrls(this);
  m_settings = new ChatSettings(Path::config(), Path::data(Path::SystemScope) + LS("/default.conf"), this);

# if !defined(SCHAT_NO_EXTENSIONS)
  m_extensions = new Extensions(this);
# endif

  loadTranslation();

  m_client = new ChatClient(this);
  new ChatNotify(this);
  new FeedStorage(this);

  new ChatAlerts(this);

  ChatIcons::init();

  new Profile(this);

  m_plugins = new ChatPlugins(this);
  m_plugins->load();

  m_settings->init();

  connect(m_settings, SIGNAL(changed(QString,QVariant)), SLOT(onSettingsChanged(QString,QVariant)));
  connect(m_service, SIGNAL(ready()), SLOT(onReady()));

  m_service->start();

  SLOG_DEBUG(t.elapsed() << "ms");
}


ChatCore::~ChatCore()
{
  m_ready = false;
  m_self  = 0;

  TokenFilter::clear();
  ProfileFieldFactory::clear();

  m_service->quit();
  m_service->wait();
}


QByteArray ChatCore::randomId()
{
  return ChatId(ChatId::MessageId, ChatClient::id()).toByteArray();
}


/*!
 * Определение опций компиляции во время выполнения.
 */
QStringList ChatCore::config()
{
  QStringList out;
# if defined(SCHAT_DEBUG)
  out += LS("DEBUG");
# endif
# if !defined(SCHAT_NO_SSL)
  out += LS("SSL");
# endif
# if !defined(SCHAT_NO_QDNS)
  out += LS("QDNS");
# endif
# if !defined(SCHAT_NO_EXTENSIONS)
  out += LS("EXTENSIONS");
# endif

  return out;
}


/*!
 * Отправка сообщения.
 */
void ChatCore::send(const QString &text)
{
  if (text.isEmpty())
    return;

  ChatClient::messages()->send(m_currentId, text);
}


void ChatCore::onReady()
{
  m_client->setReady();

  new Hooks::MessagesImpl(m_client);
  new Hooks::CommandsImpl(m_client);
  new Hooks::ChannelsImpl(m_client);
  new Hooks::ClientImpl(m_client);
  new ClientFeedsImpl(m_client);

  new Hooks::ChannelMenu(m_client);
  new ChannelMenuImpl(m_client);
  new UserMenuImpl(m_client);
  new ServerMenuImpl(m_client);

  new ChatViewHooks(m_client);
  new SettingsTabHook(m_client);

  m_networkManager = new NetworkManager(this);
  ChatClient::id(); // Необходимо для инициализации базовых настроек.

  new WebBridge(m_client);

  m_ready = true;
  emit ready();

  ChatClient::open();
}


void ChatCore::onSettingsChanged(const QString &key, const QVariant &value)
{
  if (key == LS("Translation"))
    m_translation->load(value.toString());
}


/*!
 * Загрузка перевода пользовательского интерфейса.
 */
void ChatCore::loadTranslation()
{
  m_translation = new Translation(this);
  m_translation->setSearch(QStringList() << (Path::data(Path::SystemScope) + LS("/translations")) << (Path::data() + LS("/translations")));
  m_translation->load(m_settings->value(QLatin1String("Translation")).toString());
}
