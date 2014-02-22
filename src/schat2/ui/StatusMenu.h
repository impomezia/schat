/* $Id: StatusMenu.h 2596 2012-05-01 23:38:08Z IMPOMEZIA $
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

#ifndef STATUSMENU_H_
#define STATUSMENU_H_

#include <QMenu>

#include "schat.h"

class ChannelInfo;

class SCHAT_CORE_EXPORT StatusMenu : public QMenu
{
  Q_OBJECT

public:
  StatusMenu(QWidget *parent = 0);
  inline static StatusMenu *i() { return m_self; }
  static QString statusTitle(int status);
  static void apply(int status) { m_self->applyStatus(status); }

signals:
  void updated();

protected:
  void changeEvent(QEvent *event);

private slots:
  void channel(const ChannelInfo &info);
  void clientStateChanged();
  void settingsChanged(const QString &key, const QVariant &value);
  void statusChanged(QAction *action);

private:
  inline void retranslateUi() { reload(); }
  void addStatus(int status);
  void applyStatus(int status);
  void reload();

  QActionGroup *m_group;            ///< Группа для того чтобы можно было выбрать только один статус.
  QHash<int, QAction *> m_statuses; ///< Статусы.
  static StatusMenu *m_self;        ///< Указатель на себя.
};

#endif /* STATUSMENU_H_ */
