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

#ifndef NODEPLUGINS_H_
#define NODEPLUGINS_H_

#include "Plugins.h"

class NodePlugin;

class SCHAT_EXPORT NodePlugins : public Plugins
{
  Q_OBJECT

public:
  NodePlugins(QObject *parent = 0);
  inline static NodePlugins *i() { return m_self; }

protected:
  void init();

private:
  QList<NodePlugin *> m_nodePlugins; ///< Список плагинов.
  static NodePlugins *m_self;        ///< Указатель на себя.
};

#endif /* NODEPLUGINS_H_ */
