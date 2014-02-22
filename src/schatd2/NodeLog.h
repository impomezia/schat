/* $Id: NodeLog.h 3722 2013-07-02 23:05:19Z IMPOMEZIA $
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

#ifndef NODELOG_H_
#define NODELOG_H_

#include <QDebug>
#include <QFile>
#include <QMutex>
#include <QStringList>
#include <QTextStream>

#include "schat.h"

class QSqlError;

class SCHAT_EXPORT NodeLog
{
public:
  enum Level {
    Disable = -1,
    FatalLevel,
    ErrorLevel,
    WarnLevel,
    InfoLevel,
    DebugLevel,
    TraceLevel
  };

  enum OutFlag {
    NoOut   = 0,
    FileOut = 1,
    StdOut  = 2
  };

  Q_DECLARE_FLAGS(OutFlags, OutFlag)

  NodeLog();
  bool open(const QString &file, Level level);
  inline static Level level()                     { return m_level; }
  inline static NodeLog *i()                      { return m_self; }
  inline static void setColors(bool colors)       { m_colors = colors; }
  inline static void setLevel(Level level)        { m_level = level; }
  inline static void setOutFlags(OutFlags output) { m_output = output; }
  static QString toString(const QSqlError &error);
  void add(Level level, const QString &code, const QString &tag, const QString &message);
  void add(Level level, const QString &message);

  class SCHAT_EXPORT Stream : public QTextStream
  {
  public:
    explicit Stream(Level level, const QString &code, const QString &tag)
      : QTextStream()
      , m_code(code)
      , m_tag(tag)
      , m_level(level)
    {
      setString(&m_buffer);
    }

    ~Stream() {
      NodeLog::i()->add(m_level, m_code, m_tag, m_buffer);
    }

  private:
    const QString &m_code;
    const QString &m_tag;
    Level m_level;
    QString m_buffer;
  };

  /// \deprecated Этот класс должен быть заменён на NodeLog::Stream.
  class SCHAT_EXPORT Helper
  {
  public:
    explicit Helper(Level level)
    : m_level(level)
    , m_debug(&m_buffer)
    {}

    ~Helper();

    inline QDebug& stream() { return m_debug; }

  private:
    Level m_level;
    QDebug m_debug;
    QString m_buffer;
  };

private:
  QString time() const;

  QFile m_file;             ///< Файл журнала.
  QMutex m_mutex;           ///< Мьютекс защищающий запись.
  QStringList m_levels;     ///< Текстовые обозначения уровней журналирования.
  QTextStream m_stdout;     ///< Текстовый поток для вывода в stdout;
  QTextStream m_stream;     ///< Текстовый поток для записи в журнал.
  static bool m_colors;     ///< \b true если используется цвета для вывода в stdout;
  static Level m_level;     ///< Уровень ведения журнала.
  static NodeLog *m_self;   ///< Указатель на себя.
  static OutFlags m_output; ///< Битовая маска каналов для вывода логов.
};

Q_DECLARE_OPERATORS_FOR_FLAGS(NodeLog::OutFlags)

#define LOG_FATAL(code, tag, x) if (NodeLog::level() >= NodeLog::FatalLevel) NodeLog::Stream(NodeLog::FatalLevel, QLatin1String(code), QLatin1String(tag)) << x ;
#define LOG_ERROR(code, tag, x) if (NodeLog::level() >= NodeLog::ErrorLevel) NodeLog::Stream(NodeLog::ErrorLevel, QLatin1String(code), QLatin1String(tag)) << x ;
#define LOG_WARN(code, tag, x)  if (NodeLog::level() >= NodeLog::WarnLevel)  NodeLog::Stream(NodeLog::WarnLevel,  QLatin1String(code), QLatin1String(tag)) << x ;
#define LOG_INFO(code, tag, x)  if (NodeLog::level() >= NodeLog::InfoLevel)  NodeLog::Stream(NodeLog::InfoLevel,  QLatin1String(code), QLatin1String(tag)) << x ;
#define LOG_DEBUG(code, tag, x) if (NodeLog::level() >= NodeLog::DebugLevel) NodeLog::Stream(NodeLog::DebugLevel, QLatin1String(code), QLatin1String(tag)) << x ;
#define LOG_TRACE(code, tag, x) if (NodeLog::level() >= NodeLog::TraceLevel) NodeLog::Stream(NodeLog::TraceLevel, QLatin1String(code), QLatin1String(tag)) << x ;

#endif /* NODELOG_H_ */
