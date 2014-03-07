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

#ifndef PREVIEWCHATVIEW_H_
#define PREVIEWCHATVIEW_H_

#include <QObject>

#include "interfaces/IChatViewHook.h"

class PreviewCore;

class PreviewChatView : public QObject, public IChatViewHook
{
  Q_OBJECT

public:
  PreviewChatView(PreviewCore *core);
  ~PreviewChatView();

  void init(ChatView *view) Q_DECL_OVERRIDE;
  void loadFinished(ChatView *view) Q_DECL_OVERRIDE;

private:
  PreviewCore *m_core;
};

#endif // PREVIEWCHATVIEW_H_
