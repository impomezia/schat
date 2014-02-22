/* $Id: LanguageField.cpp 2457 2012-03-29 08:02:24Z IMPOMEZIA $
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

#include "ChatCore.h"
#include "ChatSettings.h"
#include "sglobal.h"
#include "Translation.h"
#include "ui/fields/LanguageField.h"

LanguageField::LanguageField(QWidget *parent)
  : LanguageBox(ChatCore::translation(), parent)
{
  connect(this, SIGNAL(currentIndexChanged(int)), SLOT(indexChanged()));
}


void LanguageField::indexChanged()
{
  if (save())
    ChatCore::settings()->setValue(LS("Translation"), m_translation->name());
}
