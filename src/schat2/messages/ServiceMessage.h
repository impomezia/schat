/* $Id: ServiceMessage.h 3492 2013-02-11 03:48:50Z IMPOMEZIA $
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

#ifndef SERVICEMESSAGE_H_
#define SERVICEMESSAGE_H_

#include <QCoreApplication>

#include "messages/Message.h"

class SCHAT_CORE_EXPORT ServiceMessage : public Message
{
  Q_DECLARE_TR_FUNCTIONS(ServiceMessage)

public:
  ServiceMessage(const QString &text, const QByteArray &user = QByteArray());
  bool isValid() const;

  static ServiceMessage joined(const QByteArray &user);
  static ServiceMessage part(const QByteArray &user);
  static ServiceMessage quit(const QByteArray &user);
};

#endif /* SERVICEMESSAGE_H_ */
