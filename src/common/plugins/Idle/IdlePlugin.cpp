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

#include <QtPlugin>

#include "ChatCore.h"
#include "ChatSettings.h"
#include "client/ChatClient.h"
#include "client/ClientChannels.h"
#include "Idle.h"
#include "IdlePlugin.h"
#include "IdlePlugin_p.h"
#include "IdleSettings.h"
#include "sglobal.h"
#include "Translation.h"

IdlePluginImpl::IdlePluginImpl(QObject *parent)
  : ChatPlugin(parent)
  , m_autoAway(0)
{
  ChatCore::settings()->setDefault(SETTINGS_AUTO_AWAY, 600);
  ChatCore::translation()->addOther(LS("idle"));

  m_idle = new Idle(this);

  connect(m_idle, SIGNAL(secondsIdle(int)), SLOT(idle(int)));
  connect(ChatCore::settings(), SIGNAL(changed(QString,QVariant)), SLOT(settingsChanged(QString,QVariant)));
}


void IdlePluginImpl::chatReady()
{
    QTimer::singleShot(0, this, SLOT(start()));
}


void IdlePluginImpl::idle(int seconds)
{
  int status = ChatClient::channel()->status().value();
  int diff = seconds - m_autoAway; // Уведомления могут приходить не каждую секунду.
  if ((diff >= 0 && diff <= 1) && status != Status::AutoAway) {
    away();
  }
  else if (seconds == 0 && status == Status::AutoAway)
    away(false);
}


void IdlePluginImpl::settingsChanged(const QString &key, const QVariant &value)
{
  if (key != SETTINGS_AUTO_AWAY)
    return;

  int autoAway = value.toInt();
  if (m_autoAway == autoAway)
    return;

  m_autoAway = autoAway;
  if (m_autoAway < 1 && m_idle->isActive())
    m_idle->stop();
  else if (m_autoAway >= 1 && !m_idle->isActive())
    m_idle->start();
}


void IdlePluginImpl::start()
{
  m_autoAway = ChatCore::settings()->value(SETTINGS_AUTO_AWAY).toInt();
  if (m_autoAway < 1)
    return;

  m_idle->start();
}


void IdlePluginImpl::away(bool away)
{
  if (ChatClient::channel()->status() == Status::DnD)
    return;

  int status = Status::AutoAway;
  if (!away)
    status = ChatCore::settings()->value(LS("Profile/Status")).toInt();

  if (status == Status::Offline)
    status = Status::Online;

  ChatClient::channel()->status() = status;
  ChatClient::channels()->update();
}


ChatPlugin *IdlePlugin::create()
{
  m_plugin = new IdlePluginImpl(this);
  return m_plugin;
}


QWidget *IdlePlugin::settings(QWidget *parent)
{
  return new IdleSettings(parent);
}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2(Idle, IdlePlugin);
#endif
