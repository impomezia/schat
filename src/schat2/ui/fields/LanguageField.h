/* $Id: LanguageField.h 1521 2011-07-10 23:22:16Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2011 IMPOMEZIA <schat@impomezia.com>
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

#ifndef LANGUAGEFIELD_H_
#define LANGUAGEFIELD_H_

#include "ui/LanguageBox.h"

class LanguageField : public LanguageBox
{
  Q_OBJECT

public:
  LanguageField(QWidget *parent = 0);

private slots:
  void indexChanged();
};

#endif /* LANGUAGEFIELD_H_ */
