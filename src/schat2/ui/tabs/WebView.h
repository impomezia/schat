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

#ifndef WEBVIEW_H_
#define WEBVIEW_H_

#include <QWebView>

#include "schat.h"

class BackdropWidget;
class QGraphicsBlurEffect;

class SCHAT_CORE_EXPORT WebView : public QWebView
{
  Q_OBJECT

public:
  WebView(QWidget *parent = 0);
  static bool canPaste();
  void setBackdropParent(QWidget *parent);
  void showDialog(QWidget *widget);

protected:
  void changeEvent(QEvent *event) Q_DECL_OVERRIDE;
  void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;
  void developerMenu(QMenu *menu);
  void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
  void setIcons();

  virtual void contextMenu(QMenu *menu, const QWebHitTestResult &result);
  virtual void retranslateUi();

private slots:
  void onBackdropClosed();
  void onSettingsChanged(const QString &key, const QVariant &value);

private:
  void blur(bool enabled);

  BackdropWidget *m_backdrop;
};


class WebPage : public QWebPage
{
  Q_OBJECT

public:
  WebPage(QObject* parent = 0);

public slots:
  bool shouldInterruptJavaScript();
};

#endif /* WEBVIEW_H_ */
