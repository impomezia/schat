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

#ifndef COMMANDSIMPL_H_
#define COMMANDSIMPL_H_

#include "client/ClientHooks.h"

namespace Hooks
{

class SCHAT_CORE_EXPORT CommandsImpl : public Messages
{
  Q_OBJECT

public:
  CommandsImpl(QObject *parent = 0);
  bool command(const QByteArray &dest, const ClientCmd &cmd);
  int read(MessagePacket packet);

private:
  QString colorizePing(int offset) const;
  void setGender(const QString &gender, const QString &color);
};

} // namespace Hooks

#endif /* COMMANDSIMPL_H_ */
