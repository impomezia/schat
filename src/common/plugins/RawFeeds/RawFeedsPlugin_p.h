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

#ifndef RAWFEEDSPLUGIN_P_H_
#define RAWFEEDSPLUGIN_P_H_

#include <QStringList>

#include "plugins/ChatPlugin.h"

class ChatView;

class RawFeeds : public ChatPlugin
{
  Q_OBJECT

public:
  RawFeeds(QObject *parent);
  inline bool isEnabled() const         { return m_enabled; }
  inline bool remove(const QString &id) { if (m_req.contains(id)) { m_req.removeAll(id); return true; } return false; }
  inline void add(const QString &id)    { if (!m_req.contains(id)) m_req.append(id); }
  inline void setEnabled(bool enabled)  { m_enabled = enabled; }
  void chatReady() Q_DECL_OVERRIDE;

private slots:
  void init(ChatView *view);
  void loadFinished(ChatView *view);

private:
  bool m_enabled;
  QStringList m_req;
};

#endif /* RAWFEEDSPLUGIN_P_H_ */
