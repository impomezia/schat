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

#ifndef CLIENTFEEDSIMPL_H_
#define CLIENTFEEDSIMPL_H_

#include "client/ClientHooks.h"

class FeedNotify;

class ClientFeedsImpl : public Hooks::Feeds
{
  Q_OBJECT

public:
  ClientFeedsImpl(QObject *parent = 0);

protected:
  void addImpl(ClientChannel channel, const ChannelInfo &info, const QVariantMap &json);
  void onReply(const NetRequest &req, const NetReply &reply);
  void readFeedImpl(const FeedNotice &packet);

private:
  void feed();
  void get();
  void get(const QByteArray &id, const QStringList &feeds);
  void headers();
  void reply();

  ClientChannel m_channel;
  const FeedNotice *m_packet;
};

#endif /* CLIENTFEEDSIMPL_H_ */
