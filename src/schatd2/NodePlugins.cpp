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

#include "cores/Core.h"
#include "NodePlugins.h"
#include "plugins/NodeApi.h"
#include "plugins/NodePlugin.h"
#include "Settings.h"
#include "Storage.h"
#include "sglobal.h"

NodePlugins *NodePlugins::m_self = 0;

NodePlugins::NodePlugins(QObject *parent)
  : Plugins(parent)
{
  m_self = this;
  m_type = LS("server");
}


void NodePlugins::init()
{
  Settings *settings = Storage::settings();

  for (int i = 0; i < m_list.size(); ++i) {
    PluginItem *item = m_list.at(i);

    NodeApi *api = qobject_cast<NodeApi *>(item->plugin());

    if (!api)
      continue;

    const QString key = LS("Plugins/") + item->id();
    settings->setDefault(key, item->header().value(CORE_API_ENABLED).toBool());
    if (settings->value(key) == false)
      continue;

    NodePlugin *plugin = api->create();
    if (!plugin)
      continue;

    item->setLoaded(true);
    m_nodePlugins.append(plugin);
  }
}
