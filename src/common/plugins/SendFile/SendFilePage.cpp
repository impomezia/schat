/* $Id: SendFilePage.cpp 2974 2012-08-05 02:50:17Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2012 IMPOMEZIA <schat@impomezia.com>
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
#include <QVBoxLayout>
#include <QSpinBox>

#include "ChatCore.h"
#include "ChatSettings.h"
#include "SendFilePage.h"
#include "SendFilePlugin_p.h"
#include "sglobal.h"

SendFilePage::SendFilePage(SendFilePluginImpl *plugin, QWidget *parent)
  : SettingsPage(QIcon(LS(":/images/SendFile/attach.png")), LS("sendfile"), parent)
  , m_plugin(plugin)
{
  m_label = new QLabel(this);

  m_portLabel = new QLabel(this);
  m_port = new QSpinBox(this);
  m_port->setButtonSymbols(QSpinBox::NoButtons);
  m_port->setRange(1, 65536);
  m_port->setPrefix(LS(" "));
  m_port->setSuffix(LS(" "));
  m_port->setValue(SCHAT_OPTION(LS("SendFile/Port")).toInt());
  m_portLabel2 = new QLabel(this);

  m_warningIcon = new QLabel(this);
  m_warningIcon->setPixmap(QPixmap(LS(":/images/exclamation.png")));

  m_warningLabel = new QLabel(this);
  m_warningLabel->setWordWrap(true);
  setWarning(false);

  QHBoxLayout *portLay = new QHBoxLayout();
  portLay->addWidget(m_portLabel);
  portLay->addWidget(m_port);
  portLay->addWidget(m_portLabel2, 1);
  portLay->setContentsMargins(10, 0, 3, 0);

  QHBoxLayout *warningLay = new QHBoxLayout();
  warningLay->addWidget(m_warningIcon);
  warningLay->addWidget(m_warningLabel, 1);

  m_mainLayout = new QVBoxLayout(this);
  m_mainLayout->addWidget(m_label);
  m_mainLayout->addLayout(portLay);
  m_mainLayout->addStretch();
  m_mainLayout->addLayout(warningLay);

  retranslateUi();

  connect(m_port, SIGNAL(valueChanged(int)), SLOT(portChanged(int)));
}


void SendFilePage::retranslateUi()
{
  m_name = tr("File transfer");
  m_portLabel->setText(tr("Use port"));
  m_portLabel2->setText(tr("for file transfer"));
  m_warningLabel->setText(tr("Your changes will be applied the next time you start Simple Chat"));
  m_label->setText(LS("<b>") + m_name + LS("</b>"));
}


void SendFilePage::portChanged(int port)
{
  ChatCore::settings()->setValue(LS("SendFile/Port"), port);
  setWarning(m_plugin->setPort(port) != port);
}


void SendFilePage::setWarning(bool visible)
{
  m_warningIcon->setVisible(visible);
  m_warningLabel->setVisible(visible);
}


SettingsPage* SendFilePageCreator::page(QWidget *parent)
{
  return new SendFilePage(m_plugin, parent);
}
