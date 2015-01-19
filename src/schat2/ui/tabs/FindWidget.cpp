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

#include <QAction>
#include <QHBoxLayout>
#include <QIcon>
#include <QKeyEvent>
#include <QLabel>
#include <QToolBar>

#include "ChatCore.h"
#include "ChatNotify.h"
#include "sglobal.h"
#include "ui/SLineEdit.h"
#include "ui/tabs/FindWidget.h"

FindWidget::FindWidget(QWidget *parent)
  : QFrame(parent)
{
  installEventFilter(this);

  m_toolBar = new QToolBar(this);
  m_toolBar->setIconSize(QSize(16, 16));

  QLabel *label = new QLabel(this);
  label->setPixmap(QPixmap(LS(":/images/search.png")));

  m_editFind = new SLineEdit(this);
  m_editFind->addWidget(label, LineEdit::LeftSide);
  m_editFind->setWidgetSpacing(3);
  m_editFind->installEventFilter(this);

  m_toolBar->addWidget(m_editFind);
  m_previous = m_toolBar->addAction(QIcon(LS(":/images/find_previous.png")), tr("Previous"));
  m_next = m_toolBar->addAction(QIcon(LS(":/images/find_next.png")), tr("Next"));
  m_toolBar->addSeparator();
  m_close = m_toolBar->addAction(QIcon(LS(":/images/stop.png")), tr("Close"));

  QHBoxLayout *mainLay = new QHBoxLayout(this);
  mainLay->addWidget(m_toolBar);

# if defined(Q_OS_MACX)
  mainLay->setMargin(0);
# else
  mainLay->setMargin(4);
# endif

# if defined(Q_OS_WIN32)
  setObjectName(LS("FindWidget"));
  setStyleSheet(QString("FindWidget { background-color:%1; }").arg(palette().color(QPalette::Window).name()));
# endif

  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

  connect(m_editFind, SIGNAL(returnPressed()), SLOT(find()));
  connect(m_editFind, SIGNAL(textChanged(QString)), SLOT(find()));
  connect(m_toolBar, SIGNAL(actionTriggered(QAction*)), SLOT(actionTriggered(QAction*)));
}


void FindWidget::setPalette(bool found)
{
  QPalette palette = m_editFind->palette();
  palette.setColor(QPalette::Active, QPalette::Base, found ? Qt::white : QColor(255, 102, 102));
  m_editFind->setPalette(palette);
}


QString FindWidget::text() const
{
  return m_editFind->text();
}


void FindWidget::find()
{
  emit find(m_editFind->text(), true);
}


void FindWidget::setFocus()
{
  m_editFind->setFocus();
}


void FindWidget::changeEvent(QEvent *event)
{
  if (event->type() == QEvent::LanguageChange)
    retranslateUi();

  QWidget::changeEvent(event);
}


void FindWidget::actionTriggered(QAction *action)
{

  if (action == m_close) {
    hide();
    emit hidden();

    ChatNotify::start(Notify::SetSendFocus);
  }
  else if (action == m_next)
    find();
  else if (action == m_previous)
    emit find(m_editFind->text(), false);
}


bool FindWidget::eventFilter(QObject *watched, QEvent *event)
{
  if (watched == this && event->type() == QEvent::KeyPress) {
    QKeyEvent *e = static_cast<QKeyEvent*>(event);

    if (e->matches(QKeySequence::Find) || e->matches(QKeySequence::FindNext)) {
      ChatNotify::start(Notify::Find, ChatCore::currentId());
      return true;
    }
    else if (e->matches(QKeySequence::FindPrevious)) {
      ChatNotify::start(Notify::FindPrevious, ChatCore::currentId());
      return true;
    }
    else if (e->key() == Qt::Key_Escape) {
      hide();
      emit hidden();

      ChatNotify::start(Notify::SetSendFocus);
      return true;
    }
  }
  else if (watched == m_editFind && event->type() == QEvent::KeyPress) {
    if (static_cast<QKeyEvent*>(event)->matches(QKeySequence::Copy)) {
      if (!m_editFind->hasSelectedText()) {
        ChatNotify::start(Notify::CopyRequest);
        return true;
      }
    }
  }

  return QWidget::eventFilter(watched, event);
}


void FindWidget::retranslateUi()
{
  m_next->setText(tr("Next"));
  m_previous->setText(tr("Previous"));
  m_close->setText(tr("Close"));
}
