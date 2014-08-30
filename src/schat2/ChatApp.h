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

#ifndef CHATAPP_H_
#define CHATAPP_H_

#ifdef SCHAT_NO_SINGLEAPP
# define QtSingleApplication QApplication
# include <QApplication>
#else
# include "qtsingleapplication/qtsingleapplication.h"
#endif

class ChatCore;
class ChatWindow;

#define CMDLINE_ARG_BASE QLatin1String("-base")
#define CMDLINE_ARG_EXEC QLatin1String("-exec")
#define CMDLINE_ARG_EXIT QLatin1String("-exit")
#define CMDLINE_ARG_HIDE QLatin1String("-hide")

class ChatApp : public QtSingleApplication
{
  Q_OBJECT

public:
  static bool restartRequired;

  ChatApp(int &argc, char **argv);
  ~ChatApp();
  bool isRunning();
  void start();
  void stop();

  static bool selfUpdate();

private slots:
  void handleMessage(const QString& message);
  void onRestartRequest();
  void restart();

private:
  QString value(const QString &cmdKey, const QStringList &arguments) const;

  ChatCore *m_core;     ///< Глобальный объект чата.
  ChatWindow *m_window; ///< Главное окно.
};

#endif /* CHATAPP_H_ */
