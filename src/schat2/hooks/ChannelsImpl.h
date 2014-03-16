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

#ifndef CHANNELSIMPL_H_
#define CHANNELSIMPL_H_

#include "client/ClientHooks.h"

class ChannelInfo;

namespace Hooks
{

class ChannelsImpl : public Channels
{
  Q_OBJECT

public:
  ChannelsImpl(QObject *parent = 0);

  ClientChannel get(const QByteArray &id);
  void add(ClientChannel channel, const ChannelInfo &info, const QVariantMap &json);
};

} // namespace Hooks

#endif /* CHANNELSIMPL_H_ */
