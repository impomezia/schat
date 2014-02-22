/* $Id: UpdateWidget.cpp 3384 2013-01-08 15:49:07Z IMPOMEZIA $
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

#include <QHBoxLayout>
#include <QLabel>

#include "sglobal.h"
#include "ui/Spinner.h"
#include "UpdatePlugin_p.h"
#include "UpdateWidget.h"
#include "WebBridge.h"

#define OK_PIXMAP    m_spinner->stop(); m_icon->show(); m_icon->setPixmap(QPixmap(LS(":/images/Update/ok.png")));
#define ERROR_PIXMAP m_spinner->stop(); m_icon->show(); m_icon->setPixmap(QPixmap(LS(":/images/exclamation-red.png")));

UpdateWidget::UpdateWidget(UpdatePluginImpl *plugin, QWidget *parent)
  : QFrame(parent)
  , m_plugin(plugin)
{
  m_spinner = new Spinner(this);
  m_spinner->hide();

  m_icon = new QLabel(this);
  m_icon->setVisible(false);

  m_text = new QLabel(this);
  m_text->setWordWrap(true);

  QHBoxLayout *mainLay = new QHBoxLayout(this);
  mainLay->addWidget(m_spinner);
  mainLay->addWidget(m_icon);
  mainLay->addWidget(m_text, 1);
  mainLay->setSpacing(2);
  mainLay->setContentsMargins(125, 0, 0, 5);

  setStyleSheet(LS("QFrame { background-color: #fff; }"));
  connect(m_plugin, SIGNAL(done(int)), SLOT(reload()));

  if (m_plugin->state() == UpdatePluginImpl::Idle && m_plugin->status() != UpdatePluginImpl::UpdateReady) {
    m_spinner->start();
    m_text->setText(tr("Checking for updates..."));
    m_plugin->check();
  }
  else
    reload();
}


void UpdateWidget::reload()
{
  int status = m_plugin->status();
  int state  = m_plugin->state();

  if (status == UpdatePluginImpl::CheckError) {
    m_text->setText(tr("An error occurred while checking for updates."));

    ERROR_PIXMAP
  }
  if (status == UpdatePluginImpl::NoUpdates) {
    m_text->setText(tr("Simple Chat is up to date."));

    OK_PIXMAP
  }
  else if (status == UpdatePluginImpl::UpdateAvailable) {
    if (state == UpdatePluginImpl::DownloadUpdate) {
      m_text->setText(tr("Downloading update..."));
      m_spinner->start();
      m_icon->hide();
    }
    else {
      if (m_plugin->info().size)
        m_text->setText(tr("New version of Simple Chat available. Version <b>%1</b>, size <b>%2</b>.").arg(m_plugin->info().version, WebBridge::i()->bytesToHuman(m_plugin->info().size, false)));
      else
        m_text->setText(tr("New version of Simple Chat available. Version <b>%1</b>.").arg(m_plugin->info().version));

      OK_PIXMAP
    }
  }
  else if (status == UpdatePluginImpl::DownloadError) {
    m_text->setText(tr("An error occurred when downloading updates."));

    ERROR_PIXMAP
  }
  else if (status == UpdatePluginImpl::UpdateReady) {
    m_text->setText(tr("Update ready to install."));

    OK_PIXMAP
  }

}
