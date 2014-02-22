/* $Id: HistoryButton.cpp 3416 2013-01-18 03:48:52Z IMPOMEZIA $
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

#include <QCalendarWidget>
#include <QLocale>
#include <QMenu>
#include <QWidgetAction>

#include "ChatCore.h"
#include "client/ChatClient.h"
#include "client/ClientFeeds.h"
#include "HistoryButton.h"
#include "net/SimpleID.h"
#include "sglobal.h"
#include "Translation.h"
#include "ui/tabs/ChannelBaseTab.h"
#include "ui/tabs/ChatView.h"
#include "ui/TabWidget.h"

HistoryButton::HistoryButton(QWidget *parent)
  : QToolButton(parent)
{
  m_menu = new QMenu(this);

  setAutoRaise(true);
  setIcon(QIcon(LS(":/images/History/book.png")));
  setPopupMode(QToolButton::InstantPopup);
  setMenu(m_menu);

  retranslateUi();

  connect(m_menu, SIGNAL(aboutToHide()), SLOT(menuAboutToHide()));
  connect(m_menu, SIGNAL(aboutToShow()), SLOT(menuAboutToShow()));
}


void HistoryButton::changeEvent(QEvent *event)
{
  if (event->type() == QEvent::LanguageChange)
    retranslateUi();

  QToolButton::changeEvent(event);
}


void HistoryButton::activated(const QDate &date)
{
  m_menu->close();

  if (ChatClient::state() != ChatClient::Online)
    return;

# if QT_VERSION >= 0x040700
  qint64 start = QDateTime(date).toMSecsSinceEpoch();
# else
  qint64 start = qint64(QDateTime(date).toTime_t()) * 1000;
# endif

  m_dates[ChatCore::currentId()] = date;

  QVariantMap data;
  data[LS("date")] = start;
  data[LS("end")]  = start + 86400000;
  data[LS("day")]  = date.toString(LS("yyyy_MM_dd"));
  ClientFeeds::request(ChatCore::currentId(), FEED_METHOD_GET, LS("messages/since"), data);

  ChannelBaseTab *tab = TabWidget::i()->channelTab(ChatCore::currentId(), false, false);
  if (tab)
    tab->chatView()->evaluateJavaScript(LS("History.loading(Settings.id);"));
}


void HistoryButton::menuAboutToHide()
{
  QList<QAction *> actions = m_menu->actions();
  foreach (QAction *action, actions) {
    m_menu->removeAction(action);
    action->deleteLater();
  }
}


void HistoryButton::menuAboutToShow()
{
  QCalendarWidget *calendar = new QCalendarWidget(this);
  calendar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
  calendar->setMaximumDate(QDate::currentDate());
  calendar->setSelectedDate(m_dates.value(ChatCore::currentId(), QDate::currentDate()));

  QLocale locale(ChatCore::translation()->name());
  calendar->setLocale(locale);

# if QT_VERSION >= 0x040800
  calendar->setFirstDayOfWeek(locale.firstDayOfWeek());
# else
  QLocale::Language language = locale.language();
  if (language == QLocale::C || language == QLocale::English || language == QLocale::Hebrew)
    calendar->setFirstDayOfWeek(Qt::Sunday);
  else
    calendar->setFirstDayOfWeek(Qt::Monday);
# endif

  connect(calendar, SIGNAL(activated(QDate)), SLOT(activated(QDate)));

  QWidgetAction *action = new QWidgetAction(this);
  action->setDefaultWidget(calendar);
  m_menu->addAction(action);
}


void HistoryButton::retranslateUi()
{
  setToolTip(tr("History"));
}


HistoryAction::HistoryAction()
  : ToolBarActionCreator(10500, LS("history"), WidgetType | AutoShow | AutoDelete)
{
}


QWidget* HistoryAction::createWidget(QWidget *parent) const
{
  return new HistoryButton(parent);
}


bool HistoryAction::isVisible(const QString &type, const QByteArray &id)
{
  Q_UNUSED(type)

  if (SimpleID::typeOf(id) == SimpleID::UserId || SimpleID::typeOf(id) == SimpleID::ChannelId)
    return true;

  return false;
}


QIcon HistoryAction::icon() const
{
  return QIcon(LS(":/images/History/book.png"));
}


QString HistoryAction::title() const
{
  return tr("History");
}
