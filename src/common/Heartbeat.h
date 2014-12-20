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

#ifndef HEARTBEAT_H_
#define HEARTBEAT_H_

#include <QObject>

#include "interfaces/IClientListener.h"
#include "schat.h"

class QBasicTimer;

class SCHAT_EXPORT Heartbeat : public QObject, public IClientListener
{
  Q_OBJECT

public:
  Heartbeat(QObject *parent = 0);
  ~Heartbeat();
  void onAuthRequired(IClient *client) override;
  void onConnectToHost(IClient *client, const QString &hostName, quint16 port) override;
  void onDisconnected(IClient *client, int status) override;
  void onPacket(IClient *client, const SJMPPacket &packet) override;
  void onPong(IClient *client) override;

protected:
  void timerEvent(QTimerEvent *event) override;

private:
  enum TimerState {
    WaitConnect,
    WaitAuth,
    Idle,
    WaitPong,
    Reconnect
  };

  void setTimerState(TimerState state);

  IClient *m_client;
  QBasicTimer *m_timer;
  TimerState m_timerState;
};

#endif // HEARTBEAT_H_
