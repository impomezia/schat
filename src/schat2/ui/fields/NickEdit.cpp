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

#include <QLabel>

#include "Channel.h"
#include "ChatCore.h"
#include "ChatSettings.h"
#include "client/ChatClient.h"
#include "client/ClientChannels.h"
#include "client/SimpleClient.h"
#include "net/Channels.h"
#include "net/packets/ChannelNotice.h"
#include "sglobal.h"
#include "ui/fields/NickEdit.h"
#include "ui/Spinner.h"

NickEdit::NickEdit(QWidget *parent)
  : SLineEdit(parent)
{
  setText(ChatClient::io()->nick());
  setMaxLength(Channel::MaxNameLength);

  m_error = new QLabel(this);
  m_error->setPixmap(QPixmap(LS(":/images/exclamation-red.png")));
  m_error->setVisible(false);

  m_spinner = new Spinner(this);

  connect(this, SIGNAL(editingFinished()), SLOT(editingFinished()));
  connect(this, SIGNAL(textChanged(QString)), SLOT(textChanged()));
  connect(ChatCore::settings(), SIGNAL(changed(QString,QVariant)), SLOT(settingsChanged(QString,QVariant)));
  connect(ChatClient::channels(), SIGNAL(notice(ChannelNotice)), SLOT(notice(ChannelNotice)));
}


void NickEdit::editingFinished()
{
  if (!Channel::isValidName(text()))
    return;

  if (ChatClient::state() != ChatClient::Online) {
    ChatClient::io()->setNick(text());
    ChatCore::settings()->setValue(LS("Profile/Nick"), ChatClient::channel()->name());
    return;
  }

  if (ChatClient::io()->nick() == text())
    return;

  spinner();
  ChatClient::channels()->nick(text());
}


void NickEdit::notice(const ChannelNotice &notice)
{
  if (notice.sender() != ChatClient::id())
    return;

  if (notice.command() == CHANNELS_INFO_CMD && notice.status() == Notice::OK) {
    makeRed(false);
    spinner(false);
    return;
  }

  if (notice.command() == CHANNELS_UPDATE_CMD) {
    if (notice.status() == Notice::ObjectAlreadyExists) {
      makeRed();
      setToolTip(tr("Nickname is already in use"));
      m_error->setVisible(true);
      addWidget(m_error, RightSide);
      spinner(false);
    }
    else {
      makeRed(false);
      spinner(false);
    }
  }
}


void NickEdit::settingsChanged(const QString &key, const QVariant &value)
{
  if (key == LS("Profile/Nick"))
    setText(value.toString());
}


void NickEdit::textChanged()
{
  makeRed(!Channel::isValidName(text()));
}


void NickEdit::makeRed(bool red)
{
  QPalette palette = this->palette();

  if (!red) {
    palette.setColor(QPalette::Active, QPalette::Base, Qt::white);
    removeWidget(m_error);
    setToolTip(QString());
  }
  else
    palette.setColor(QPalette::Active, QPalette::Base, QColor(255, 102, 102));

  setPalette(palette);
}


void NickEdit::spinner(bool start)
{
  if (start) {
    m_spinner->start();
    addWidget(m_spinner, RightSide);
  }
  else {
    m_spinner->stop();
    removeWidget(m_spinner);
  }
}
