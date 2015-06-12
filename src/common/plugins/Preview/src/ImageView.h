/* Simple Chat
 * Copyright (c) 2008-2015 Alexander Sedov <imp@schat.me>
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

#ifndef IMAGEVIEW_H_
#define IMAGEVIEW_H_

#include <QGraphicsView>
#include <QImage>
#include <QPixmap>
#include <QRect>

#include "schat.h"

class QTimer;
class QGraphicsScene;

class ImageView : public QGraphicsView
{
  Q_OBJECT

public:
  ImageView(QWidget *parent = 0);
  virtual ~ImageView();

  inline bool autoZoomFit() const        { return m_autoZoomFit; }
  inline const QImage &image() const     { return m_image; }
  inline double scaleFactor() const      { return m_scaleFactor; }
  inline void setAutoZoomFit(bool value) { m_autoZoomFit = value; }
  void reset();
  void setImage(const QImage &image);
  void setMovie(QMovie *movie);

public slots:
  inline void zoomIn()       { zoom(m_scaleFactor * 1.1); }
  inline void zoomOriginal() { zoom(1.0); }
  inline void zoomOut()      { zoom(m_scaleFactor / 1.1); }
  void zoom(double scale);
  void zoomFit();
  
protected:
  void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
  void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
  void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
  void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

private slots:
  void generateCache();
  void onFinished(const QImage &image);

private:
  QRect sceneToViewport(const QRectF &rect) const;
  QRect viewportToScene(const QRect &rect) const;
  void queueGenerateCache();

private:
  bool m_autoZoomFit;
  double m_scaleFactor;
  QGraphicsProxyWidget *m_proxy;
  QGraphicsRectItem *m_item;
  QGraphicsScene *m_scene;
  QImage m_image;
  QPixmap m_cachedPixmap;
  QRect m_cachedRect;
  QRect m_cachedSceneRect;
  QTimer *m_cacheTimer;
};

#endif // IMAGEVIEW_H_
