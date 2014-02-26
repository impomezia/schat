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
  void request(const QString &method, const QByteArray &dest, const QString &body);

  RawFeeds *m_plugin;
  QVariantMap m_headers;
};

} // namespace Hooks

#endif /* RAWFEEDSCMD_H_ */
