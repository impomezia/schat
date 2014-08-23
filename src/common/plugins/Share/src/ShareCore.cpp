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

#include <QPluginLoader>

#include "ChatCore.h"
#include "ChatSettings.h"
#include "interfaces/IPlugin.h"
#include "Providers.h"
#include "sglobal.h"
#include "ShareButton.h"
#include "ShareChatView.h"
#include "ShareCore.h"
#include "Translation.h"
#include "ui/SendWidget.h"

IMPORT_PLUGIN(NoneProvider)
IMPORT_PLUGIN(RupProvider)
IMPORT_PLUGIN(ImgurProvider)
IMPORT_PLUGIN(GeekpicProvider)

const QString ShareCore::kProvider = LS("Share/Provider");

ShareCore::ShareCore(QObject *parent)
  : ChatPlugin(parent)
  , m_settings(ChatCore::settings())
{
  ChatCore::translation()->addOther(LS("share"));

  initProviders();
}


void ShareCore::chatReady()
{
  new ShareChatView(this);

  SendWidget::add(new ShareAction(this));
}


QObjectList ShareCore::getPlugins(const char *className) const
{
  QObjectList out;
  QObjectList instances = QPluginLoader::staticInstances();

  foreach (QObject *object, instances) {
    if (object->inherits(className))
      out.append(object);
  }

  return out;
}


void ShareCore::initProviders()
{
  m_providers = new Providers(this);
  QObjectList providers = getPlugins("IProvider");
  for (int i = 0; i < providers.size(); ++i) {
    IProvider *provider = qobject_cast<IProvider*>(providers.at(i));

    provider->init(m_settings);
    m_providers->add(provider);
  }

  m_settings->setDefault(kProvider, LS("rup"));
  m_providers->setCurrentId(m_settings->value(kProvider).toString());
}
