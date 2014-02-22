/* $Id: EmoticonsTabs.cpp 2717 2012-05-29 10:32:47Z IMPOMEZIA $
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

#include <QTabBar>

#include "ChatCore.h"
#include "Emoticons.h"
#include "EmoticonSelector.h"
#include "EmoticonsTabs.h"
#include "Extension.h"
#include "Extensions.h"
#include "sglobal.h"

EmoticonsTabs::EmoticonsTabs(Emoticons *emoticons, QWidget *parent)
  : QTabWidget(parent)
  , m_emoticons(emoticons)
{
  tabBar()->setVisible(false);
  setDocumentMode(true);

  QStringList themes = m_emoticons->themes();
  if (themes.isEmpty())
    return;

  foreach (const QString &theme, themes) {
    Extension* ext = ChatCore::extensions()->get(LS("emoticons/") + theme);
    if (ext)
      addTab(new EmoticonSelector(theme, m_emoticons, this), ext->name());
  }

  if (count() > 1) {
    tabBar()->setVisible(true);
    setDocumentMode(false);
  }
}
