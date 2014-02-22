/* $Id: SoundButton.h 3126 2012-09-29 05:04:36Z IMPOMEZIA $
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

#ifndef SOUNDBUTTON_H_
#define SOUNDBUTTON_H_

#include <QToolButton>

class SoundButton : public QToolButton
{
  Q_OBJECT

public:
  SoundButton(QWidget *parent = 0);
  void setMute(bool mute);

protected:
  void changeEvent(QEvent *event);

private slots:
  void clicked();
  void settingsChanged(const QString &key, const QVariant &value);

private:
  void retranslateUi();

  bool m_mute; ///< \a true если звук отключен.
};

#endif /* SOUNDBUTTON_H_ */
