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

#ifndef IDLEPLUGIN_P_H_
#define IDLEPLUGIN_P_H_

#include "plugins/ChatPlugin.h"

class Idle;

class IdlePluginImpl : public ChatPlugin
{
  Q_OBJECT

public:
  IdlePluginImpl(QObject *parent);
  void chatReady() Q_DECL_OVERRIDE;

private slots:
  void idle(int seconds);
  void settingsChanged(const QString &key, const QVariant &value);
  void start();

private:
  void away(bool away = true);

  Idle *m_idle;   ///< Объект определяющий время бездействия.
  int m_autoAway; ///< Время в секундах до перехода в статус Auto Away.
};

#endif /* IDLEPLUGIN_P_H_ */
