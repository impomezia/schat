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

#include <QApplication>
#include <QDesktopWidget>

#include <qt_windows.h>

#include "PopupManager.h"
#include "ui/TabWidget.h"

bool PopupManager::isFullscreenAppActive() const
{
  BOOL ret;
  SystemParametersInfo(SPI_GETSCREENSAVERRUNNING, 0, &ret, 0);
  if (ret)
    return true;

  HWND hwnd = GetForegroundWindow();
  if (NULL == hwnd)
    return false;

  RECT rect;
  GetWindowRect(hwnd, &rect);

  return QRect(QPoint(rect.left, rect.top), QPoint(rect.right - 1, rect.bottom - 1)) == QApplication::desktop()->screenGeometry(TabWidget::i());
}
