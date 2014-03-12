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

#include "UserSortFilterModel.h"
#include "UserItem.h"

UserSortFilterModel::UserSortFilterModel(bool offlineUsers, QObject *parent)
  : QSortFilterProxyModel(parent)
  , m_offlineUsers(offlineUsers)
{
}


void UserSortFilterModel::setOfflineUsers(bool offline)
{
  m_offlineUsers = offline;

  invalidateFilter();
}


bool UserSortFilterModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
  Q_UNUSED(source_column)
  Q_UNUSED(source_parent)

  return true;
}


bool UserSortFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
  if (m_offlineUsers)
    return true;

  const QModelIndex index = sourceModel()->index(source_row, filterKeyColumn(), source_parent);
  if (!index.isValid())
    return true;

  return index.data(UserItem::StatusRole).toInt() != Status::Offline;
}
