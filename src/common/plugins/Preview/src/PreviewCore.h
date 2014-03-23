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

#ifndef PREVIEWCORE_H_
#define PREVIEWCORE_H_

#include <QObject>
#include <QUrl>

#include "plugins/ChatPlugin.h"

class ChatId;
class PreviewStorage;
class PreviewWindowObject;

class PreviewCore : public ChatPlugin
{
  Q_OBJECT

public:
  static const QString kAnimation;
  static const QString kMaxSize;
  static const QString kSavePath;

  PreviewCore(QObject *parent);
  inline PreviewStorage *storage() const           { return m_storage; }
  inline PreviewWindowObject *windowObject() const { return m_windowObject; }
  void add(const ChatId &id, const QList<QUrl> &urls);
  void chatReady() Q_DECL_OVERRIDE;

private:
  PreviewStorage *m_storage;
  PreviewWindowObject *m_windowObject;
};

#endif // PREVIEWCORE_H_
