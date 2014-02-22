/* $Id: NodeNoticeReader.h 2112 2012-01-07 07:00:49Z IMPOMEZIA $
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

#ifndef NODENOTICEREADER_H_
#define NODENOTICEREADER_H_

#include <QMap>
#include <QSharedPointer>

#include "ServerChannel.h"

class AuthResult;
class Core;
class PacketReader;
class Storage;

class SCHAT_EXPORT NodeNoticeReader
{
public:
  NodeNoticeReader(int type, Core *core);
  virtual ~NodeNoticeReader() {}

  inline int type() const { return m_type; }

  static bool read(int type, PacketReader *reader);
  static void accept(ChatChannel user, const AuthResult &result, QList<QByteArray> &packets);
  static void add(ChatChannel channel);
  static void add(NodeNoticeReader *reader);
  static void release(ChatChannel channel, quint64 socket);

protected:
  virtual bool read(PacketReader *reader) { Q_UNUSED(reader) return false; }
  virtual void acceptImpl(ChatChannel /*user*/, const AuthResult & /*result*/, QList<QByteArray> & /*packets*/) {}
  virtual void addImpl(ChatChannel user) { Q_UNUSED(user) }
  virtual void releaseImpl(ChatChannel user, quint64 socket) { Q_UNUSED(user) Q_UNUSED(socket) }

  Core *m_core;       ///< Ядро чата.
  int m_type;         ///< Тип поддерживаемых пакетов.
  Storage *m_storage; ///< Хранилище данных.

  static QMap<int, QSharedPointer<NodeNoticeReader> > m_readers;
};

typedef QSharedPointer<NodeNoticeReader> NodeNoticeReaderPtr;

#endif /* NODENOTICEREADER_H_ */
