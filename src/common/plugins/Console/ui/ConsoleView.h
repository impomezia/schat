/* $Id: ConsoleView.h 3775 2013-08-24 03:41:07Z IMPOMEZIA $
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

#ifndef CONSOLEVIEW_H_
#define CONSOLEVIEW_H_

#include <QVariant>

#include "ui/tabs/WebView.h"

class Notify;

class ConsoleView : public WebView
{
  Q_OBJECT

public:
  ConsoleView(QWidget *parent = 0);
  Q_INVOKABLE bool setName(const QString &channelId, const QString &name);
  Q_INVOKABLE QString expirationText(int days) const;
  Q_INVOKABLE QString toPassword(const QString &password) const;
  Q_INVOKABLE void openUrl(const QString &url);

signals:
  void feed(const QVariantMap &data);
  void serverRenamed(const QString &name);

private slots:
  void notify(const Notify &notify);
  void populateJavaScriptWindowObject();
};

#endif /* CONSOLEVIEW_H_ */
