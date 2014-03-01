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

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QToolTip>

#include "AddRoomDialog.h"
#include "ChatNotify.h"
#include "client/ChatClient.h"
#include "client/ClientFeeds.h"
#include "feeds/ListFeed.h"
#include "net/packets/Notice.h"
#include "sglobal.h"
#include "ui/SLineEdit.h"
#include "ui/Spinner.h"

AddRoomDialog::AddRoomDialog(QWidget *parent)
  : DialogFrame(parent)
{
  m_layout->addWidget(line());

  m_nameLabel = new QLabel(this);

  m_nameEdit = new SLineEdit(this);
  m_nameEdit->setMaxLength(64);

  setFocusProxy(m_nameEdit);

  m_spinner = new Spinner(this);

  m_privateBtn = new QCheckBox(this);

  m_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
  m_box->button(QDialogButtonBox::Ok)->setEnabled(false);

  QGridLayout *layout = new QGridLayout();
  layout->addWidget(m_nameLabel, 0, 0);
  layout->addWidget(m_nameEdit, 0, 1);
  layout->addWidget(m_privateBtn, 1, 1);
  layout->addWidget(line(), 2, 0, 1, 2);
  layout->addWidget(m_box, 3, 0, 1, 2);
  layout->setContentsMargins(0, 10, 0, 0);
  layout->setSpacing(10);

  m_layout->addLayout(layout);

  retranslateUi();

# if QT_VERSION >= 0x050000
  connect(m_box, &QDialogButtonBox::accepted, this, &AddRoomDialog::onAccepted);
  connect(m_box, &QDialogButtonBox::rejected, this, &AddRoomDialog::close);
  connect(m_nameEdit, &SLineEdit::textChanged, this, &AddRoomDialog::onTextChanged);
  connect(m_nameEdit, &SLineEdit::returnPressed, this, &AddRoomDialog::onAccepted);
  connect(ChatNotify::i(), &ChatNotify::notify, this, &AddRoomDialog::onNotify);
# else
  connect(m_box, SIGNAL(accepted()), SLOT(onAccepted()));
  connect(m_box, SIGNAL(rejected()), SLOT(close()));
  connect(m_nameEdit, SIGNAL(textChanged(QString)), SLOT(onTextChanged(QString)));
  connect(m_nameEdit, SIGNAL(returnPressed()), SLOT(onAccepted()));
  connect(ChatNotify::i(), SIGNAL(notify(Notify)), SLOT(onNotify(Notify)));
# endif
}


void AddRoomDialog::retranslateUi()
{
  setTitle(tr("Create new room"));
  m_nameLabel->setText(tr("Room name:"));
  m_privateBtn->setText(tr("This room is private"));
}


void AddRoomDialog::onAccepted()
{
  m_name = m_nameEdit->text().simplified();
  if (m_name.size() < 3)
    return;

  m_box->button(QDialogButtonBox::Ok)->setEnabled(false);
  m_spinner->start();
  m_nameEdit->addWidget(m_spinner, SLineEdit::RightSide);

  ClientFeeds::get(ChatClient::serverId(), LIST_FEED_ID_REQ, m_name);
}


void AddRoomDialog::onNotify(const Notify &notify)
{
  if (notify.type() != Notify::FeedReply)
    return;

  m_spinner->stop();
  const FeedNotify &n = static_cast<const FeedNotify &>(notify);

  if (n.name() == LIST_FEED_ID_REQ) {
    if (n.status() == Notice::NotFound) {
      emit create(n.json().value(LS("value")).toString(), m_name, m_privateBtn->isChecked());
      close();
      return;
    }

    QString errorText;
    if (n.status() == Notice::OK)
      errorText = tr("That room already exists. Please choose another name.");
    else if (n.status() == Notice::BadRequest)
      errorText = tr("Invalid room name. Please choose another name.");

    if (!errorText.isEmpty())
      QToolTip::showText(mapToGlobal(m_nameEdit->pos()), errorText);

    m_box->button(QDialogButtonBox::Ok)->setEnabled(true);
  }
}


void AddRoomDialog::onTextChanged(const QString &text)
{
  m_box->button(QDialogButtonBox::Ok)->setEnabled(text.simplified().size() >= 3);
}
