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

#ifndef GENERICNODEPLUGIN_H_
#define GENERICNODEPLUGIN_H_

#include "CoreApi.h"
#include "NodeApi.h"

class GenericNodePlugin : public QObject, CoreApi, NodeApi
{
  Q_OBJECT
  Q_INTERFACES(CoreApi NodeApi)
  Q_PLUGIN_METADATA(IID "me.schat.server.GenericNode")

public:
  QVariantMap header() const
  {
    QVariantMap out        = CoreApi::header();
    out[CORE_API_ID]       = "GenericNode";
    out[CORE_API_NAME]     = "Generic Node";
    out[CORE_API_VERSION]  = "2.3.2";
    out[CORE_API_TYPE]     = "server";
    out[CORE_API_SITE]     = "http://wiki.schat.me/Plugin/GenericNode";
    out[CORE_API_DESC]     = "Standard core of server";

    return out;
  }

  NodePlugin *create();
};

#endif /* GENERICNODEPLUGIN_H_ */
