/* $Id: NodeChannelsPlugin_p.h 3334 2012-12-20 03:51:57Z IMPOMEZIA $
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

#ifndef NODECHANNELSPLUGIN_P_H_
#define NODECHANNELSPLUGIN_P_H_

#include "plugins/NodePlugin.h"

class NodeChannelIndex;

class NodeChannelsImpl : public NodePlugin
{
  Q_OBJECT

public:
  NodeChannelsImpl(QObject *parent);
  inline static NodeChannelIndex *index() { return m_self->m_index; }
  inline static NodeChannelsImpl *i()     { return m_self; }

private:
  NodeChannelIndex *m_index;       ///< Индекс каналов.
  static NodeChannelsImpl *m_self; ///< Указатель на себя.
};

#endif /* NODECHANNELSPLUGIN_P_H_ */
