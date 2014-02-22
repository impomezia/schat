/* $Id: ServiceApp.h 3152 2012-10-11 14:39:27Z IMPOMEZIA $
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

#ifndef SERVICEAPP_H_
#define SERVICEAPP_H_

#include <QPointer>

#include "qtservice/qtservice.h"

class NodeInit;

class ServiceApp : public QtService<QCoreApplication>
{
public:
  ServiceApp(int argc, char **argv);

protected:
  void start();
  void stop();

private:
  QPointer<NodeInit> m_init;
};

#endif /* SERVICEAPP_H_ */
