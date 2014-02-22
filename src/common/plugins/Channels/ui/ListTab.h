/* $Id: ListTab.h 3501 2013-02-14 07:16:38Z IMPOMEZIA $
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

#ifndef LISTTAB_H_
#define LISTTAB_H_

#include "ui/TabCreator.h"
#include "ui/tabs/AbstractTab.h"

#define LIST_TAB "list"

class ChannelsView;

class ListTab : public AbstractTab
{
  Q_OBJECT

public:
  ListTab(TabWidget *parent);

private:
  void retranslateUi();

  ChannelsView *m_view;
};


class ListTabCreator : public TabCreator
{
public:
  ListTabCreator() : TabCreator() {}
  AbstractTab *create(const QVariant &data, TabWidget *parent) const;
  inline QByteArray name() const { return LIST_TAB; }
  void reload(AbstractTab *tab, const QVariant &data) const;
};

#endif /* LISTTAB_H_ */
