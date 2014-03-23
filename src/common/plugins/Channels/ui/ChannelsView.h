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

#ifndef CHANNELSVIEW_H_
#define CHANNELSVIEW_H_

#include <QVariant>

#include "ui/tabs/WebView.h"

class Notify;

class ChannelsView : public WebView
{
  Q_OBJECT

public:
  ChannelsView(QWidget *parent = 0);
  Q_INVOKABLE QString toUrl(const QString &id, const QString &name) const;
  Q_INVOKABLE void addRoom();
  Q_INVOKABLE void join(const QString &name);

signals:
  void feed(const QVariantMap &data);
  void serverRenamed(const QString &name);

public slots:
  void create(const QString &id, const QString &name, bool _private);

protected:
  void contextMenu(QMenu *menu, const QWebHitTestResult &result);

private slots:
  void channel(const QByteArray &id);
  void notify(const Notify &notify);
  void populateJavaScriptWindowObject();

private:
  QMap<QByteArray, bool> m_channels; ///< Каналы ожидающих создания.
};

#endif /* CHANNELSVIEW_H_ */
