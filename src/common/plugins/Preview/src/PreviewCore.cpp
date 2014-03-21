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

#include <QDesktopServices>

#include "ChatCore.h"
#include "ChatSettings.h"
#include "id/ChatId.h"
#include "Path.h"
#include "PreviewChatView.h"
#include "PreviewCore.h"
#include "PreviewFilter.h"
#include "PreviewStorage.h"
#include "PreviewWindowObject.h"
#include "sglobal.h"
#include "text/TokenFilter.h"
#include "Translation.h"

const QString PreviewCore::kAnimation = LS("Preview/Animation");
const QString PreviewCore::kMaxSize   = LS("Preview/MaxSize");
const QString PreviewCore::kSavePath  = LS("Preview/SavePath");

PreviewCore::PreviewCore(QObject *parent)
  : ChatPlugin(parent)
{
  ChatCore::settings()->setDefault(kAnimation, true);
  ChatCore::settings()->setDefault(kMaxSize,   1024 * 1024 * 5);

# if QT_VERSION >= 0x050000
  ChatCore::settings()->setLocalDefault(kSavePath, QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
# else
  ChatCore::settings()->setLocalDefault(kSavePath, QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation));
# endif

  ChatCore::translation()->addOther(LS("preview"));

  TokenFilter::add(LS("channel"), new PreviewFilter(this));

  m_storage = new PreviewStorage(this);
  m_windowObject = new PreviewWindowObject(this);
}


void PreviewCore::add(const ChatId &id, const QList<QUrl> &urls)
{
  SLOG_DEBUG(id.toBase32() << urls);

  m_storage->add(id, urls);
}


void PreviewCore::chatReady()
{
  new PreviewChatView(this);
}
