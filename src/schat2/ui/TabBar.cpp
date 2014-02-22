/* $Id: TabBar.cpp 3699 2013-06-18 23:29:34Z IMPOMEZIA $
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

#include <QProxyStyle>
#include <QMouseEvent>

#include "sglobal.h"
#include "ui/TabBar.h"

#if defined(Q_OS_WIN32)
# include <qt_windows.h>
#endif

class TabStyle : public QProxyStyle
{
public:
  TabStyle(QStyle *style = 0)
  : QProxyStyle(style)
  {}

  int pixelMetric(PixelMetric metric, const QStyleOption *option = 0, const QWidget *widget = 0) const
  {
    int result = QProxyStyle::pixelMetric(metric, option, widget);
    if (metric == PM_TabBarTabHSpace)
      return result / 2;

#   if defined(Q_OS_WIN32)
    else if (metric == PM_TabBarTabVSpace)
      return result + 2;
    else if (metric == PM_TabCloseIndicatorWidth || metric == PM_TabCloseIndicatorHeight)
      return 14;
#   endif

    return result;
  }
};


TabBar::TabBar(QWidget *parent)
  : QTabBar(parent)
{
  setMovable(true);
  setTabsClosable(true);
  setUsesScrollButtons(true);
  setElideMode(Qt::ElideRight);

  setStyle(new TabStyle());

# if defined(Q_OS_WIN32)
  setStyleSheet(LS("QTabBar::close-button { image: url(:/images/close.png) } QTabBar::close-button:hover { image: url(:/images/close-hover.png) }"));
# endif
}


TabBar::ButtonPosition TabBar::closeButtonPosition() const
{
  return static_cast<TabBar::ButtonPosition>(style()->styleHint(QStyle::SH_TabBar_CloseButtonPosition));
}


void TabBar::mousePressEvent(QMouseEvent *event)
{
# if defined(Q_OS_WIN32) && QT_VERSION < 0x050000
  if (event->button() == Qt::LeftButton && tabAt(event->pos()) == -1) {
    ReleaseCapture();
    PostMessage(parentWidget()->parentWidget()->parentWidget()->winId(), WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
  }
# endif

  QTabBar::mousePressEvent(event);
}
