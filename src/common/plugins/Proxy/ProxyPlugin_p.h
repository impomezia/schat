/* $Id: ProxyPlugin_p.h 3158 2012-10-12 17:41:07Z IMPOMEZIA $
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

#ifndef PROXYPLUGIN_P_H_
#define PROXYPLUGIN_P_H_

#include "plugins/ChatPlugin.h"

class SettingsPage;

class ProxyPluginImpl : public ChatPlugin
{
  Q_OBJECT

public:
  ProxyPluginImpl(QObject *parent);

private slots:
  void added(const QString &id, SettingsPage *page);
};

#endif /* PROXYPLUGIN_P_H_ */
