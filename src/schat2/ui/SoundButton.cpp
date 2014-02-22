/* $Id: SoundButton.cpp 3698 2013-06-17 13:41:51Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2013 IMPOMEZIA <schat@impomezia.com>
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

#include <QEvent>

#include "ChatAlerts.h"
#include "ChatCore.h"
#include "ChatSettings.h"
#include "client/ChatClient.h"
#include "sglobal.h"
#include "ui/SoundButton.h"

SoundButton::SoundButton(QWidget *parent)
  : QToolButton(parent)
  , m_mute(ChatAlerts::isMute())
{
  setAutoRaise(true);
  setMute(m_mute);

  connect(this, SIGNAL(clicked(bool)), SLOT(clicked()));
  connect(ChatCore::settings(), SIGNAL(changed(QString,QVariant)), SLOT(settingsChanged(QString,QVariant)));
}


void SoundButton::setMute(bool mute)
{
  m_mute = mute;

  if (m_mute)
    setIcon(QIcon(LS(":/images/sound_mute.png")));
  else
    setIcon(QIcon(LS(":/images/sound.png")));

  retranslateUi();
}


void SoundButton::changeEvent(QEvent *event)
{
  if (event->type() == QEvent::LanguageChange)
    retranslateUi();

  QToolButton::changeEvent(event);
}


void SoundButton::clicked()
{
  setMute(!m_mute);

  if (ChatClient::channel()->status() == Status::DnD)
    ChatCore::settings()->setValue(LS("Alerts/Sounds.DnD"), !m_mute);
  else
    ChatCore::settings()->setValue(LS("Alerts/Sounds"), !m_mute);
}


void SoundButton::settingsChanged(const QString &key, const QVariant &value)
{
  Q_UNUSED(value)

  if (key == LS("Profile/Status") || key == LS("Alerts/Sounds.DnD") || key == LS("Alerts/Sounds"))
    setMute(ChatAlerts::isMute());
}


void SoundButton::retranslateUi()
{
  if (m_mute)
    setToolTip(tr("Turn on sounds"));
  else
    setToolTip(tr("Turn off sounds"));
}
