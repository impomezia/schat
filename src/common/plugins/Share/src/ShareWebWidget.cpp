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

#include <QGridLayout>
#include <QMenu>
#include <QToolButton>
#include <QUrl>

#include "ShareWebWidget.h"
#include "ui/SLineEdit.h"
#include "sglobal.h"
#include "ui/ChatIcons.h"

ShareWebWidget::ShareWebWidget(QWidget *parent)
  : QFrame(parent)
{
  m_urlEdit = new SLineEdit(this);
  m_urlEdit->setMinimumWidth(256);
  m_urlEdit->setInactiveText(tr("Enter link to image"));

  m_addBtn = new QToolButton(this);
  m_addBtn->setIcon(SCHAT_ICON(Add));
  m_addBtn->setText(tr("Add"));
  m_addBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_addBtn->setEnabled(false);

  QGridLayout *layout = new QGridLayout(this);
  layout->addWidget(m_urlEdit, 0, 0);
  layout->addWidget(m_addBtn, 0, 1);
  layout->setMargin(6);

  setFocusPolicy(Qt::WheelFocus);

  connect(m_addBtn, SIGNAL(clicked()), SLOT(apply()));
  connect(m_urlEdit, SIGNAL(textChanged(QString)), SLOT(onTextChanged(QString)));
  connect(m_urlEdit, SIGNAL(returnPressed()), SLOT(apply()));
}


void ShareWebWidget::showEvent(QShowEvent *event)
{
  QFrame::showEvent(event);

  m_urlEdit->setFocus();
}


void ShareWebWidget::apply()
{
  const QUrl url(QUrl::fromUserInput(m_urlEdit->text()));
  if (!isValid(url))
    return;

  QMenu *popup = qobject_cast<QMenu *>(parentWidget());
  if (popup && isVisible())
    popup->close();

  close();

  QList<QUrl> urls;
  urls.reserve(1);
  urls.append(url);

  emit upload(urls, false);
}


void ShareWebWidget::onTextChanged(const QString &text)
{
  m_addBtn->setEnabled(isValid(QUrl::fromUserInput(text)));
}


bool ShareWebWidget::isValid(const QUrl &url) const
{
  if (!url.isValid())
    return false;

  return url.scheme() == LS("http") || url.scheme() == LS("https");
}
