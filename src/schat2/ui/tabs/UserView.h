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

#ifndef USERVIEW_H_
#define USERVIEW_H_

#include <QListView>
#include <QPointer>
#include <QStandardItemModel>

#include "Channel.h"

class ChannelInfo;
class Notify;
class UserItem;
class UserSortFilterModel;
class UserViewProperties;

/*!
 * Отображает список пользователей.
 */
class UserView : public QListView
{
  Q_OBJECT

public:
  UserView(ClientChannel channel, QWidget *parent = 0);
  bool add(ClientChannel user);
  bool reload(const QByteArray &id);
  bool remove(const QByteArray &id);
  inline bool contains(const QByteArray &id) const { return m_channels.contains(id); }
  inline bool sortable() const                     { return m_sortable; }
  inline int count() const                         { return m_channels.size(); }
  inline void setSortable(bool sortable)           { m_sortable = sortable; }
  void clear();
  void sort();

private slots:
  void channel(const ChannelInfo &info);
  void onSettingsChanged(const QString &key, const QVariant &value);

protected:
  void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;
  void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
  void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private slots:
  void addTab(const QModelIndex &index);
  void notify(const Notify &notify);

private:
  bool m_sortable;                        ///< true если список пользователей нужно сортировать при добавлении пользователя.
  ClientChannel m_channel;                ///< Канал.
  QMap<QByteArray, UserItem*> m_channels; ///< Таблица для ускоренного поиска пользователей.
  QPointer<UserViewProperties> m_dialog;
  QStandardItemModel *m_source;           ///< Модель для отображения списка пользователей
  UserSortFilterModel *m_model;
};

#endif /* USERVIEW_H_ */
