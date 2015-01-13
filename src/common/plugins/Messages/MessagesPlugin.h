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

#ifndef MESSAGESPLUGIN_H_
#define MESSAGESPLUGIN_H_

#include "CoreApi.h"
#include "NodeApi.h"

class MessagesPlugin : public QObject, CoreApi, NodeApi
{
  Q_OBJECT
  Q_INTERFACES(CoreApi NodeApi)

public:
  MessagesPlugin(QObject *parent = 0);

  QVariantMap header() const Q_DECL_OVERRIDE
  {
    QVariantMap out        = CoreApi::header();
    out[CORE_API_ID]       = "Messages";
    out[CORE_API_NAME]     = "Messages";
    out[CORE_API_VERSION]  = "3.0.0";
    out[CORE_API_TYPE]     = "server";
    out[CORE_API_SITE]     = "https://wiki.schat.me/Plugin/Messages";
    out[CORE_API_DESC]     = "Server Messages Support";

    return out;
  }

  NodePlugin *create() Q_DECL_OVERRIDE;
};

#endif /* MESSAGESPLUGIN_H_ */
