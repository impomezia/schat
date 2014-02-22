/* $Id: NodeFeedLogPlugin.h 3411 2013-01-17 04:07:15Z IMPOMEZIA $
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

#ifndef NODEFEEDLOGPLUGIN_H_
#define NODEFEEDLOGPLUGIN_H_

#include "CoreApi.h"
#include "NodeApi.h"

class NodeFeedLogPlugin : public QObject, CoreApi, NodeApi
{
  Q_OBJECT
  Q_INTERFACES(CoreApi NodeApi)

# if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "me.schat.server.NodeFeedLog" FILE "NodeFeedLog.json")
# endif

public:
  QVariantMap header() const
  {
    QVariantMap out        = CoreApi::header();
    out[CORE_API_ID]       = "NodeFeedLog";
    out[CORE_API_NAME]     = "Node Feed Log";
    out[CORE_API_VERSION]  = "0.1.0";
    out[CORE_API_TYPE]     = "server";
    out[CORE_API_SITE]     = "http://wiki.schat.me/Plugin/NodeFeedLog";
    out[CORE_API_DESC]     = "Node Feed Log";
    out[CORE_API_ENABLED]  = false;

    return out;
  }

  NodePlugin *create();
};

#endif /* NODEFEEDLOGPLUGIN_H_ */
