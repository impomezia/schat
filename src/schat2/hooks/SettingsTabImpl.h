/* $Id: SettingsTabImpl.h 3115 2012-09-23 03:45:44Z IMPOMEZIA $
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

#ifndef SETTINGSTABIMPL_H_
#define SETTINGSTABIMPL_H_

#include "ui/tabs/SettingsTabHook.h"

class AlertsWidget;
class GenderField;
class LanguageField;
class NetworkWidget;
class NickEdit;
class ProfileLayout;
class QLabel;

/*!
 * Страница настройки профиля.
 */
class ProfilePage : public SettingsPage
{
  Q_OBJECT

public:
  ProfilePage(QWidget *parent = 0);

private:
  void retranslateUi();

  GenderField *m_genderField;
  NickEdit *m_nickEdit;
  ProfileLayout *m_layout;    ///< Виджет для размещения полей профиля.
  QLabel *m_genderLabel;
  QLabel *m_nickLabel;
  QLabel *m_profileLabel;
};


class ProfilePageCreator : public SettingsPageCreator
{
public:
  ProfilePageCreator()
  : SettingsPageCreator(1000)
  {}

  SettingsPage* page(QWidget *parent = 0);
};


/*!
 * Страница настройки сетей.
 */
class NetworkPage : public SettingsPage
{
  Q_OBJECT

public:
  NetworkPage(QWidget *parent = 0);

private:
  void retranslateUi();

  NetworkWidget *m_network;
  QLabel *m_networkLabel;
};


class NetworkPageCreator : public SettingsPageCreator
{
public:
  NetworkPageCreator()
  : SettingsPageCreator(2000)
  {}

  SettingsPage* page(QWidget *parent = 0);
};


/*!
 * Страница настройки уведомлений.
 */
class AlertsPage : public SettingsPage
{
  Q_OBJECT

public:
  AlertsPage(QWidget *parent = 0);

private:
  void retranslateUi();

  AlertsWidget *m_alertsWidget;
  QLabel *m_label;
};


class AlertsPageCreator : public SettingsPageCreator
{
public:
  inline AlertsPageCreator() : SettingsPageCreator(2100) {}
  inline SettingsPage* page(QWidget *parent = 0)         { return new AlertsPage(parent); }
};


/*!
 * Страница настройки языка.
 */
class LocalePage : public SettingsPage
{
  Q_OBJECT

public:
  LocalePage(QWidget *parent = 0);

private:
  void retranslateUi();

  LanguageField *m_language; ///< Виджет выбора языка.
  QLabel *m_localeLabel;
};


class LocalePageCreator : public SettingsPageCreator
{
public:
  LocalePageCreator()
  : SettingsPageCreator(10000)
  {}

  SettingsPage* page(QWidget *parent = 0);
};

#endif /* SETTINGSTABIMPL_H_ */
