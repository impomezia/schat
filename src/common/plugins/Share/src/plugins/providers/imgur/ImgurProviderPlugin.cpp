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

#include "ImgurProviderPlugin.h"
#include "ImgurUploader.h"
#include "sglobal.h"

int ImgurProviderPlugin::maxImages() const
{
  return 1;
}


QIcon ImgurProviderPlugin::icon() const
{
  return QIcon(LS(":/images/providers/imgur.png"));
}


qint64 ImgurProviderPlugin::maxSize() const
{
  return 5 * 1024 * 1024;
}


QString ImgurProviderPlugin::id() const
{
  return LS("imgur");
}


QString ImgurProviderPlugin::name() const
{
  return LS("Imgur.com");
}


QVariant ImgurProviderPlugin::data() const
{
  return QVariant();
}


QWidget *ImgurProviderPlugin::settingsWidget(QWidget *parent)
{
  Q_UNUSED(parent)
  return 0;
}


Uploader *ImgurProviderPlugin::uploader(QObject *parent) const
{
  return new ImgurUploader(parent);
}


void ImgurProviderPlugin::init(ISettings *settings)
{
  Q_UNUSED(settings);
}

Q_EXPORT_PLUGIN2(ImgurProvider, ImgurProviderPlugin);
