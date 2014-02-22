/* $Id: RawFeedsCmd.h 3224 2012-10-27 03:43:46Z IMPOMEZIA $
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

#ifndef RAWFEEDSCMD_H_
#define RAWFEEDSCMD_H_

#include "client/ClientHooks.h"

class RawFeeds;

namespace Hooks
{

class RawFeedsCmd : public Messages
{
  Q_OBJECT

public:
  RawFeedsCmd(RawFeeds *parent);
  bool command(const QByteArray &dest, const ClientCmd &cmd);

private:
  void localFeeds(const QByteArray &dest);
  void request(const QByteArray &dest, const ClientCmd &cmd);

  RawFeeds *m_plugin;
};

} // namespace Hooks

#endif /* RAWFEEDSCMD_H_ */
