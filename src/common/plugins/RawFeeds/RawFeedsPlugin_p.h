/* $Id: RawFeedsPlugin_p.h 2732 2012-06-02 17:51:29Z IMPOMEZIA $
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

#ifndef RAWFEEDSPLUGIN_P_H_
#define RAWFEEDSPLUGIN_P_H_

#include "plugins/ChatPlugin.h"

class ChatView;

class RawFeeds : public ChatPlugin
{
  Q_OBJECT

public:
  RawFeeds(QObject *parent);
  inline bool isEnabled() const        { return m_enabled; }
  inline void setEnabled(bool enabled) { m_enabled = enabled; }

private slots:
  void init(ChatView *view);
  void loadFinished(ChatView *view);

private:
  bool m_enabled;
};

#endif /* RAWFEEDSPLUGIN_P_H_ */
