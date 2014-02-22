/* $Id: ToolBarAction.cpp 2769 2012-06-12 23:18:08Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2012 IMPOMEZIA <schat@impomezia.com>
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

#include "ui/ToolBarAction.h"

ToolBarActionCreator::ToolBarActionCreator(int weight, const QString &name, Flags flags)
  : m_flags(flags)
  , m_name(name)
  , m_weight(weight)
  , m_action(0)
{
}


bool ToolBarActionCreator::isVisible(const QString &type, const QByteArray &id)
{
  Q_UNUSED(type);
  Q_UNUSED(id);
  return true;
}


QAction* ToolBarActionCreator::createAction(QObject *parent) const
{
  Q_UNUSED(parent)
  return 0;
}


QWidget* ToolBarActionCreator::createWidget(QWidget *parent) const
{
  Q_UNUSED(parent)
  return 0;
}
