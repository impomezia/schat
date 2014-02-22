/* $Id: SendButton.cpp 3764 2013-08-12 23:01:53Z IMPOMEZIA $
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

#include <QEvent>
#include <QMenu>

#include "hooks/SendButton.h"
#include "sglobal.h"
#include "text/PlainTextFilter.h"
#include "ui/ChatIcons.h"
#include "ui/InputWidget.h"
#include "ui/SendWidget.h"

SendButton::SendButton(QWidget *parent)
  : QToolButton(parent)
{
  m_history = new QMenu(this);

  setAutoRaise(true);
  setIcon(SCHAT_ICON(Send));
  setPopupMode(QToolButton::MenuButtonPopup);
  setMenu(m_history);

  connect(m_history, SIGNAL(aboutToShow()), SLOT(showMenu()));
  connect(this, SIGNAL(clicked()), SendWidget::i()->input(), SLOT(send()));
  connect(SendWidget::i()->input(), SIGNAL(send(QString)), SLOT(clear()));
  connect(SendWidget::i()->input(), SIGNAL(reloaded()), SLOT(clear()));

  retranslateUi();
}


void SendButton::changeEvent(QEvent *event)
{
  if (event->type() == QEvent::LanguageChange)
    retranslateUi();

  QWidget::changeEvent(event);
}


void SendButton::clear()
{
  m_history->clear();
}


void SendButton::showItem()
{
  QAction *action = qobject_cast<QAction *>(sender());
  if (action)
    SendWidget::i()->input()->setMsg(action->data().toInt());
}


/*!
 * Показ меню истории отправленных сообщений.
 *
 * \todo Добавить кнопку очистки истории.
 */
void SendButton::showMenu()
{
  if (!m_history->isEmpty())
    return;

  const QStringList history = SendWidget::i()->input()->history();
  if (!history.isEmpty()) {
    const QFontMetrics fm = fontMetrics();
    QAction *action = 0;

    for (int i = history.size() - 1; i >= 0; --i) {
      action = m_history->addAction(fm.elidedText(PlainTextFilter::filter(history.at(i)), Qt::ElideMiddle, 150));
      action->setData(i);
      connect(action, SIGNAL(triggered()), SLOT(showItem()));
    }
  }

  if (m_history->isEmpty())
    m_history->addAction(tr("Empty"))->setEnabled(false);
}


void SendButton::retranslateUi()
{
  setToolTip(tr("Send"));
}


SendAction::SendAction()
  : ToolBarActionCreator(11000, LS("send"), WidgetType | AutoShow | AutoDelete)
{
}


QWidget* SendAction::createWidget(QWidget *parent) const
{
  return new SendButton(parent);
}


QIcon SendAction::icon() const
{
  return SCHAT_ICON(Send);
}


QString SendAction::title() const
{
  return tr("Send");
}
