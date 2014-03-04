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

#ifndef HISTORYPLUGIN_P_H_
#define HISTORYPLUGIN_P_H_

#include "plugins/ChatPlugin.h"

class FeedNotify;
class HistoryPluginTr;
class Notify;

class HistoryImpl : public ChatPlugin
{
  Q_OBJECT

public:
  HistoryImpl(QObject *parent);
  ~HistoryImpl();
  static bool fetch(const QByteArray &id, const QList<QByteArray> &required);
  static bool get(const QByteArray &id, const QList<QByteArray> &messages);
  static bool since(const QByteArray &id, qint64 date);
  static QList<QByteArray> getLocal(const QList<QByteArray> &messages);
  void chatReady() Q_DECL_OVERRIDE;

private slots:
  void notify(const Notify &notify);
  void open();
  void ready();
  void start();

private:
  HistoryPluginTr *m_tr;
};


#endif /* HISTORYPLUGIN_P_H_ */
