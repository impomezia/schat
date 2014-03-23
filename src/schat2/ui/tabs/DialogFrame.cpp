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

#include <QGraphicsDropShadowEffect>
#include <QKeyEvent>
#include <QLabel>
#include <QToolButton>
#include <QVBoxLayout>

#include "ChatNotify.h"
#include "DialogFrame.h"
#include "sglobal.h"
#include "ui/ChatIcons.h"

DialogFrame::DialogFrame(QWidget *parent)
  : QFrame(parent)
{
  setObjectName(LS("DialogFrame"));
  setStyleSheet(LS("DialogFrame{background-color:#fff;border: 1px solid #5b5b5b;border-radius: 5px;}"));
  setAttribute(Qt::WA_DeleteOnClose);
  setFocusPolicy(Qt::WheelFocus);

  QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
  shadow->setBlurRadius(10);
  shadow->setOffset(0);
  setGraphicsEffect(shadow);

  m_titleLabel = new QLabel(this);
  m_titleLabel->setWordWrap(true);

  QFont font = m_titleLabel->font();
  font.setPointSize(font.pointSize() * 1.3);
  font.setBold(true);

  m_titleLabel->setFont(font);

  m_closeBtn = new QToolButton(this);
  m_closeBtn->setAutoRaise(true);
  m_closeBtn->setIcon(SCHAT_ICON(Times));
  m_closeBtn->setToolTip(tr("Close"));

  m_headerLayout = new QHBoxLayout();
  m_headerLayout->addWidget(m_titleLabel);
  m_headerLayout->addWidget(m_closeBtn, 0, Qt::AlignTop | Qt::AlignRight);

  m_layout = new QVBoxLayout(this);
  m_layout->addLayout(m_headerLayout);

# if QT_VERSION >= 0x050000
  connect(m_closeBtn, &QToolButton::clicked, this, &DialogFrame::close);
# else
  connect(m_closeBtn, SIGNAL(clicked()), SLOT(close()));
# endif
}


DialogFrame::~DialogFrame()
{
  ChatNotify::start(Notify::SetSendFocus);
}


QSize DialogFrame::sizeHint() const
{
  QSize size = QFrame::sizeHint();

  if (size.width() < 500)
    size.setWidth(500);

  return size;
}


QString DialogFrame::title() const
{
  return m_titleLabel->text();
}


void DialogFrame::setTitle(const QString &title)
{
  m_titleLabel->setText(title);
}


QFrame *DialogFrame::line()
{
  QFrame *frame = new QFrame(this);
  frame->setMinimumHeight(1);
  frame->setStyleSheet(LS("border:1px solid #e5e5e5;"));
  return frame;
}


void DialogFrame::changeEvent(QEvent *event)
{
  if (event->type() == QEvent::LanguageChange) {
    m_closeBtn->setToolTip(tr("Close"));
    retranslateUi();
  }

  QFrame::changeEvent(event);
}


void DialogFrame::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Escape)
    close();
  else
    QFrame::keyPressEvent(event);
}
