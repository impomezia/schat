/*   Copyright (C) 2013-2014 Alexander Sedov <imp@schat.me>
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

#include "NoneProviderPlugin.h"
#include "NoneUploader.h"
#include "sglobal.h"

int NoneProviderPlugin::maxImages() const
{
  return 1;
}


QIcon NoneProviderPlugin::icon() const
{
  return QIcon();
}


qint64 NoneProviderPlugin::maxSize() const
{
  return 0;
}


QString NoneProviderPlugin::id() const
{
  return LS("none");
}


QString NoneProviderPlugin::name() const
{
  return tr("None");
}


QVariant NoneProviderPlugin::data() const
{
  return QVariant();
}


QWidget *NoneProviderPlugin::settingsWidget(QWidget *parent)
{
  Q_UNUSED(parent)
  return 0;
}


Uploader *NoneProviderPlugin::uploader(QObject *parent) const
{
  return new NoneUploader(parent);
}


void NoneProviderPlugin::init(ISettings *settings)
{
  Q_UNUSED(settings);
}

Q_EXPORT_PLUGIN2(NoneProvider, NoneProviderPlugin);
