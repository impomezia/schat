/* $Id: NodeFeedLogPlugin.cpp 3614 2013-04-02 22:51:15Z IMPOMEZIA $
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

#include <QtPlugin>
#include <QDir>

#include "DateTime.h"
#include "feeds/FeedEvents.h"
#include "feeds/FeedStrings.h"
#include "JSON.h"
#include "net/packets/Notice.h"
#include "net/SimpleID.h"
#include "NodeFeedLogPlugin.h"
#include "NodeFeedLogPlugin.h"
#include "NodeFeedLogPlugin_p.h"
#include "Path.h"
#include "sglobal.h"

NodeFeedLogImpl::NodeFeedLogImpl(QObject *parent)
  : NodePlugin(parent)
{
  QString path = Path::cache();
# if defined(Q_OS_UNIX) && !defined(Q_OS_MAC)
  if (!Path::isPortable())
    path = LS("/var/log/") + Path::app();
# endif

  openLog(path + LC('/') + Path::app() + LS(".feeds"));

  connect(FeedEvents::i(), SIGNAL(notify(FeedEvent)), SLOT(notify(FeedEvent)));
}


void NodeFeedLogImpl::notify(const FeedEvent &event)
{
  if (!m_file.isOpen())
    return;

  m_stream << date((event.method != FEED_METHOD_GET && event.status == Notice::OK) ? event.date : 0)
           << LC(' ') << event.status
           << LC(' ') << SimpleID::encode(event.sender)
           << LC('/') << qSetFieldWidth(10) << event.socket << qSetFieldWidth(0)
           << LC(' ') << qSetFieldWidth(6)  << event.method << qSetFieldWidth(0)
           << LC(' ') << event.date
           << LC(' ') << SimpleID::encode(event.channel)
           << LC('/') << qSetFieldWidth(40) << (event.name + (!event.path.isEmpty() ? LC('/') + event.path : QString())) << qSetFieldWidth(0) << qSetFieldWidth(0);

  if (event.diffTo && event.diffTo != event.date)
    m_stream << LS(" diffTo:") << event.diffTo;

  if (!event.request.isEmpty())
    m_stream << LS(" request:") << JSON::generate(event.request);

  if (!event.reply.isEmpty())
    m_stream << LS(" reply:") << JSON::generate(event.reply);

  m_stream << endl;
}


/*!
 * Возвращает время в виде строки для записи в журнал.
 *
 * \param date время, в случае если время 0 будет использовано текущее время.
 */
QString NodeFeedLogImpl::date(qint64 date) const
{
  const QDateTime dt = date ? DateTime::toDateTime(date) : QDateTime::currentDateTime();
  QDateTime utc(dt);
  utc.setTimeSpec(Qt::UTC);
  int seconds = dt.secsTo(utc);

  const QChar sign = (seconds >= 0 ? LC('+') : LC('-'));

  if (seconds < 0)
    seconds = -seconds;

  int minutes = (seconds % 3600) / 60;
  int hours = (seconds / 3600);

  QTime t(hours, minutes);
  return dt.toString(LS("yyyy-MM-dd hh:mm:ss.zzz")) + sign + t.toString(LS("hh:mm"));
}


/*!
 * Открытие журнала.
 */
void NodeFeedLogImpl::openLog(const QString &file)
{
  QDir dir(QFileInfo(file).absolutePath());
  if (!dir.exists())
    dir.mkpath(dir.absolutePath());

  bool bom = false;

  m_file.setFileName(file);
  if (!m_file.exists())
    bom = true;

  if (!m_file.open(QFile::WriteOnly | QFile::Text | QFile::Append))
    return;

  m_stream.setDevice(&m_file);
  m_stream.setGenerateByteOrderMark(bom);
  m_stream.setCodec("UTF-8");
  m_stream.setFieldAlignment(QTextStream::AlignLeft);
}


NodePlugin *NodeFeedLogPlugin::create()
{
  m_plugin = new NodeFeedLogImpl(this);
  return m_plugin;
}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2(NodeFeedLog, NodeFeedLogPlugin);
#endif
