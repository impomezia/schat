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

#include <QEvent>

#include "ChatAlerts.h"
#include "ChatCore.h"
#include "ChatSettings.h"
#include "client/ChatClient.h"
#include "sglobal.h"
#include "ui/SoundButton.h"

SoundButton::SoundButton(QWidget *parent)
  : QToolButton(parent)
{
  setAutoRaise(true);
  onReady();

  connect(this, SIGNAL(clicked(bool)), SLOT(onClicked()));
  connect(ChatCore::settings(), SIGNAL(changed(QString,QVariant)), SLOT(onSettingsChanged(QString,QVariant)));
  
  if (!ChatCore::isReady())
    connect(ChatCore::i(), SIGNAL(ready()), SLOT(onReady()));
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


void SoundButton::onClicked()
{
  setMute(!m_mute);

  if (ChatClient::channel()->status() == Status::DnD)
    ChatCore::settings()->setValue(LS("Alerts/Sounds.DnD"), !m_mute);
  else
    ChatCore::settings()->setValue(LS("Alerts/Sounds"), !m_mute);
}


void SoundButton::onReady()
{
  setMute(ChatAlerts::isMute());
}


void SoundButton::onSettingsChanged(const QString &key, const QVariant &value)
{
  Q_UNUSED(value)

  if (key == ChatSettings::kProfileStatus || key == LS("Alerts/Sounds.DnD") || key == LS("Alerts/Sounds"))
    setMute(ChatAlerts::isMute());
}


void SoundButton::retranslateUi()
{
  if (m_mute)
    setToolTip(tr("Turn on sounds"));
  else
    setToolTip(tr("Turn off sounds"));
}
