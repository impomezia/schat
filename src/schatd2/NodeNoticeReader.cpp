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

#include "cores/Core.h"
#include "NodeNoticeReader.h"
#include "Storage.h"

QMap<int, NodeNoticeReader*> NodeNoticeReader::m_readers;

NodeNoticeReader::NodeNoticeReader(int type, Core *core)
  : m_core(core)
  , m_type(type)
  , m_storage(Storage::i())
{
  add(this);
}


NodeNoticeReader::~NodeNoticeReader()
{
  m_readers.remove(type());
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
  foreach (NodeNoticeReader *reader, m_readers) {
    reader->acceptImpl(user, result, packets);
  }
}


/*!
 * Добавление нового канала.
 */
void NodeNoticeReader::add(ChatChannel channel)
{
  foreach (NodeNoticeReader *reader, m_readers) {
    reader->addImpl(channel);
  }
}


void NodeNoticeReader::add(NodeNoticeReader *reader)
{
  Q_ASSERT(reader);

  m_readers.insert(reader->type(), reader);
}


void NodeNoticeReader::release(ChatChannel channel, quint64 socket)
{
  foreach (NodeNoticeReader *reader, m_readers) {
    reader->releaseImpl(channel, socket);
  }
}
