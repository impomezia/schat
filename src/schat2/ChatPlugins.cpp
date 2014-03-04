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

#include "ChatCore.h"
#include "ChatPlugins.h"
#include "ChatSettings.h"
#include "plugins/ChatApi.h"
#include "plugins/ChatPlugin.h"
#include "plugins/CoreApi.h"
#include "sglobal.h"

ChatPlugins::ChatPlugins(QObject *parent)
  : Plugins(parent)
{
  m_type = LS("chat");

  if (!ChatCore::isReady())
    connect(ChatCore::i(), SIGNAL(ready()), SLOT(onReady()));
  else
    onReady();
}


/*!
 * Загрузка плагинов.
 */
void ChatPlugins::init()
{
  ChatSettings *settings = ChatCore::settings();
  QStringList invalid;

  for (int i = 0; i < m_sorted.size(); ++i) {
    PluginItem *item = m_plugins.value(m_sorted.at(i));
    ChatApi *api = qobject_cast<ChatApi *>(item->plugin());

    if (!api || !api->check()) {
      invalid.append(item->id());
      continue;
    }

    const QString key = LS("Plugins/") + item->id();
    settings->setLocalDefault(key, item->header().value(CORE_API_ENABLED).toBool());
    if (settings->value(key) == false)
      continue;

    ChatPlugin *plugin = api->create();
    if (!plugin) {
      invalid.append(item->id());
      continue;
    }

    if (ChatCore::isReady())
      plugin->chatReady();

    SLOG_DEBUG(item->header().value(CORE_API_VERSION).toString() << item->header().value(CORE_API_ID).toString());

    item->setLoaded(true);
    m_chatPlugins.append(plugin);
  }

  foreach (const QString &id, invalid) {
    m_sorted.removeAll(id);
    PluginItem *item = m_plugins.value(id);
    m_plugins.remove(id);

    if (item)
      delete item;
  }
}


void ChatPlugins::onReady()
{
  foreach (ChatPlugin *plugin, m_chatPlugins) {
    plugin->chatReady();
  }
}
