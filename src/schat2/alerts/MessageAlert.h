/* $Id: MessageAlert.h 3139 2012-10-08 10:07:00Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2011 IMPOMEZIA <schat@impomezia.com>
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

#ifndef MESSAGEALERT_H_
#define MESSAGEALERT_H_

#include "alerts/AlertType.h"
#include "ChatAlerts.h"

class ChannelMessage;

class MessageAlert : public Alert
{
public:
  MessageAlert(const ChannelMessage &message);

private:
  void popup(const QVariantMap &data);
};


class MessageAlertType : public AlertType
{
public:
  MessageAlertType(const QString &type, int weight);
  QString name() const;
};

#endif /* MESSAGEALERT_H_ */
