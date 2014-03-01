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

#ifndef DIALOGFRAME_H_
#define DIALOGFRAME_H_

#include <QFrame>

#include "schat.h"

class QHBoxLayout;
class QLabel;
class QVBoxLayout;
class QToolButton;

class SCHAT_CORE_EXPORT DialogFrame : public QFrame
{
  Q_OBJECT

public:
  DialogFrame(QWidget *parent = 0);
  QSize sizeHint() const Q_DECL_OVERRIDE;
  QString title() const;
  void setTitle(const QString &title);

protected:
  QFrame *line();
  virtual void retranslateUi() {}
  void changeEvent(QEvent *event) Q_DECL_OVERRIDE;
  void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

  QHBoxLayout *m_headerLayout;
  QLabel *m_titleLabel;
  QToolButton *m_closeBtn;
  QVBoxLayout *m_layout;
};

#endif // DIALOGFRAME_H_
