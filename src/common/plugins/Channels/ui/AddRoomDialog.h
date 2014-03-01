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

#ifndef ADDROOMDIALOG_H_
#define ADDROOMDIALOG_H_

#include "ui/tabs/DialogFrame.h"

class QCheckBox;
class QDialogButtonBox;
class SLineEdit;
class Spinner;
class Notify;

class AddRoomDialog : public DialogFrame
{
  Q_OBJECT

public:
  AddRoomDialog(QWidget *parent = 0);

signals:
  void create(const QString &id, const QString &name, bool _private);

private slots:
  void onAccepted();
  void onNotify(const Notify &notify);
  void onTextChanged(const QString &text);

private:
  QCheckBox *m_privateBtn;
  QDialogButtonBox *m_box;
  QLabel *m_nameLabel;
  QString m_name;
  SLineEdit *m_nameEdit;
  Spinner *m_spinner;
};

#endif // ADDROOMDIALOG_H_
