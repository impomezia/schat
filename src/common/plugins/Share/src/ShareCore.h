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

#include "data/UploadItem.h"
#include "plugins/ChatPlugin.h"

class ChatSettings;
class Providers;
class QRunnable;
class ShareNet;
class UploadResult;

class ShareCore : public ChatPlugin
{
  Q_OBJECT

public:
  static const QString kCaptureMouse;
  static const QString kEditor;
  static const QString kLastOpenDir;
  static const QString kLastSaveDir;
  static const QString kProvider;
  static const QString kSaveCopy;
  static const QString kSaveCopyIn;

  ShareCore(QObject *parent);
  inline Providers *providers() const { return m_providers; }
  void chatReady() Q_DECL_OVERRIDE;

public slots:
  void add(QRunnable *task);
  void openFile();
  void openFile(const QString &fileName);

private slots:
  void onEditingFinished(UploadItemPtr item);
  void onImageSaved(const ChatId &id, const QByteArray &body, const Thumbnail &thumbnail);
  void onTaskReady(qint64 counter, QObject *object);
  void onUploadFinished(const UploadResult &result);
  void onUploadProgress(const ChatId &id, int percent);
  void startTasks();

private:
  QObjectList getPlugins(const char *className) const;
  void initProviders();

  ChatSettings *m_settings;              ///< Настройки.
  int m_netId;
  Providers *m_providers;                ///< Доступные провайдеры.
  QList<QRunnable*> m_tasks;             ///< Задачи для выполнения в отдельном потоке.
  QMap<ChatId, UploadItemPtr> m_pending;
  ShareNet *m_net;
};

#endif // PREVIEWCORE_H_
