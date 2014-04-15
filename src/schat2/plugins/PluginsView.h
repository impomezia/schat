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

#ifndef PLUGINSVIEW_H_
#define PLUGINSVIEW_H_

#include <QVariant>

#include "ui/tabs/WebView.h"

/*!
 * Виджет просмотра списка плагинов.
 */
class PluginsView : public WebView
{
  Q_OBJECT

public:
  PluginsView(QWidget *parent = 0);
  Q_INVOKABLE int state(const QString &id);
  Q_INVOKABLE QVariantList list() const;
  Q_INVOKABLE void configure(const QString &id);
  Q_INVOKABLE void enable(const QString &id, bool enable);
  Q_INVOKABLE void restart();
  void scroll(const QString &id);

signals:
  void scrollTo(const QString &id);
  void settings(const QString &id);

private slots:
  void boot();
  void populateJavaScriptWindowObject();
  void restartSlot();

private:
  QString desc(const QVariantMap &data) const;
};

#endif /* PLUGINSVIEW_H_ */
