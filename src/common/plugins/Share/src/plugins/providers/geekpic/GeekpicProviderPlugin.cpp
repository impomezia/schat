/* Copyright (c) 2008-2014 Alexander Sedov <imp@schat.me>
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

#include "GeekpicProviderPlugin.h"
#include "GeekpicUploader.h"
#include "sglobal.h"

int GeekpicProviderPlugin::maxImages() const
{
  return 1;
}


QIcon GeekpicProviderPlugin::icon() const
{
  return QIcon(LS(":/images/providers/geekpic.png"));
}


qint64 GeekpicProviderPlugin::maxSize() const
{
  return 5 * 1024 * 1024;
}


QString GeekpicProviderPlugin::id() const
{
  return LS("geekpic");
}


QString GeekpicProviderPlugin::name() const
{
  return LS("Geekpic.net");
}


QVariant GeekpicProviderPlugin::data() const
{
  return QVariant();
}


QWidget *GeekpicProviderPlugin::settingsWidget(QWidget *parent)
{
  Q_UNUSED(parent)
  return 0;
}


Uploader *GeekpicProviderPlugin::uploader(QObject *parent) const
{
  return new GeekpicUploader(parent);
}


void GeekpicProviderPlugin::init(ISettings *settings)
{
  Q_UNUSED(settings);
}

Q_EXPORT_PLUGIN2(GeekpicProvider, GeekpicProviderPlugin);
