/* $Id: ProfileLayout.h 2459 2012-03-29 15:12:00Z IMPOMEZIA $
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

#ifndef PROFILELAYOUT_H_
#define PROFILELAYOUT_H_

#include <QWidget>
#include <QMap>

class Notify;
class ProfileField;
class QGridLayout;
class QMenu;
class QToolButton;

/*!
 * Виджет для размещения полей профиля.
 */
class ProfileLayout : public QWidget
{
  Q_OBJECT

public:
  ProfileLayout(QWidget *parent = 0);
  inline QToolButton *button() { return m_button; }
  void retranslateUi();

public slots:
  void add(const QString &field);

private slots:
  void menuAboutToShow();
  void menuTriggered(QAction *action);
  void notify(const Notify &notify);
  void reload();

private:
  QStringList available() const;

  QGridLayout *m_layout;                 ///< Компоновщик.
  QMap<QString, ProfileField*> m_fields; ///< Добавленные поля.
  QMenu *m_menu;                         ///< Меню, добавления новых полей.
  QToolButton *m_button;                 ///< Кнопка добавления полей профиля.
};

#endif /* PROFILELAYOUT_H_ */
