/* $Id: AlertMessage.h 3492 2013-02-11 03:48:50Z IMPOMEZIA $
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

#ifndef ALERTMESSAGE_H_
#define ALERTMESSAGE_H_

#include "messages/Message.h"

#define ALERT_MESSAGE_ERROR   QLatin1String("error")
#define ALERT_MESSAGE_INFO    QLatin1String("info")
#define ALERT_MESSAGE_SPINNER QLatin1String(" alert-box-spinner")
#define ALERT_MESSAGE_SUCCESS QLatin1String("success")
#define ALERT_MESSAGE_WARNING QLatin1String("warning")

class SCHAT_CORE_EXPORT AlertMessage : public Message
{
public:
  AlertMessage(const QString &text, const QString &extra = ALERT_MESSAGE_INFO);
  static QString show(const Message &message);
  static QString show(const QString &text, const QString &extra = ALERT_MESSAGE_INFO, const QByteArray &tab = QByteArray());
};

#endif /* ALERTMESSAGE_H_ */
