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

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QLabel>
#include <QMenu>
#include <QMovie>
#include <QTimer>
#include <QToolBar>
#include <QToolButton>
#include <QUrl>
#include <QVBoxLayout>

#include "ImageView.h"
#include "JSON.h"
#include "PreviewDialog.h"
#include "PreviewItem.h"
#include "sglobal.h"
#include "ui/ChatIcons.h"

PreviewDialog::PreviewDialog(const QVariant &data, QWidget *parent)
  : DialogFrame(parent)
{
  const QVariantMap map = data.toMap();
  m_flags    = map.value(LS("flags")).toInt();
  m_fileName = map.value(LS("orig")).toUrl().toLocalFile();

  m_view = new ImageView(this);
  m_view->scene()->setSceneRect(0, 0, map.value(LS("width")).toInt(), map.value(LS("height")).toInt());
  m_view->setAutoZoomFit(!(m_flags & PreviewItem::Animated));

  m_urlLabel = new QLabel(this);
  m_urlLabel->setOpenExternalLinks(true);

  m_layout->addWidget(m_view);
  m_layout->addWidget(m_urlLabel);

  setUrl(map.value(LS("url")).toString());

  m_toolBar = new QToolBar(this);
  m_toolBar->setIconSize(QSize(22, 22));

  createZoom();

  m_toolBar->addSeparator();
  m_copyLink = m_toolBar->addAction(QIcon(LS(":/images/Preview/link.png")), tr("Copy Link"), this, SLOT(copyLink()));
  m_openLink = m_toolBar->addAction(QIcon(LS(":/images/Preview/globe.png")), tr("Open Link"), this, SLOT(openLink()));

  m_headerLayout->insertWidget(0, m_toolBar);
  m_headerLayout->setStretch(1, 1);

  retranslateUi();

  QTimer::singleShot(0, this, SLOT(start()));
}


void PreviewDialog::retranslateUi()
{
  m_zoomBtn->setToolTip(tr("Zoom In"));
  m_zoomOut->setText(tr("Zoom Out"));
  m_zoomOriginal->setText(tr("100%"));
  m_zoomFit->setText(tr("Fit Screen"));
  m_copyLink->setText(tr("Copy Link"));
  m_openLink->setText(tr("Open Link"));
}


void PreviewDialog::copyLink()
{
  QApplication::clipboard()->setText(m_url.toString());
}


void PreviewDialog::openLink()
{
  QDesktopServices::openUrl(m_url);
}


void PreviewDialog::start()
{
  if (m_flags & PreviewItem::Animated)
    m_view->setMovie(new QMovie(m_fileName));
  else
    m_view->setImage(QImage(m_fileName));
}


void PreviewDialog::createZoom()
{
  m_zoomBtn = new QToolButton(this);
  m_zoomBtn->setIcon(QIcon(LS(":/images/Preview/zoom-in.png")));
  m_zoomBtn->setPopupMode(QToolButton::MenuButtonPopup);

  QMenu *menu = new QMenu(m_zoomBtn);
  m_zoomBtn->setMenu(menu);

  m_zoomOriginal = menu->addAction(tr("100%"), m_view, SLOT(zoomOriginal()));
  m_zoomFit = menu->addAction(tr("Fit Screen"), m_view, SLOT(zoomFit()));

  m_toolBar->addWidget(m_zoomBtn);
  m_zoomOut = m_toolBar->addAction(QIcon(LS(":/images/Preview/zoom-out.png")), tr("Zoom Out"), m_view, SLOT(zoomOut()));

  connect(m_zoomBtn, SIGNAL(clicked()), m_view, SLOT(zoomIn()));
}


void PreviewDialog::setUrl(const QUrl &url)
{
  m_urlLabel->setText(QString(LS("<a href='%1' style='text-decoration:none; color:#216ea7;'>%1</a>")).arg(url.toString()));
  m_urlLabel->setToolTip(url.toString());

  m_url = url;
}
