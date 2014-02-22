/* $Id: SettingsTabHook.h 2974 2012-08-05 02:50:17Z IMPOMEZIA $
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

#ifndef SETTINGSTABHOOK_H_
#define SETTINGSTABHOOK_H_

#include <QWidget>
#include <QIcon>
#include <QMap>

#include "schat.h"

class ChatSettings;
class QVBoxLayout;

/*!
 * Базовый класс для страниц настроек.
 */
class SCHAT_CORE_EXPORT SettingsPage : public QWidget
{
  Q_OBJECT

public:
  SettingsPage(const QIcon &icon, const QString &id, QWidget *parent = 0);
  SettingsPage(QWidget *parent = 0);
  inline const QIcon& icon() const       { return m_icon; }
  inline const QString& id() const       { return m_id; }
  inline const QString& name() const     { return m_name; }
  inline QVBoxLayout *mainLayout() const { return m_mainLayout; }
  virtual void retranslateUi() {}

protected:
  void setupLayout();

  ChatSettings *m_settings;  ///< Указатель на объект настроек.
  QIcon m_icon;              ///< Иконка.
  QString m_id;              ///< Идентификатор.
  QString m_name;            ///< Имя страницы настроек.
  QVBoxLayout *m_mainLayout; ///< Главный компоновщик страницы.
};


class SettingsPageCreator
{
public:
  inline SettingsPageCreator(int weight = 0)
  : m_weight(weight)
  {}

  virtual ~SettingsPageCreator() {}
  inline int weight() const { return m_weight; }
  virtual SettingsPage* page(QWidget *) { return 0; }

private:
  int m_weight;
};


class SCHAT_CORE_EXPORT SettingsTabHook : public QObject
{
  Q_OBJECT

public:
  SettingsTabHook(QObject *parent = 0);
  ~SettingsTabHook();
  inline const QMap<int, SettingsPageCreator *>& pages() const { return m_pages; }
  inline static SettingsTabHook *i()                           { return m_self; }
  static void add(SettingsPage *page);
  static void add(SettingsPageCreator *creator);

signals:
  void added(const QString &id, SettingsPage *page);

private:
  void add(int weight, SettingsPageCreator *creator);

  QMap<int, SettingsPageCreator *> m_pages; ///< Страницы настроек.
  static SettingsTabHook *m_self;           ///< Указатель на себя.
};

#endif /* SETTINGSTABHOOK_H_ */
