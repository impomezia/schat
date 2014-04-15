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

#ifndef SETTINGSTABIMPL_H_
#define SETTINGSTABIMPL_H_

#include "ui/tabs/SettingsTabHook.h"

class AlertsWidget;
class GenderField;
class LanguageField;
class NetworkWidget;
class NickEdit;
class ProfileLayout;
class QCheckBox;
class QLabel;

/*!
 * Страница настройки профиля.
 */
class ProfilePage : public SettingsPage
{
  Q_OBJECT

public:
  ProfilePage(QWidget *parent = 0);

private slots:
  void onAutoRunClicked(bool checked);

private:
  bool isAutoRun() const;
  void retranslateUi() Q_DECL_OVERRIDE;

  GenderField *m_genderField;
  NickEdit *m_nickEdit;
  ProfileLayout *m_layout;    ///< Виджет для размещения полей профиля.
  QCheckBox *m_autoRunBtn;
  QLabel *m_genderLabel;
  QLabel *m_nickLabel;
  QLabel *m_profileLabel;
};


class ProfilePageCreator : public SettingsPageCreator
{
public:
  inline ProfilePageCreator() : SettingsPageCreator(1000)        {}
  inline SettingsPage* page(QWidget *parent = 0) Q_DECL_OVERRIDE { return new ProfilePage(parent); }
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
  void retranslateUi() Q_DECL_OVERRIDE;

  NetworkWidget *m_network;
  QLabel *m_networkLabel;
};


class NetworkPageCreator : public SettingsPageCreator
{
public:
  inline NetworkPageCreator() : SettingsPageCreator(2000)        {}
  inline SettingsPage* page(QWidget *parent = 0) Q_DECL_OVERRIDE { return new NetworkPage(parent); }
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
  void retranslateUi() Q_DECL_OVERRIDE;

  AlertsWidget *m_alertsWidget;
  QLabel *m_label;
};


class AlertsPageCreator : public SettingsPageCreator
{
public:
  inline AlertsPageCreator() : SettingsPageCreator(2100)         {}
  inline SettingsPage* page(QWidget *parent = 0) Q_DECL_OVERRIDE { return new AlertsPage(parent); }
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
  void retranslateUi() Q_DECL_OVERRIDE;

  LanguageField *m_language; ///< Виджет выбора языка.
  QLabel *m_localeLabel;
};


class LocalePageCreator : public SettingsPageCreator
{
public:
  inline LocalePageCreator() : SettingsPageCreator(10000)        {}
  inline SettingsPage* page(QWidget *parent = 0) Q_DECL_OVERRIDE { return new LocalePage(parent); }
};

#endif /* SETTINGSTABIMPL_H_ */
