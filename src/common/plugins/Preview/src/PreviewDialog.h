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

#ifndef PREVIEWDIALOG_H_
#define PREVIEWDIALOG_H_

#include <QUrl>

#include "ui/tabs/DialogFrame.h"

class ImageView;
class QToolBar;

class PreviewDialog : public DialogFrame
{
  Q_OBJECT

public:
  PreviewDialog(const QVariant &data, QWidget *parent = 0);

protected:
  void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
  void retranslateUi() Q_DECL_OVERRIDE;

private slots:
  void copy();
  void copyLink();
  void openLink();
  void save();
  void start();

private:
  void createZoom();
  void setUrl(const QUrl &url);

private:
  ImageView *m_view;
  int m_flags;
  QAction *m_copy;
  QAction *m_copyLink;
  QAction *m_openLink;
  QAction *m_save;
  QAction *m_zoomFit;
  QAction *m_zoomOriginal;
  QAction *m_zoomOut;
  QLabel *m_sizeLabel;
  QLabel *m_urlLabel;
  QSize m_size;
  QString m_fileName;
  QToolBar *m_toolBar;
  QToolButton *m_zoomBtn;
  QUrl m_url;
};

#endif // PREVIEWDIALOG_H_
