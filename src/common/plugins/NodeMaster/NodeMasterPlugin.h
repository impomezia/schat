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

#ifndef NODEMASTERPLUGIN_H_
#define NODEMASTERPLUGIN_H_

#include "CoreApi.h"
#include "NodeApi.h"

class NodeMasterPlugin : public QObject, CoreApi, NodeApi
{
  Q_OBJECT
  Q_INTERFACES(CoreApi NodeApi)

# if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID "me.schat.server.NodeMaster" FILE "NodeMaster.json")
# endif

public:
  QVariantMap header() const
  {
    QVariantMap out        = CoreApi::header();
    out[CORE_API_ID]       = "NodeMaster";
    out[CORE_API_NAME]     = "Node Master";
    out[CORE_API_VERSION]  = "0.1.0";
    out[CORE_API_TYPE]     = "server";
    out[CORE_API_SITE]     = "https://wiki.schat.me/Plugin/NodeMaster";
    out[CORE_API_DESC]     = "Node Master";
    out[CORE_API_ENABLED]  = false;

    return out;
  }

  NodePlugin *create();
};

#endif /* NODEMASTERPLUGIN_H_ */
