/* $Id: EmoticonsPage.h 2716 2012-05-28 18:50:44Z IMPOMEZIA $
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

#ifndef EMOTICONSPAGE_H_
#define EMOTICONSPAGE_H_

#include "ui/tabs/SettingsTabHook.h"

class Emoticons;
class QLabel;

class EmoticonsPage : public SettingsPage
{
  Q_OBJECT

public:
  EmoticonsPage(Emoticons *emoticons, QWidget *parent = 0);

private:
  void retranslateUi();

  Emoticons *m_emoticons;
  QLabel *m_label;
};


class EmoticonsPageCreator : public SettingsPageCreator
{
public:
  EmoticonsPageCreator(Emoticons *emoticons)
  : SettingsPageCreator(5000)
  , m_emoticons(emoticons)
  {}

  SettingsPage* page(QWidget *parent = 0);

private:
  Emoticons *m_emoticons;
};

#endif /* EMOTICONSPAGE_H_ */
