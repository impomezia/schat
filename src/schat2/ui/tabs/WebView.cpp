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
#include <QContextMenuEvent>
#include <QGraphicsColorizeEffect>
#include <QMenu>
#include <QMimeData>
#include <QWebFrame>

#include "BackdropWidget.h"
#include "sglobal.h"
#include "ui/ChatIcons.h"
#include "ui/tabs/WebView.h"

WebView::WebView(QWidget *parent)
  : QWebView(parent)
{
  setPage(new WebPage(this));
  setAcceptDrops(false);

  m_backdrop = new BackdropWidget(parentWidget());
  m_backdrop->hide();

  setIcons();
}


bool WebView::canPaste()
{
  const QMimeData *md = QApplication::clipboard()->mimeData();
  if (!md)
    return false;

  return (md->hasText() && !md->text().isEmpty()) || md->hasHtml() || md->hasFormat(LS("application/x-qrichtext")) || md->hasFormat(LS("application/x-qt-richtext"));
}


void WebView::setBackdropParent(QWidget *parent)
{
  Q_ASSERT(parent);

  m_backdrop->setParent(parent);
}


void WebView::changeEvent(QEvent *event)
{
  if (event->type() == QEvent::LanguageChange)
    retranslateUi();

  QWebView::changeEvent(event);
}


void WebView::contextMenuEvent(QContextMenuEvent *event)
{
  QMenu menu(this);
  const QWebHitTestResult r = page()->mainFrame()->hitTestContent(event->pos());
  bool selected = r.isContentSelected();
  bool editable = r.isContentEditable();

  if (selected) {
    if (editable)
      menu.addAction(pageAction(QWebPage::Cut));

    menu.addAction(pageAction(QWebPage::Copy));
  }

  const QUrl url = r.linkUrl();
  if (!url.isEmpty() && url.scheme() != LS("chat") && url.scheme() != LS("qrc") && url.scheme() != LS("emoticon"))
    menu.addAction(pageAction(QWebPage::CopyLinkToClipboard));

  if (editable && canPaste())
    menu.addAction(pageAction(QWebPage::Paste));

  menu.addAction(pageAction(QWebPage::SelectAll));
  developerMenu(&menu);
  contextMenu(&menu, r);

  menu.exec(event->globalPos());
}


void WebView::developerMenu(QMenu *menu)
{
  if (QWebSettings::globalSettings()->testAttribute(QWebSettings::DeveloperExtrasEnabled)) {
    menu->addSeparator();
    menu->addAction(pageAction(QWebPage::Reload));
    menu->addAction(pageAction(QWebPage::InspectElement));
  }
}


void WebView::resizeEvent(QResizeEvent *event)
{
  QWebView::resizeEvent(event);

  m_backdrop->resize(event->size());
}


void WebView::setIcons()
{
  pageAction(QWebPage::Cut)->setIcon(SCHAT_ICON(EditCut));
  pageAction(QWebPage::Copy)->setIcon(SCHAT_ICON(EditCopy));
  pageAction(QWebPage::CopyLinkToClipboard)->setIcon(SCHAT_ICON(EditCopy));
  pageAction(QWebPage::Paste)->setIcon(SCHAT_ICON(EditPaste));
  pageAction(QWebPage::SelectAll)->setIcon(SCHAT_ICON(EditSelectAll));
}


void WebView::contextMenu(QMenu *menu, const QWebHitTestResult &result)
{
  Q_UNUSED(menu)
  Q_UNUSED(result)
}


void WebView::retranslateUi()
{
  pageAction(QWebPage::Cut)->setText(tr("Cut"));
  pageAction(QWebPage::Copy)->setText(tr("Copy"));
  pageAction(QWebPage::CopyLinkToClipboard)->setText(tr("Copy Link"));
  pageAction(QWebPage::Paste)->setText(tr("Paste"));
  pageAction(QWebPage::SelectAll)->setText(tr("Select All"));
}


WebPage::WebPage(QObject* parent)
  : QWebPage(parent)
{
}


bool WebPage::shouldInterruptJavaScript()
{
  return false;
}

