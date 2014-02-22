/* $Id: WebView.h 3368 2013-01-02 22:52:33Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2013 IMPOMEZIA <schat@impomezia.com>
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

class SCHAT_CORE_EXPORT WebView : public QWebView
{
  Q_OBJECT

public:
  WebView(QWidget *parent = 0);
  static bool canPaste();

protected:
  void changeEvent(QEvent *event);
  void contextMenuEvent(QContextMenuEvent *event);
  void developerMenu(QMenu *menu);
  void setIcons();

  virtual void contextMenu(QMenu *menu, const QWebHitTestResult &result);
  virtual void retranslateUi();
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
