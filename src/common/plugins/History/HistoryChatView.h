/* Simple Chat
 * Copyright (c) 2008-2015 Alexander Sedov <imp@schat.me>
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

#include <QObject>

#include "interfaces/IChatViewHook.h"

class Notify;
class QAction;

struct MessageRecord;

class HistoryChatView : public QObject, public IChatViewHook
{
  Q_OBJECT

public:
  HistoryChatView(QObject *parent = 0);
  ~HistoryChatView();
  Q_INVOKABLE bool isAutoLoad(const QString &id) const;
  Q_INVOKABLE QString filter(const QString &id, const QString &text) const;
  Q_INVOKABLE QString genTag(const QStringList &messages) const;
  Q_INVOKABLE QString getTag(const QString &channel) const;
  Q_INVOKABLE void setTag(const QString &channel, const QString &tag) const;

  bool contextMenu(ChatView *view, QMenu *menu, const QWebHitTestResult &result) Q_DECL_OVERRIDE;
  void add(ChatView *view) Q_DECL_OVERRIDE;
  void init(ChatView *view) Q_DECL_OVERRIDE;
  void loadFinished(ChatView *view) Q_DECL_OVERRIDE;
  void retranslate() Q_DECL_OVERRIDE;

signals:
  void loading(const QString &id);

private slots:
  void notify(const Notify &notify);
  void pin();
  void ready();
  void remove();
  void settingsChanged(const QString &key, const QVariant &value);
  void synced();
  void unpin();

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
  QAction *pinAction(const QVariant &data);
  QAction *removeAction(const QVariant &data);
  QAction *unpinAction(const QVariant &data);
  void emulateLast(const QByteArray &channelId, const QList<QByteArray> &ids);
  void unpin(ChatView *view, QMenu *menu, const QString &messageId);

  bool m_autoLoad;   ///< Опция SETTINGS_HISTORY_AUTO_LOAD.
  QAction *m_pin;
  QAction *m_remove; ///< Удаление сообщения.
  QAction *m_unpin;
};

#endif /* HISTORYCHATVIEW_H_ */
