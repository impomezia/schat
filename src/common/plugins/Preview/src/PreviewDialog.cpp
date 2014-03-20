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

#include <QMovie>
#include <QTimer>
#include <QUrl>
#include <QVBoxLayout>

#include "ImageView.h"
#include "JSON.h"
#include "PreviewDialog.h"
#include "PreviewItem.h"
#include "sglobal.h"

PreviewDialog::PreviewDialog(const QVariant &data, QWidget *parent)
  : DialogFrame(parent)
{
  const QVariantMap map = data.toMap();
  m_flags    = map.value(LS("flags")).toInt();
  m_fileName = map.value(LS("orig")).toUrl().toLocalFile();

  m_view = new ImageView(this);
  m_view->scene()->setSceneRect(0, 0, map.value(LS("width")).toInt(), map.value(LS("height")).toInt());
  m_view->setAutoZoomFit(!(m_flags & PreviewItem::Animated));

  m_layout->addWidget(m_view);

  QTimer::singleShot(0, this, SLOT(start()));
}


void PreviewDialog::retranslateUi()
{
}


void PreviewDialog::start()
{
  if (m_flags & PreviewItem::Animated)
    m_view->setMovie(new QMovie(m_fileName));
  else
    m_view->setImage(QImage(m_fileName));
}
