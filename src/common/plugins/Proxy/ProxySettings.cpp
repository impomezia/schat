/* $Id: ProxySettings.cpp 3708 2013-06-23 23:40:16Z IMPOMEZIA $
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

#include <QCheckBox>
#include <QComboBox>
#include <QEvent>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QNetworkProxy>
#include <QPushButton>
#include <QSpinBox>
#include <QTimer>

#include "ChatCore.h"
#include "ChatSettings.h"
#include "client/ChatClient.h"
#include "client/SimpleClient.h"
#include "net/SimpleID.h"
#include "ProxySettings.h"
#include "sglobal.h"

ProxySettings::ProxySettings(QWidget *parent)
  : QWidget(parent)
{
  m_enable        = new QCheckBox(this);
  m_typeLabel     = new QLabel(this);
  m_typeBox       = new QComboBox(this);
  m_addressLabel  = new QLabel(this);
  m_addressEdit   = new QLineEdit(this);
  m_portLabel     = new QLabel(this);
  m_portBox       = new QSpinBox(this);
  m_nameLabel     = new QLabel(this);
  m_nameEdit      = new QLineEdit(this);
  m_passwordLabel = new QLabel(this);
  m_passwordEdit  = new QLineEdit(this);
  m_apply         = new QPushButton(this);

  QFont font = m_enable->font();
  font.setBold(true);
  m_enable->setFont(font);

  m_typeBox->addItem("HTTP");
  m_typeBox->addItem("SOCKS5");

  m_portBox->setRange(1, 65536);
  m_passwordEdit->setEchoMode(QLineEdit::Password);

  QHBoxLayout *authLay = new QHBoxLayout();
  authLay->addWidget(m_nameEdit);
  authLay->addWidget(m_passwordLabel);
  authLay->addWidget(m_passwordEdit);

  QGridLayout *mainLay = new QGridLayout(this);
  mainLay->addWidget(m_enable, 0, 0, 1, 5);
  mainLay->addWidget(m_typeLabel, 1, 0);
  mainLay->addWidget(m_typeBox, 1, 1);
  mainLay->addWidget(m_addressLabel, 2, 0);
  mainLay->addWidget(m_addressEdit, 2, 1, 1, 2);
  mainLay->addWidget(m_portLabel, 2, 3);
  mainLay->addWidget(m_portBox, 2, 4);
  mainLay->addWidget(m_nameLabel, 3, 0);
  mainLay->addLayout(authLay, 3, 1, 1, 2);
  mainLay->addWidget(m_apply, 3, 3, 1, 2);
  mainLay->setContentsMargins(10, 16, 0, 0);
  mainLay->setColumnStretch(2, 1);

  retranslateUi();

  load();
  setVisibleAll(m_enable->isChecked());
  addressChanged(m_addressEdit->text());

  connect(m_enable, SIGNAL(clicked(bool)),SLOT(enable(bool)));
  connect(m_apply, SIGNAL(clicked(bool)),SLOT(apply()));
  connect(m_typeBox, SIGNAL(currentIndexChanged(QString)),SLOT(reload(QString)));
  connect(m_addressEdit, SIGNAL(textChanged(QString)),SLOT(addressChanged(QString)));
}


void ProxySettings::setProxy()
{
  ChatSettings *settings = ChatCore::settings();
  const QString type = settings->value(LS("Proxy/Type")).toString();

  QNetworkProxy proxy;
  if (type == LS("http"))
    proxy.setType(QNetworkProxy::HttpProxy);
  else if (type == LS("socks5"))
    proxy.setType(QNetworkProxy::Socks5Proxy);
  else
    proxy.setType(QNetworkProxy::NoProxy);

  if (proxy.type() != QNetworkProxy::NoProxy) {
    proxy.setHostName(settings->value(LS("Proxy/")  + type + LS(".host")).toString());
    proxy.setPort(settings->value(LS("Proxy/")      + type + LS(".port")).toInt());
    proxy.setUser(settings->value(LS("Proxy/")      + type + LS(".name")).toString());
    proxy.setPassword(ChatId::fromBase32(settings->value(LS("Proxy/") + type + LS(".password")).toString().toLatin1()));
  }

  QNetworkProxy::setApplicationProxy(proxy);
}


void ProxySettings::changeEvent(QEvent *event)
{
  if (event->type() == QEvent::LanguageChange)
    retranslateUi();

  QWidget::changeEvent(event);
}


void ProxySettings::addressChanged(const QString &text)
{
  m_apply->setEnabled(!text.isEmpty());
}


void ProxySettings::apply()
{
  save();

  if (ChatClient::state() != ChatClient::Offline) {
    ChatClient::io()->leave();
    ChatClient::open();
  }
}


void ProxySettings::enable(bool enable)
{
  setVisibleAll(enable);
  if (enable)
    return;

  QNetworkProxy::ProxyType type = QNetworkProxy::applicationProxy().type();
  save();

  if (type != QNetworkProxy::NoProxy && ChatClient::state() != ChatClient::Offline) {
    ChatClient::io()->leave();
    ChatClient::open();
  }
}


/*!
 * Обновление отображаемых данных в зависимости от типа прокси.
 */
