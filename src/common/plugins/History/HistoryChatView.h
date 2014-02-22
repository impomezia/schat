/* $Id: HistoryChatView.h 3756 2013-07-15 15:23:36Z IMPOMEZIA $
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

#ifndef HISTORYCHATVIEW_H_
#define HISTORYCHATVIEW_H_

#include "hooks/ChatViewHooks.h"

class Notify;
class QAction;

struct MessageRecord;

class HistoryChatView : public ChatViewHooks
{
  Q_OBJECT

public:
  HistoryChatView(QObject *parent = 0);
  Q_INVOKABLE bool isAutoLoad(const QString &id) const;

signals:
  void loading(const QString &id);

protected:
  bool onContextMenu(ChatView *view, QMenu *menu, const QWebHitTestResult &result);
  void addImpl(ChatView *view);
  void initImpl(ChatView *view);
  void loadFinishedImpl(ChatView *view);
  void onRetranslate();

private slots:
  void notify(const Notify &notify);
  void ready();
  void remove();
  void settingsChanged(const QString &key, const QVariant &value);
  void synced();

private:
  /// Опции редактирования сообщений.
  enum EditFlags {
    NoEdit          = 0, ///< Редактирование запрещено.
    SelfEdit        = 1, ///< Пользователь может редактировать и удалять свои сообщения.
    ModeratorRemove = 2, ///< Модераторы канала могут удалять сообщения.
    ModeratorEdit   = 4  ///< Модераторы канала могут редактировать сообщения.
  };

  /// Опции по умолчанию.
  enum Defaults {
    DefaultEditFlags = SelfEdit | ModeratorRemove, ///< Пользователь может редактировать и удалять свои сообщения, модераторы только удалять.
    DefaultTimeOut   = 3600                        ///< По прошествии 1 часа (3600 секунд) пользователь больше не может редактировать свои сообщения.
  };

  enum Permissions {
    NoPermissions = 0,
    Remove        = 1,
    Modify        = 2
  };

  bool compatible(const QByteArray &id) const;
  bool sync(const QByteArray &id, qint64 date = 0);
  int permissions(const MessageRecord &record) const;
  QAction *removeAction(const QVariant &data);
  void emulateLast(const QByteArray &channelId, const QList<QByteArray> &ids);

  bool m_autoLoad;   ///< Опция SETTINGS_HISTORY_AUTO_LOAD.
  QAction *m_remove; ///< Удаление сообщения.
};

#endif /* HISTORYCHATVIEW_H_ */
