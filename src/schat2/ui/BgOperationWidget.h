/* $Id: BgOperationWidget.h 3377 2013-01-06 14:38:05Z IMPOMEZIA $
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

#ifndef BGOPERATIONWIDGET_H_
#define BGOPERATIONWIDGET_H_

#include <QWidget>

#include "schat.h"

class QProgressBar;
class QLabel;

class SCHAT_CORE_EXPORT BgOperationWidget : public QWidget
{
  Q_OBJECT

public:
  BgOperationWidget(QWidget *parent = 0);
  inline static BgOperationWidget *i()   { return m_self; }
  inline static QLabel *label()          { if (m_self) return m_self->m_label; return 0; }
  inline static QProgressBar *progress() { if (m_self) return m_self->m_progress; return 0; }
  static bool lock(const QString &key);
  static bool lock(const QString &key, const QString &text);
  static bool unlock(const QString &key, bool reset = false);
  static void setText(const QString &text);

signals:
  void clicked(const QString &key, QMouseEvent *event);

protected:
  void mouseReleaseEvent(QMouseEvent *event);

private:
  QProgressBar *m_progress;         ///< Прогресс бар для отображения фоновых операций.
  QLabel *m_label;                  ///< Поясняющий текст.
  QString m_lockKey;                ///< Ключ блокировки.
  static BgOperationWidget *m_self; ///< Указатель на себя.
};

#endif /* BGOPERATIONWIDGET_H_ */
