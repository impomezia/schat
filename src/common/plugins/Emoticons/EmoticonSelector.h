/* $Id: EmoticonSelector.h 2718 2012-05-29 13:52:14Z IMPOMEZIA $
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

#ifndef EMOTICONSELECTOR_H_
#define EMOTICONSELECTOR_H_

#include <QLabel>

#include "EmoticonData.h"

class Emoticons;
class QGridLayout;

class EmoticonLabel : public QLabel
{
  Q_OBJECT

public:
  EmoticonLabel(Emoticon emoticon, QWidget *parent);

protected:
  void enterEvent(QEvent *event);
  void leaveEvent(QEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

private:
  void updateStyleSheet(bool hover = false);

  bool m_ready;
  QString m_text;
};


class EmoticonSelector : public QFrame
{
  Q_OBJECT

public:
  EmoticonSelector(const QString &theme, Emoticons *emoticons, QWidget *parent = 0);

private:
  void fill();

  Emoticons *m_emoticons; ///< Указатель на менеджер смайлов.
  QGridLayout *m_layout;  ///< Компоновщик смайлов.
  QString m_theme;        ///< Идентификатор темы смайликов.
};

#endif /* EMOTICONSELECTOR_H_ */
