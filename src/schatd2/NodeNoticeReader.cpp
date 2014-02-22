/* $Id: NodeNoticeReader.cpp 2413 2012-03-14 21:10:31Z IMPOMEZIA $
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

#include <QDebug>

#include "cores/Core.h"
#include "NodeNoticeReader.h"
#include "Storage.h"

QMap<int, QSharedPointer<NodeNoticeReader> > NodeNoticeReader::m_readers;

NodeNoticeReader::NodeNoticeReader(int type, Core *core)
  : m_core(core)
  , m_type(type)
  , m_storage(Storage::i())
{
  add(this);
}


/*!
 * Чтение пакета.
 *
 * \param type   Тип пакета Notice::Type.
 * \param reader Класс чтения пакета.
 *
 * \return true если не нужна дальнейшая пересылка пакета.
 */
bool NodeNoticeReader::read(int type, PacketReader *reader)
{
  if (!m_readers.contains(type))
    return true;

  return m_readers.value(type)->read(reader);
}


void NodeNoticeReader::accept(ChatChannel user, const AuthResult &result, QList<QByteArray> &packets)
{
  foreach (NodeNoticeReaderPtr reader, m_readers) {
    reader->acceptImpl(user, result, packets);
  }
}


/*!
 * Добавление нового канала.
 */
void NodeNoticeReader::add(ChatChannel channel)
{
  foreach (NodeNoticeReaderPtr reader, m_readers) {
    reader->addImpl(channel);
  }
}


void NodeNoticeReader::add(NodeNoticeReader *reader)
{
  m_readers[reader->type()] = NodeNoticeReaderPtr(reader);
}


void NodeNoticeReader::release(ChatChannel channel, quint64 socket)
{
  foreach (NodeNoticeReaderPtr reader, m_readers) {
    reader->releaseImpl(channel, socket);
  }
}
