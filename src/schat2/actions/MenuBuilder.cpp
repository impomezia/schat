/* $Id: MenuBuilder.cpp 3698 2013-06-17 13:41:51Z IMPOMEZIA $
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

#include <QDebug>

#include <QMenu>
#include <QUrl>

#include "actions/MenuBuilder.h"
#include "ChatUrls.h"

MenuBuilder::MenuBuilder(QObject *parent)
  : QObject(parent)
  , m_menu(0)
{
}


MenuBuilder::~MenuBuilder()
{
  qDebug() << " --- ~MenuBuilder()";
}


void MenuBuilder::bind(QMenu *menu)
{
  m_menu = menu;
  connect(m_menu, SIGNAL(triggered(QAction*)), SLOT(triggered(QAction*)));
  connect(m_menu, SIGNAL(destroyed(QObject*)), SLOT(deleteLater()));

  bindImpl();
}


void MenuBuilder::triggered(QAction *action)
{
  if (!m_menu)
    return;

  if (action->data().type() == QVariant::Url)
    ChatUrls::open(action->data().toUrl());
}


void MenuBuilder::bindImpl()
{

}
