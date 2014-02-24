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

#include <QFile>
#include <QIcon>

#include "ChatWindow.h"
#include "sglobal.h"

void ChatWindow::setupAppIcon()
{
  QIcon icon;
  QList<int> sizes;
  sizes << 16 << 22 << 24 << 32 << 36 << 48 << 64 << 72 << 96 << 128;

  foreach (const int size, sizes) {
    const QString file = QString(LS("/usr/share/icons/hicolor/%1x%1/apps/schat2.png")).arg(size);
    if (QFile::exists(file))
      icon.addFile(file, QSize(size, size));
  }

  if (icon.availableSizes().isEmpty())
    setWindowIcon(QIcon(LS(":/images/icons/48x48/schat2.png")));
  else
    setWindowIcon(icon);
}


void ChatWindow::stylize()
{
}
