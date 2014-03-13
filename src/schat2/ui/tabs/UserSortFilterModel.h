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

#ifndef USERSORTFILTERMODEL_H_
#define USERSORTFILTERMODEL_H_

#include <QSortFilterProxyModel>

#include "schat.h"

class UserSortFilterModel : public QSortFilterProxyModel
{
  Q_OBJECT

public:
  UserSortFilterModel(bool offlineUsers, QObject *parent = 0);
  inline void invalidateFilter() { QSortFilterProxyModel::invalidateFilter(); }
  void setOfflineUsers(bool offline);

protected:
  bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const Q_DECL_OVERRIDE;
  bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const Q_DECL_OVERRIDE;

private:
  bool m_offlineUsers;
};

#endif // USERSORTFILTERMODEL_H_
