/* $Id: CountryField.cpp 3741 2013-07-09 23:49:56Z IMPOMEZIA $
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

#include <QComboBox>
#include <QHBoxLayout>

#include "client/ChatClient.h"
#include "CountryField.h"
#include "feeds/Feed.h"
#include "feeds/UserFeed.h"
#include "ProfilePlugin_p.h"
#include "sglobal.h"
#include "Tr.h"
#include "ui/Spinner.h"

CountryField::CountryField(QWidget *parent)
  : ProfileField(LS("country"), parent)
{
  m_box = new QComboBox(this);
  m_spinner = new Spinner(this);

  m_mainLay = new QHBoxLayout(this);
  m_mainLay->setMargin(0);
  m_mainLay->setSpacing(0);
  m_mainLay->addWidget(m_box);
  m_mainLay->addWidget(m_spinner);

  load();

  connect(m_box, SIGNAL(currentIndexChanged(int)), SLOT(indexChanged(int)));
}


void CountryField::retranslateUi()
{
  ProfileField::retranslateUi();
  m_box->disconnect(this);

  QVariant data = m_box->itemData(m_box->currentIndex());
  m_box->removeItem(0);

  int size = m_box->count();
  for (int i = 0; i < size; ++i) {
    m_box->setItemText(i, ProfilePluginImpl::Countries::name(m_box->itemData(i).toString()));
  }

  m_box->model()->sort(0);
  QPixmap layout(LS(":/images/flags.png"));
  m_box->insertItem(0, icon(LS("zz"), layout), tr("Not selected"));
  m_box->setCurrentIndex(m_box->findData(data));

  connect(m_box, SIGNAL(currentIndexChanged(int)), SLOT(indexChanged(int)));
}


void CountryField::setData(const QVariant &value)
{
  m_spinner->stop();
  m_mainLay->setSpacing(0);

  if (value.type() != QVariant::String)
    return;

  QString code = value.toString();
  if (code.size() != 2)
    return;

  int index = m_box->findData(code);
  if (index == -1)
    index = 0;

  m_box->setCurrentIndex(index);
}


void CountryField::indexChanged(int index)
{
  QString code;
  if (index > 0)
    code = m_box->itemData(index).toString();

  if (apply(code)) {
    m_mainLay->setSpacing(4);
    m_spinner->start();
  }
}


QIcon CountryField::icon(const QString &code, const QPixmap &layout)
{
  QPoint point = pos(code);
  if (point.isNull())
    return QIcon();

  return QIcon(layout.copy(point.x(), point.y(), 16, 11));
}


QPoint CountryField::pos(const QString &code)
{
  QPoint point;
  if (code.size() != 2)
    return point;

  char x = code.at(1).toLatin1();
  if (x < 'a' || x > 'z')
    return point;

  char y = code.at(0).toLatin1();
  if (y < 'a' || y > 'z')
    return point;

  point.setX(16 * (x - 'a') + 16);
  point.setY(11 * (y - 'a') + 11);
  return point;
}


void CountryField::load()
{
  QStringList countries = ProfilePluginImpl::Countries::list();
  countries.removeAll(LS("aq"));
  countries.removeAll(LS("bl"));
  countries.removeAll(LS("bq"));
  countries.removeAll(LS("cw"));
  countries.removeAll(LS("gg"));
  countries.removeAll(LS("im"));
  countries.removeAll(LS("je"));
  countries.removeAll(LS("jr"));
  countries.removeAll(LS("mf"));
  countries.removeAll(LS("ss"));
  countries.removeAll(LS("sx"));

  const QPixmap layout(LS(":/images/flags.png"));

  foreach (QString code, countries) {
    m_box->addItem(icon(code, layout), ProfilePluginImpl::Countries::name(code), code);
  }

  m_box->model()->sort(0);

  QStringList list;
  FeedPtr feed = ChatClient::channel()->feed(FEED_NAME_USER, false);
  if (feed) {
    QMapIterator<QString, QVariant> i(feed->data().value(USER_FEED_CONNECTIONS_KEY).toMap());
    while (i.hasNext()) {
      i.next();
      const QString country = i.value().toMap().value(USER_FEED_GEO_KEY).toMap().value(USER_FEED_COUNTRY_KEY).toString().toLower();
      if (!country.isEmpty() && !list.contains(country))
        list.append(country);
    }
  }

  m_box->insertItem(0, icon(LS("zz"), layout), tr("Not selected"));

  if (!list.isEmpty()) {
    foreach (QString code, list)
      m_box->insertItem(1, icon(code, layout), ProfilePluginImpl::Countries::name(code), code);

    m_box->setCurrentIndex(1);
  }
  else
    m_box->setCurrentIndex(0);
}
