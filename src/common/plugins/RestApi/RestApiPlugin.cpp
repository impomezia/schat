/* $Id: RestApiPlugin.cpp 3024 2012-08-23 21:53:10Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2012 IMPOMEZIA <schat@impomezia.com>
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

#include <QtPlugin>

#include "handlers/ChannelHandler.h"
#include "handlers/ServerHandler.h"
#include "RestApiCore.h"
#include "RestApiPlugin.h"
#include "RestApiPlugin_p.h"
#include "sglobal.h"
#include "Storage.h"

RestApiImpl::RestApiImpl(QObject *parent)
  : NodePlugin(parent)
{
  m_apiCore = new RestApiCore(this);
  RestApiCore::add(new ServerHandler());
  RestApiCore::add(new ChannelHandler());

  Storage::addFeature(LS("rest"));
}


NodePlugin *RestApiPlugin::create()
{
  m_plugin = new RestApiImpl(this);
  return m_plugin;
}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2(RestApi, RestApiPlugin);
#endif
