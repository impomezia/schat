/* $Id: WelcomeTab.h 3501 2013-02-14 07:16:38Z IMPOMEZIA $
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

#ifndef WELCOMETAB_H_
#define WELCOMETAB_H_

#include "ui/TabCreator.h"
#include "ui/tabs/AbstractTab.h"

class GenderField;
class LanguageField;
class NetworkWidget;
class NickEdit;
class QLabel;
class QPushButton;
class SimpleClient;

#define WELCOME_TAB "welcome"

/*!
 * Страница приветствия.
 */
class WelcomeTab : public AbstractTab
{
  Q_OBJECT

public:
  WelcomeTab(TabWidget *parent);

private:
  void retranslateUi();

  GenderField *m_genderField;   ///< Виджет выбора пола.
  LanguageField *m_languageBox; ///< Виджет выбора языка.
  NetworkWidget *m_network;    ///< Виджет выбора сети.
  NickEdit *m_nickEdit;         ///< Поле редактирования ника.
  QLabel *m_genderLabel;
  QLabel *m_languageLabel;
  QLabel *m_networkLabel;
  QLabel *m_nickLabel;
  QLabel *m_profileLabel;
};


class WelcomeTabCreator : public TabCreator
{
public:
  WelcomeTabCreator() : TabCreator() {}
  AbstractTab *create(const QVariant &data, TabWidget *parent) const;
  inline QByteArray name() const { return WELCOME_TAB; }
  void reload(AbstractTab *tab, const QVariant &data) const;
};

#endif /* WELCOMETAB_H_ */
