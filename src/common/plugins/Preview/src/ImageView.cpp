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


#include <QGraphicsProxyWidget>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QLabel>
#include <QMovie>
#include <QPainter>
#include <QPaintEvent>
#include <QThreadPool>
#include <QTimer>
#include <QWheelEvent>

#include "ImageView.h"
#include "ScaleRunnable.h"

ImageView::ImageView(QWidget* parent)
  : QGraphicsView(parent)
  , m_autoZoomFit(false)
  , m_scaleFactor(1.0)
  , m_proxy(0)
  , m_item(0)
  , m_cacheTimer(0)
{
  m_scene = new QGraphicsScene(this);

  setViewportMargins(0, 0, 0, 0);
  setContentsMargins(0, 0, 0, 0);
  setLineWidth(0);
  setDragMode(ScrollHandDrag);
  setFrameStyle(Plain | NoFrame);

  setScene(m_scene);
}


ImageView::~ImageView() {
  if (m_cacheTimer) {
    m_cacheTimer->stop();
    delete m_cacheTimer;
  }
}


void ImageView::setImage(const QImage &image)
{
  reset();

  m_image = image;

  m_item = new QGraphicsRectItem();
  m_item->setRect(0, 0, m_image.width(), m_image.height());
  m_item->setBrush(m_image);
  m_item->setPen(Qt::NoPen);

  m_scene->addItem(m_item);
  m_scene->setSceneRect(0, 0, m_image.width(), m_image.height());

  if (m_autoZoomFit)
    zoomFit();

  queueGenerateCache();
}


void ImageView::setMovie(QMovie *movie)
{
  reset();

  QLabel *label = new QLabel();
  label->setMovie(movie);
  movie->setParent(label);

  movie->start();

  m_proxy = m_scene->addWidget(label);
  m_scene->setSceneRect(0, 0, label->sizeHint().width(), label->sizeHint().height());
}


void ImageView::zoom(double scale)
{
  if (m_scaleFactor == scale)
    return;

  m_autoZoomFit = false;

  resetTransform();
  m_scaleFactor = scale;
  this->scale(scale, scale);

  queueGenerateCache();
}


void ImageView::zoomFit()
{
  if (!m_image.isNull() && m_image.width() <= width() && m_image.height() <= height()) {
    zoomOriginal();
    return;
  }

  fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
  m_scaleFactor = transform().m11();
  queueGenerateCache();
}


void ImageView::keyPressEvent(QKeyEvent *event)
{
  const int key = event->key();
  if (key == Qt::Key_Equal || key == Qt::Key_Plus)
    zoomIn();
  else if (key == Qt::Key_Minus)
    zoomOut();
  else if (event->nativeVirtualKey() == Qt::Key_H && event->modifiers() == Qt::ControlModifier)
    zoomOriginal();

  QGraphicsView::keyPressEvent(event);
}


void ImageView::paintEvent(QPaintEvent* event)
{
  if (m_item && m_scaleFactor != 1.0 && !m_cachedPixmap.isNull()) {
    const QRect viewportImageRect = sceneToViewport(m_item->rect());
    const QRect desiredCachedRect = viewportToScene(viewportImageRect.intersected(viewport()->rect()));

    if (m_cachedSceneRect == desiredCachedRect) {
      const QRect repaintImageRect = viewportImageRect.intersected(event->rect());
      if (m_cachedRect.contains(repaintImageRect)) {
        QPainter painter(viewport());
        painter.fillRect(event->rect(), backgroundBrush());
        painter.drawPixmap(repaintImageRect, m_cachedPixmap);
        return;
      }
    }
  }

  if (!m_image.isNull())
    queueGenerateCache();

  QGraphicsView::paintEvent(event);
}


void ImageView::resizeEvent(QResizeEvent *event)
{
  QGraphicsView::resizeEvent(event);

  if (m_autoZoomFit)
    zoomFit();
}


void ImageView::wheelEvent(QWheelEvent *event)
{
  if (event->modifiers() & Qt::ControlModifier) {
    if (event->delta() > 0)
      zoomIn();
    else
      zoomOut();
  }
  else
    QGraphicsView::wheelEvent(event);
}


void ImageView::generateCache() {
  m_cacheTimer->deleteLater();
  m_cacheTimer = 0;

  m_cachedRect      = sceneToViewport(m_item->rect()).intersected(viewport()->rect());
  m_cachedSceneRect = viewportToScene(m_cachedRect);
  const QRect subRect = m_image.rect().intersected(m_cachedSceneRect);

  const uchar* bits = m_image.constBits();
  const unsigned int offset = subRect.x() * m_image.depth() / 8 + subRect.y() * m_image.bytesPerLine();

  const QImage subImage = QImage(bits + offset, subRect.width(), subRect.height(), m_image.bytesPerLine(), m_image.format());

  ScaleRunnable *runnable = new ScaleRunnable(subImage, m_cachedRect.size());
  connect(runnable, SIGNAL(finished(QImage)), SLOT(onFinished(QImage)));

  QThreadPool::globalInstance()->start(runnable);
}

void ImageView::onFinished(const QImage &image)
{
  if (qAbs(m_cachedRect.width() - image.width()) > 2 || qAbs(m_cachedRect.height() - image.height()))
    return;

  m_cachedPixmap = QPixmap::fromImage(image);

  viewport()->update();
}


QRect ImageView::viewportToScene(const QRect &rect) const
{
  QPoint topLeft = mapToScene(rect.topLeft()).toPoint();
  QPoint bottomRight = mapToScene(rect.bottomRight()).toPoint();
  return QRect(topLeft, bottomRight);
}


QRect ImageView::sceneToViewport(const QRectF &rect) const
{
  QPoint topLeft = mapFromScene(rect.topLeft());
  QPoint bottomRight = mapFromScene(rect.bottomRight());
  return QRect(topLeft, bottomRight);
}


void ImageView::queueGenerateCache()
{
  if (!m_item)
    return;

  if (!m_cachedPixmap.isNull())
    m_cachedPixmap = QPixmap();

  if (m_scaleFactor == 1.0) {
    if (m_cacheTimer) {
      m_cacheTimer->stop();
      delete m_cacheTimer;
      m_cacheTimer = 0;
    }

    return;
  }

  if (!m_cacheTimer) {
    m_cacheTimer = new QTimer();
    m_cacheTimer->setSingleShot(true);

    connect(m_cacheTimer, SIGNAL(timeout()), SLOT(generateCache()));
  }

  m_cacheTimer->start(20);
}


void ImageView::reset()
{
  if (m_item) {
    m_scene->removeItem(m_item);
    delete m_item;
    m_item = 0;

    m_image = QImage();
  }

  if (m_proxy) {
    m_scene->removeItem(m_proxy);
    delete m_proxy;
    m_proxy = 0;
  }
}
