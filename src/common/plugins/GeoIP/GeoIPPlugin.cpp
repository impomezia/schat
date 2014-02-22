/* $Id: GeoIPPlugin.cpp 3024 2012-08-23 21:53:10Z IMPOMEZIA $
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

#include "GeoIPImpl.h"
#include "GeoIPPlugin.h"
#include "GeoIPPlugin_p.h"

GeoIPPluginImpl::GeoIPPluginImpl(QObject *parent)
  : NodePlugin(parent)
{
  m_hook = new GeoIPImpl();
}


GeoIPPluginImpl::~GeoIPPluginImpl()
{
  delete m_hook;
}


NodePlugin *GeoIPPlugin::create()
{
  m_plugin = new GeoIPPluginImpl(this);
  return m_plugin;
}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2(GeoIP, GeoIPPlugin);
#endif
