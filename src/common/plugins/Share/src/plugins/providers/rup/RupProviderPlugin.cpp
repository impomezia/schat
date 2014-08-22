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

#include <QLabel>

#include "RupProviderPlugin.h"
#include "RupSettings.h"
#include "RupUploader.h"
#include "sglobal.h"
#include "interfaces/ISettings.h"

int RupProviderPlugin::maxImages() const
{
  return 1;
}


QIcon RupProviderPlugin::icon() const
{
  return QIcon(LS(":/images/providers/rup.png"));
}


qint64 RupProviderPlugin::maxSize() const
{
  return 10 * 1024 * 1024;
}


QString RupProviderPlugin::id() const
{
  return LS("rup");
}


QString RupProviderPlugin::name() const
{
  return LS("Rup.io");
}


QVariant RupProviderPlugin::data() const
{
  QVariantMap map;
  if (m_token.size() == 42) {
    map.insert(LS("token"), m_token);
  }

  return map;
}


QWidget *RupProviderPlugin::settingsWidget(QWidget *parent)
{
  RupSettings *widget = new RupSettings(m_token, parent);
  connect(widget, SIGNAL(changed(QString)), SLOT(onTokenChanged(QString)));

  return widget;
}


Uploader *RupProviderPlugin::uploader(QObject *parent) const
{
  return new RupUploader(parent);
}


void RupProviderPlugin::init(ISettings *settings)
{
  m_settings = settings;
  m_token = settings->value(id() + LS(".provider/Token")).toString();
}


void RupProviderPlugin::onTokenChanged(const QString &token)
{
  m_token = token;
  m_settings->setValue(id() + LS(".provider/Token"), token);
}

Q_EXPORT_PLUGIN2(RupProvider, RupProviderPlugin);
