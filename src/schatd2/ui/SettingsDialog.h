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

#ifndef SETTINGSDIALOG_H_
#define SETTINGSDIALOG_H_

#include <QDialog>

#include "schat.h"

class QStackedWidget;
class QToolButton;
class Settings;

class SettingsDialog : public QDialog
{
  Q_OBJECT

public:
  SettingsDialog(Settings *settings, QWidget * parent = 0);

protected:
  void changeEvent(QEvent *event) Q_DECL_OVERRIDE;

private slots:
  void tooglePage();

private:
  QToolButton *addBtn(const QIcon &icon, const QString &text);
  void retranslateUi();

  QStackedWidget *m_pages;
  QToolButton *m_aboutBtn;
  QToolButton *m_settingsBtn;
};

#endif /* SETTINGSDIALOG_H_ */
