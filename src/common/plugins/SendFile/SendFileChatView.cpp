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

#include <QWebFrame>
#include <QDragEnterEvent>
#include <QFileInfo>
#include <QMimeData>

#include "hooks/ChatViewHooks.h"
#include "net/SimpleID.h"
#include "SendFileChatView.h"
#include "SendFilePlugin_p.h"
#include "sglobal.h"
#include "ui/tabs/ChatView.h"

SendFileChatView::SendFileChatView(SendFilePluginImpl *plugin)
  : QObject(plugin)
  , m_plugin(plugin)
{
  ChatViewHooks::add(this);
}


SendFileChatView::~SendFileChatView()
{
  ChatViewHooks::remove(this);
}


bool SendFileChatView::dragEnterEvent(ChatView *view, QDragEnterEvent *event)
{
  if (SimpleID::typeOf(view->id()) != SimpleID::UserId || !event->mimeData()->hasUrls() || getFiles(event->mimeData()->urls()).isEmpty())
    return false;

  event->acceptProposedAction();
  return true;
}


bool SendFileChatView::dropEvent(ChatView *view, QDropEvent *event)
{
  if (SimpleID::typeOf(view->id()) != SimpleID::UserId || !event->mimeData()->hasUrls())
    return false;

  event->acceptProposedAction();
  const QByteArray &id = view->id();
  const QStringList files = getFiles(event->mimeData()->urls());
  foreach (const QString &file, files)
    m_plugin->sendFile(id, file);

  return true;
}


void SendFileChatView::init(ChatView *view)
{
  if (SimpleID::typeOf(view->id()) != SimpleID::UserId)
    return;

  view->addJS(LS("qrc:/js/SendFile/SendFile.js"));
  view->page()->mainFrame()->addToJavaScriptWindowObject(LS("SendFile"), m_plugin);
}


void SendFileChatView::loadFinished(ChatView *view)
{
  if (SimpleID::typeOf(view->id()) == SimpleID::UserId)
    view->addCSS(LS("qrc:/css/SendFile/SendFile.css"));
}


QStringList SendFileChatView::getFiles(const QList<QUrl> &urls) const
{
  QStringList out;
  foreach (const QUrl &url, urls) {
    if (url.scheme() == LS("file")) {
      const QString file = url.toLocalFile();

      if (QFile::exists(file) && QFileInfo(file).isFile())
        out.append(url.toLocalFile());
    }
  }

  return out;
}
