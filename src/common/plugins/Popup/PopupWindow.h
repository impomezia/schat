/* $Id: PopupWindow.h 3630 2013-04-10 01:39:08Z IMPOMEZIA $
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

#ifndef POPUPWINDOW_H_
#define POPUPWINDOW_H_

#include <QFrame>

class Alert;
class QLabel;
class QTextBrowser;

class PopupWindow : public QFrame
{
  Q_OBJECT

public:
  /// Установки для фиксированного размера окна.
  enum Size {
    Width  = 280, ///< Ширина.
    Height = 80,  ///< Высота.
    Space  = 3    ///< Свободное пространство между окнами.
  };

  PopupWindow(const Alert &alert, int timeout, const QString &css);
  bool eventFilter(QObject *watched, QEvent *event);

private slots:
  void removed(const QByteArray &alertId);

private:
  QByteArray m_id;      ///< Уникальный идентификатор оповещения.
  QByteArray m_tab;     ///< Идентификатор вкладки канала.
  QLabel *m_date;       ///< Время уведомления.
  QLabel *m_icon;       ///< Иконка уведомления.
  QLabel *m_title;      ///< Заголовок уведомления.
  QTextBrowser *m_text; ///< Виджет отображения текста.
};

#endif /* POPUPWINDOW_H_ */
