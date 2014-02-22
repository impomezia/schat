/* $Id: ChatApi.h 3373 2013-01-05 13:30:36Z IMPOMEZIA $
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

#ifndef CHATAPI_H_
#define CHATAPI_H_

#include <QObject>

class ChatCore;
class ChatPlugin;

class ChatApi
{
public:
  virtual ~ChatApi() {}
  inline ChatPlugin *plugin() const    { return m_plugin; }
  virtual bool check() const           { return true; }
  virtual ChatPlugin *create() = 0;
  virtual QWidget *settings(QWidget *) { return 0; };

protected:
  ChatPlugin *m_plugin;
};

Q_DECLARE_INTERFACE(ChatApi, "me.schat.ChatApi/1.3");

#endif /* CHATAPI_H_ */
