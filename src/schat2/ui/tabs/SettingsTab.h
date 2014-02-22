/* $Id: SettingsTab.h 3501 2013-02-14 07:16:38Z IMPOMEZIA $
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

#ifndef SETTINGSTAB_H_
#define SETTINGSTAB_H_

#include "ui/TabCreator.h"
#include "ui/tabs/AbstractTab.h"

class QListWidget;
class QListWidgetItem;
class QPushButton;
class QStackedWidget;
class QUrl;
class SettingsPage;

#define SETTINGS_TAB "settings"

class SettingsTab : public AbstractTab
{
  Q_OBJECT

public:
  SettingsTab(const QUrl &url, TabWidget *parent);
  void addPage(SettingsPage *page);
  void openUrl(const QUrl &url);

protected:
  void showEvent(QShowEvent *event);

private slots:
  void pageChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
  void retranslateUi();

  QList<QListWidgetItem *> m_items;
  QListWidget *m_contents;
  QStackedWidget *m_pages;
  QStringList m_ids; ///< Идентификаторы страниц настроек.
};


class SettingsTabCreator : public TabCreator
{
public:
  SettingsTabCreator() : TabCreator() {}
  AbstractTab *create(const QVariant &data, TabWidget *parent) const;
  inline QByteArray name() const { return SETTINGS_TAB; }
  void reload(AbstractTab *tab, const QVariant &data) const;
};

#endif /* SETTINGSTAB_H_ */
