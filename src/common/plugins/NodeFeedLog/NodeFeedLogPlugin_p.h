/* $Id: NodeFeedLogPlugin_p.h 3412 2013-01-17 12:06:24Z IMPOMEZIA $
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

#ifndef NODEFEEDLOGPLUGIN_P_H_
#define NODEFEEDLOGPLUGIN_P_H_

#include <QFile>
#include <QTextStream>

#include "plugins/NodePlugin.h"

class FeedEvent;

class NodeFeedLogImpl : public NodePlugin
{
  Q_OBJECT

public:
  NodeFeedLogImpl(QObject *parent);

private slots:
  void notify(const FeedEvent &event);

private:
  QString date(qint64 date) const;
  void openLog(const QString &file);

  QFile m_file;           ///< Файл журнала.
  QTextStream m_stream;   ///< Текстовый поток для записи в журнал.
};

#endif /* NODEFEEDLOGPLUGIN_P_H_ */
