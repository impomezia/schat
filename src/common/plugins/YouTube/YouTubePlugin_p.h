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

#ifndef YOUTUBEPLUGIN_P_H_
#define YOUTUBEPLUGIN_P_H_

#include "plugins/ChatPlugin.h"

class QUrl;

class YouTubePluginImpl : public ChatPlugin
{
  Q_OBJECT

public:
  YouTubePluginImpl(QObject *parent);
  void chatReady() Q_DECL_OVERRIDE;

private slots:
  void settingsChanged(const QString &key, const QVariant &value);

private:
  const QString m_key;
};

#endif /* YOUTUBEPLUGIN_P_H_ */
