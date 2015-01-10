/* Simple Chat
 * Copyright (c) 2008-2015 Alexander Sedov <imp@schat.me>
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
#include "GenericNodePlugin.h"
#include "MessagesPlugin.h"
#include "NodeChannelsPlugin.h"
#include "NodeMasterPlugin.h"
#include "NodePlugins.h"
#include "plugins/NodeApi.h"
#include "plugins/NodePlugin.h"
#include "Settings.h"
#include "sglobal.h"
#include "Storage.h"

NodePlugins::NodePlugins(QObject *parent)
  : QObject(parent)
{
}


void NodePlugins::load()
{
  qobject_cast<NodeApi *>(new GenericNodePlugin(this))->create();
  qobject_cast<NodeApi *>(new MessagesPlugin(this))->create();
  qobject_cast<NodeApi *>(new NodeChannelsPlugin(this))->create();
  qobject_cast<NodeApi *>(new NodeMasterPlugin(this))->create();
}
