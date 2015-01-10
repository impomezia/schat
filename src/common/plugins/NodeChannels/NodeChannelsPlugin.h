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

#ifndef NODECHANNELSPLUGIN_H_
#define NODECHANNELSPLUGIN_H_

#include "CoreApi.h"
#include "NodeApi.h"

class NodeChannelsPlugin : public QObject, CoreApi, NodeApi
{
  Q_OBJECT
  Q_INTERFACES(CoreApi NodeApi)

public:
  NodeChannelsPlugin(QObject *parent);

  QVariantMap header() const override
  {
    QVariantMap out        = CoreApi::header();
    out[CORE_API_ID]       = "NodeChannels";
    out[CORE_API_NAME]     = "Node Channels";
    out[CORE_API_VERSION]  = "3.0.0";
    out[CORE_API_TYPE]     = "server";
    out[CORE_API_SITE]     = "https://wiki.schat.me/Plugin/NodeChannels";
    out[CORE_API_DESC]     = "Node Channels";

    return out;
  }

  NodePlugin *create() override;
};

#endif /* NODECHANNELSPLUGIN_H_ */
