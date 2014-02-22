/* $Id: NetworkComboBox.h 3161 2012-10-13 10:23:24Z IMPOMEZIA $
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

#ifndef NETWORKCOMBOBOX_H_
#define NETWORKCOMBOBOX_H_

#include <QComboBox>

class NetworkWidget;
class Notify;
class OfflineLogin;

class NetworkComboBox : public QComboBox
{
  Q_OBJECT

public:
  NetworkComboBox(NetworkWidget *parent = 0);
  void load();
  void open();

public slots:
  void add(const QString &url = QLatin1String("schat://"));
  void edit();
  void remove();

private slots:
  void indexChanged(int index);
  void notify(const Notify &notify);

private:
  QIcon icon(const QString &provider);
  QString currentUrl() const;
  void updateIndex();

  NetworkWidget *m_network; ///< Виджет редактирования сети.
  QByteArray m_editing;     ///< Идентификатор редактируемой сети.
  QByteArray m_tmpId;       ///< Временный идентификатор для текущей редактируемой сети.
};

#endif /* NETWORKCOMBOBOX_H_ */
