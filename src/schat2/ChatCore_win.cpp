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
#include <QDir>
#include <QProcess>

#include "ChatCore.h"
#include "sglobal.h"

void ChatCore::crashreport()
{
  const QString path = QApplication::applicationDirPath();

  QDir dir(path);

  const QStringList files = dir.entryList(QStringList(LS("*.dmp")), QDir::Files);
  if (files.isEmpty())
    return;

  QProcess::startDetached(LC('"') + path + LS("/crashreport.exe\""), QStringList(), path);
}