void ProxySettings::reload(const QString &type)
{
  QString t = type.toLower();
  ChatSettings *settings = ChatCore::settings();
  m_addressEdit->setText(settings->value(LS("Proxy/")  + t + LS(".host")).toString());
  m_portBox->setValue(settings->value(LS("Proxy/")     + t + LS(".port")).toInt());
  m_nameEdit->setText(settings->value(LS("Proxy/")     + t + LS(".name")).toString());
  m_passwordEdit->setText(ChatId::fromBase32(settings->value(LS("Proxy/") + t + LS(".password")).toString().toLatin1()));
}


void ProxySettings::save()
{
  ChatSettings *settings = ChatCore::settings();
  settings->setValue(LS("Proxy/Type"), m_enable->isChecked() ? m_typeBox->currentText().toLower() : LS("none"));

  const QString prefix = LS("Proxy/") + m_typeBox->currentText().toLower() + LS(".");
  settings->setValue(prefix + LS("host"), m_addressEdit->text());
  settings->setValue(prefix + LS("port"), m_portBox->value());
  settings->setValue(prefix + LS("name"), m_nameEdit->text());
  settings->setValue(prefix + LS("password"), QString(ChatId::toBase32(m_passwordEdit->text().toUtf8())));

  setProxy();
}


/*!
 * Загрузка конфигурации.
 */
void ProxySettings::load()
{
  const QString type = ChatCore::settings()->value(LS("Proxy/Type")).toString();
  if (type == LS("http")) {
    m_enable->setChecked(true);
  }
  else if (type == LS("socks5")) {
    m_enable->setChecked(true);
    m_typeBox->setCurrentIndex(1);
  }

  reload(m_typeBox->currentText());
}


void ProxySettings::retranslateUi()
{
  m_enable->setText(tr("Connect via proxy"));
  m_typeLabel->setText(tr("Type:"));
  m_addressLabel->setText(tr("Address:"));
  m_portLabel->setText(tr("Port:"));
  m_nameLabel->setText(tr("Name:"));
  m_passwordLabel->setText(tr("Password:"));
  m_apply->setText(tr("Apply"));

# if QT_VERSION >= 0x040700
  m_nameEdit->setPlaceholderText(tr("Optional"));
  m_passwordEdit->setPlaceholderText(tr("Optional"));
# endif
}


void ProxySettings::setVisibleAll(bool visible)
{
  m_typeLabel->setVisible(visible);
  m_typeBox->setVisible(visible);
  m_addressLabel->setVisible(visible);
  m_addressEdit->setVisible(visible);
  m_portLabel->setVisible(visible);
  m_portBox->setVisible(visible);
  m_nameLabel->setVisible(visible);
  m_nameEdit->setVisible(visible);
  m_passwordLabel->setVisible(visible);
  m_passwordEdit->setVisible(visible);
  m_apply->setVisible(visible);
}
