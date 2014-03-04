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

#include "Account.h"
#include "ChatCore.h"
#include "client/ChatClient.h"
#include "client/SimpleClient.h"
#include "sglobal.h"
#include "ui/LoginIcon.h"
#include "ui/AuthIcon.h"

LoginIcon::LoginIcon(QWidget *parent)
  : QLabel(parent)
{
  hide();

  if (!ChatCore::isReady())
    connect(ChatCore::i(), SIGNAL(ready()), SLOT(onReady()));
  else
    onReady();
}


void LoginIcon::onReady()
{
  connect(ChatClient::io(), SIGNAL(clientStateChanged(int,int)), SLOT(reload()));

  reload();
}


void LoginIcon::reload()
{
  if (ChatClient::state() != ChatClient::Online || ChatClient::channel()->account()->provider.isEmpty()) {
    setVisible(false);
    return;
  }

  QPixmap pixmap = AuthIcon::pixmap(ChatClient::channel()->account()->provider);
  if (pixmap.isNull())
    pixmap = QPixmap(LS(":/images/key.png"));

  setPixmap(pixmap);
  setVisible(true);
}
