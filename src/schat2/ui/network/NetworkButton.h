/* $Id: NetworkButton.h 2250 2012-02-03 06:16:09Z IMPOMEZIA $
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

#ifndef NETWORKBUTTON_H_
#define NETWORKBUTTON_H_

#include <QWidget>

class QLabel;
class QProgressIndicator;
class QToolButton;

class NetworkButton : public QWidget
{
  Q_OBJECT

public:
  NetworkButton(const QString &text, QWidget *parent = 0);
  inline QToolButton *button() { return m_button; }
  void setError(const QString &error);
  void setProgress();
  void setReady(bool ready = true);
  void setText(const QString &text);

private:
  QLabel *m_error;                ///< Ошибка
  QProgressIndicator *m_progress; ///< Прогресс бар.
  QToolButton *m_button;          ///< Кнопка.
};

#endif /* NETWORKBUTTON_H_ */
