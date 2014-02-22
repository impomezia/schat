/* $Id: SendFileAction.cpp 2814 2012-07-01 17:21:35Z IMPOMEZIA $
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

#include <QAction>

#include "net/SimpleID.h"
#include "SendFileAction.h"
#include "SendFilePlugin_p.h"
#include "sglobal.h"

SendFileAction::SendFileAction(SendFilePluginImpl *plugin)
  : ToolBarActionCreator(1200, LS("file"), ActionType | AutoShow | AutoDelete)
  , m_plugin(plugin)
{
}


bool SendFileAction::isVisible(const QString &type, const QByteArray &id)
{
  Q_UNUSED(type)

  if (SimpleID::typeOf(id) == SimpleID::UserId)
    return true;

  return false;
}


QAction* SendFileAction::createAction(QObject *parent) const
{
   QAction *action = new QAction(icon(), title(), parent);
   QObject::connect(action, SIGNAL(triggered()), m_plugin, SLOT(sendFile()));
   return action;
}


QIcon SendFileAction::icon() const
{
  return QIcon(LS(":/images/SendFile/attach.png"));
}


QString SendFileAction::title() const
{
  return tr("Send file");
}
