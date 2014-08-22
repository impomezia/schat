/*   Copyright (C) 2013-2014 Alexander Sedov <imp@schat.me>
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

#ifndef RUPSETTINGS_H_
#define RUPSETTINGS_H_

#include <QGroupBox>

class QLineEdit;
class QToolButton;

class RupSettings : public QGroupBox
{
  Q_OBJECT

public:
  RupSettings(const QString &token, QWidget *parent = 0);

signals:
  void changed(const QString &token);

protected:
  void changeEvent(QEvent *event) override;

private slots:
  void onCreateClicked();
  void onEditingFinished();
  void onPaste();
  void onTextChanged(const QString &text);

private:
  void makeRed(bool red = true);
  void retranslateUi();

  QLineEdit *m_tokenEdit;
  QToolButton *m_pasteBtn;
  QToolButton *m_createBtn;
};

#endif // RUPSETTINGS_H_
