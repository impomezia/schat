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

#include <QDesktopServices>
#include <QEvent>
#include <QGridLayout>
#include <QLineEdit>
#include <QToolButton>
#include <QUrl>

#include "RupSettings.h"
#include "sglobal.h"

RupSettings::RupSettings(const QString &token, QWidget *parent)
  : QGroupBox(parent)
{
  m_tokenEdit = new QLineEdit(this);
  m_tokenEdit->setEchoMode(QLineEdit::Password);

  if (token.size() == 42)
    m_tokenEdit->setText(QString().fill(' ', 42));

  m_pasteBtn = new QToolButton(this);
  m_pasteBtn->setAutoRaise(true);
  m_pasteBtn->setIcon(QIcon(LS(":/images/edit-paste.png")));

  m_createBtn = new QToolButton(this);
  m_createBtn->setObjectName("site_btn");
  m_createBtn->setCursor(Qt::PointingHandCursor);
  m_createBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_createBtn->setIcon(QIcon(LS(":/images/globe.png")));

  QGridLayout *layout = new QGridLayout(this);
  layout->addWidget(m_tokenEdit, 0, 0);
  layout->addWidget(m_pasteBtn, 0, 1);
  layout->addWidget(m_createBtn, 1, 0, 1, 2);

  retranslateUi();

  connect(m_tokenEdit, SIGNAL(editingFinished()), SLOT(onEditingFinished()));
  connect(m_tokenEdit, SIGNAL(textChanged(QString)), SLOT(onTextChanged(QString)));
  connect(m_pasteBtn, SIGNAL(clicked()), SLOT(onPaste()));
  connect(m_createBtn, SIGNAL(clicked()), SLOT(onCreateClicked()));
}


void RupSettings::changeEvent(QEvent *event)
{
  if (event->type() == QEvent::LanguageChange)
    retranslateUi();

  QWidget::changeEvent(event);
}


void RupSettings::onCreateClicked()
{
  QDesktopServices::openUrl(QUrl(LS("https://id.schat.me/settings/tokens/new?description=Screenpic&scope=image")));
}


void RupSettings::onEditingFinished()
{
  const QString text = m_tokenEdit->text().simplified();
  if (text.size() != 42)
    return;

  emit changed(text);
}


void RupSettings::onPaste()
{
  m_tokenEdit->clear();
  m_tokenEdit->paste();

  onEditingFinished();
}


void RupSettings::onTextChanged(const QString &text)
{
  m_tokenEdit->setText(text.simplified());
  const int size = text.size();

  makeRed(size > 0 && size != 42);
}


void RupSettings::makeRed(bool red)
{
  QPalette palette = m_tokenEdit->palette();
  palette.setColor(QPalette::Active, QPalette::Base, red ? QColor(255, 102, 102) : Qt::white);

  m_tokenEdit->setPalette(palette);
}


void RupSettings::retranslateUi()
{
  setTitle(tr("Access token:"));
  m_pasteBtn->setToolTip(tr("Paste"));
  m_createBtn->setText(tr("Create new token"));
}
