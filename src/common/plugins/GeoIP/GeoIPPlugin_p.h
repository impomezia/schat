/* $Id: GeoIPPlugin_p.h 2496 2012-04-05 19:52:35Z IMPOMEZIA $
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

#ifndef GEOIPPLUGIN_P_H_
#define GEOIPPLUGIN_P_H_

#include "plugins/NodePlugin.h"

class GeoIPImpl;

class GeoIPPluginImpl : public NodePlugin
{
  Q_OBJECT

public:
  GeoIPPluginImpl(QObject *parent);
  ~GeoIPPluginImpl();

private:
  GeoIPImpl *m_hook;
};


#endif /* GEOIPPLUGIN_P_H_ */
