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

#include <QApplication>
#include <QLabel>
#include <QStackedWidget>
#include <QToolButton>
#include <QVBoxLayout>

#include "AboutPage.h"
#include "SettingsDialog.h"
#include "SettingsPage.h"
#include "sglobal.h"
#include "version.h"

SettingsDialog::SettingsDialog(Settings *settings, QWidget *parent)
  : QDialog(parent)
{
  setAttribute(Qt::WA_DeleteOnClose);
  setWindowFlags(windowFlags() ^ Qt::WindowContextHelpButtonHint);
  setWindowTitle(SCHAT_NAME);
  setMinimumWidth(320);

  QFrame *topFrame = new QFrame(this);
  topFrame->setObjectName(LS("TopFrame"));
  topFrame->setStyleSheet(LS("#TopFrame{background-color:#fff;}"));

  m_settingsBtn = addBtn(QIcon(LS(":/images/32/gear.png")), tr("Settings"));
  m_settingsBtn->setChecked(true);

  m_aboutBtn = new QToolButton(this);
  m_aboutBtn->setText(QApplication::applicationVersion());
  m_aboutBtn->setAutoRaise(true);
  m_aboutBtn->setCheckable(true);
  m_aboutBtn->setToolButtonStyle(Qt::ToolButtonTextOnly);
  connect(m_aboutBtn, SIGNAL(clicked()), SLOT(tooglePage()));

  m_pages = new QStackedWidget(this);
  m_pages->addWidget(new SettingsPage(settings, this));
  m_pages->addWidget(new AboutPage(settings, this));

  QFrame *line = new QFrame(this);
  line->setFrameStyle(QFrame::HLine | QFrame::Sunken);

  QHBoxLayout *btnLay = new QHBoxLayout(topFrame);
  btnLay->addWidget(m_settingsBtn);
  btnLay->addStretch();
  btnLay->addWidget(m_aboutBtn, 0, Qt::AlignTop);
  btnLay->setMargin(4);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(topFrame);
  layout->addWidget(line);
  layout->addWidget(m_pages);
  layout->setMargin(0);
  layout->setSpacing(0);
}


void SettingsDialog::changeEvent(QEvent *event)
{
  if (event->type() == QEvent::LanguageChange)
    retranslateUi();

  QDialog::changeEvent(event);
}


void SettingsDialog::tooglePage()
{
  QToolButton *button = qobject_cast<QToolButton*>(sender());

  m_settingsBtn->setChecked(button == m_settingsBtn);
  m_aboutBtn->setChecked(button == m_aboutBtn);

  if (button == m_settingsBtn)
    m_pages->setCurrentIndex(0);

  if (button == m_aboutBtn)
    m_pages->setCurrentIndex(1);
}


QToolButton *SettingsDialog::addBtn(const QIcon &icon, const QString &text)
{
  QToolButton *button = new QToolButton(this);
  button->setIcon(icon);
  button->setText(text);
  button->setAutoRaise(true);
  button->setCheckable(true);
  button->setIconSize(QSize(32, 32));
  button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  connect(button, SIGNAL(clicked()), SLOT(tooglePage()));

  return button;
}


void SettingsDialog::retranslateUi()
{
  m_settingsBtn->setText(tr("Settings"));
}
