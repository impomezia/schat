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
#include <QFileDialog>
#include <QLabel>
#include <QMenu>
#include <QMovie>
#include <QTimer>
#include <QToolBar>
#include <QToolButton>
#include <QUrl>
#include <QVBoxLayout>

#include "ChatCore.h"
#include "ChatSettings.h"
#include "ImageView.h"
#include "JSON.h"
#include "PreviewCore.h"
#include "PreviewDialog.h"
#include "PreviewItem.h"
#include "sglobal.h"
#include "ui/ChatIcons.h"

PreviewDialog::PreviewDialog(const QVariant &data, QWidget *parent)
  : DialogFrame(parent)
  , m_copy(0)
{
  const QVariantMap map = data.toMap();
  m_flags    = map.value(LS("flags")).toInt();
  m_fileName = map.value(LS("orig")).toUrl().toLocalFile();

  m_size.setWidth(map.value(LS("width")).toInt());
  m_size.setHeight( map.value(LS("height")).toInt());

  m_view = new ImageView(this);
  m_view->scene()->setSceneRect(0, 0, m_size.width(), m_size.height());

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
  m_openLink = m_toolBar->addAction(QIcon(LS(":/images/Preview/globe.png")), tr("Open Link..."), this, SLOT(openLink()));
  m_toolBar->addSeparator();

  if (!(m_flags & PreviewItem::Animated)) {
    m_copy = m_toolBar->addAction(QIcon(LS(":/images/Preview/copy.png")), tr("Copy Image to Clipboard"), this, SLOT(copy()));
    m_view->setAutoZoomFit(true);
  }
  else {
    m_view->setAutoZoomFit(false);
  }

  m_save = m_toolBar->addAction(QIcon(LS(":/images/Preview/save.png")), tr("Save As..."), this, SLOT(save()));
  m_toolBar->addSeparator();

  m_sizeLabel = new QLabel(QString(LS("%1x%2")).arg(m_size.width()).arg(m_size.height()), this);
  m_sizeLabel->setMargin(4);
  m_sizeLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
  m_toolBar->addWidget(m_sizeLabel);

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
  m_openLink->setText(tr("Open Link..."));
  m_save->setText(tr("Save As..."));

  if (m_copy)
    m_copy->setText(tr("Copy Image to Clipboard"));
}


void PreviewDialog::closeEvent(QCloseEvent *event)
{
  m_view->reset();

  DialogFrame::closeEvent(event);
}


void PreviewDialog::copy()
{
  QApplication::clipboard()->setImage(m_view->image());
}


void PreviewDialog::copyLink()
{
  QApplication::clipboard()->setText(m_url.toString());
}


void PreviewDialog::openLink()
{
  QDesktopServices::openUrl(m_url);
}


void PreviewDialog::save()
{
  const QString path     = ChatCore::settings()->value(PreviewCore::kSavePath).toString() + LC('/') + m_url.path().section(LC('/'), -1);
  const QString suffix   = QFileInfo(m_fileName).suffix();
  const QString fileName = QFileDialog::getSaveFileName(this, tr("Save"), path, QString(LS("*.%1;;*")).arg(suffix == LS("jpeg") ? LS("jpg") : suffix));

  if (fileName.isEmpty())
    return;

  QFile::copy(m_fileName, fileName);
  ChatCore::settings()->setValue(PreviewCore::kSavePath, QFileInfo(fileName).absolutePath());
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
