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
#include <QClipboard>
#include <QLabel>
#include <QMenu>
#include <QToolButton>
#include <QVBoxLayout>

#include "sglobal.h"
#include "ShareDnD.h"
#include "ShareWidget.h"
#include "ui/ChatIcons.h"

ShareWidget::ShareWidget(QWidget *parent)
  : QFrame(parent)
{
  m_addLabel = new QLabel(LS("<b>") + tr("Add images") + LS("</b>"), this);

  m_diskBtn = new QToolButton(this);
  m_diskBtn->setText(tr("Browse..."));
  m_diskBtn->setIcon(QIcon(LS(":/images/Share/folder-open.png")));
  m_diskBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_diskBtn->setToolTip(tr("Add images from computer"));

  m_pasteBtn = new QToolButton(this);
  m_pasteBtn->setText(tr("Paste"));
  m_pasteBtn->setIcon(SCHAT_ICON(EditPaste));
  m_pasteBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_pasteBtn->setToolTip(tr("Add images from clipboard"));
  m_pasteBtn->setEnabled(false);

  m_webBtn = new QToolButton(this);
  m_webBtn->setText(tr("Web"));
  m_webBtn->setIcon(SCHAT_ICON(Globe));
  m_webBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_webBtn->setToolTip(tr("Add images from web"));

  QHBoxLayout *btnLay = new QHBoxLayout();
  btnLay->addWidget(m_diskBtn);
  btnLay->addWidget(m_pasteBtn);
  btnLay->addWidget(m_webBtn);
  btnLay->setSpacing(3);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(m_addLabel);
  layout->addLayout(btnLay);
  layout->setMargin(6);

  connect(m_diskBtn, SIGNAL(clicked()), SLOT(close()));
  connect(m_diskBtn, SIGNAL(clicked()), SIGNAL(addFromDisk()));
  connect(m_webBtn, SIGNAL(clicked()), SLOT(close()));
  connect(m_webBtn, SIGNAL(clicked()), SIGNAL(addFromWeb()));
  connect(m_pasteBtn, SIGNAL(clicked()), SLOT(paste()));
  connect(QApplication::clipboard(), SIGNAL(dataChanged()), SLOT(onDataChanged()));

  onDataChanged();
}


void ShareWidget::close()
{
  QMenu *popup = qobject_cast<QMenu *>(parentWidget());
  if (popup && isVisible())
    popup->close();

  QFrame::close();
}


void ShareWidget::onDataChanged()
{
  const QMimeData *mimeData = QApplication::clipboard()->mimeData();
  m_pasteBtn->setEnabled(!ShareDnD::getUrls(mimeData, 1).isEmpty() || !ShareDnD::getFiles(mimeData, 10485760, 1).isEmpty());
}


void ShareWidget::paste()
{
  close();

  const QMimeData *mimeData = QApplication::clipboard()->mimeData();

  bool loc = false;
  QList<QUrl> urls = ShareDnD::getUrls(mimeData, 1);
  if (urls.isEmpty()) {
    loc  = true;
    urls = ShareDnD::getFiles(mimeData, 10485760, 1);
  }

  if (!urls.isEmpty())
    emit upload(urls, loc);
}
