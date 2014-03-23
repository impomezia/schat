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

#ifndef PROGRESSTAB_H_
#define PROGRESSTAB_H_

#include "ui/TabCreator.h"
#include "ui/tabs/AbstractTab.h"

class QProgressIndicator;

#define PROGRESS_TAB "progress"

class ProgressTab : public AbstractTab
{
  Q_OBJECT

public:
  ProgressTab(TabWidget *parent);

private:
  void retranslateUi() Q_DECL_OVERRIDE;

  QProgressIndicator *m_progress;
};


class ProgressTabCreator : public TabCreator
{
public:
  ProgressTabCreator() : TabCreator() {}
  AbstractTab *create(const QVariant &data, TabWidget *parent) const Q_DECL_OVERRIDE;
  inline QByteArray name() const Q_DECL_OVERRIDE { return PROGRESS_TAB; }
  void reload(AbstractTab *tab, const QVariant &data) const Q_DECL_OVERRIDE;
};

#endif /* PROGRESSTAB_H_ */
