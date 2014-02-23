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

#include <QtWin>
#include <QVBoxLayout>

#include "ChatSettings.h"
#include "ChatWindow.h"
#include "sglobal.h"

void ChatWindow::setupAppIcon()
{
}


void ChatWindow::stylize()
{
  if (m_settings->value(SETTINGS_WINDOWS_AERO).toBool() && QtWin::isCompositionEnabled()) {
    QtWin::extendFrameIntoClientArea(this, -1, -1, -1, -1);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_NoSystemBackground, false);
    m_mainLay->setMargin(0);

#   if QT_VERSION >= 0x050200
    setStyleSheet(LS("ChatWindow { background: transparent; }"));
#   endif
  }
  else {
    QtWin::resetExtendedFrame(this);
    setAttribute(Qt::WA_TranslucentBackground, false);
    m_mainLay->setContentsMargins(3, 3, 3, 0);

#   if QT_VERSION >= 0x050200
    setStyleSheet(QString("ChatWindow { background: %1; }").arg(QtWin::realColorizationColor().name()));
#   endif
  }
}
