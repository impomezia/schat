/* $Id: HunSpellChecker.h 2979 2012-08-07 00:59:43Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2012 IMPOMEZIA <schat@impomezia.com>
 * Copyright © 2012 Alexey Ivanov <alexey.ivanes@gmail.com>
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

#ifndef HUNSPELLCHECKER_H
#define HUNSPELLCHECKER_H

#include <QIODevice>
#include <QMap>
#include <QMutex>
#include <QRunnable>

#include "SpellBackend.h"

class Hunspell;
class HunspellPersonalDict;
class QThreadPool;
class QFile;

class HunspellChecker: public SpellBackend
{
public:
  HunspellChecker(QObject *parent = 0);
  ~HunspellChecker();

  bool add(const QString &word);
  bool available() const;
  bool isCorrect(const QString &word) const;
  QStringList dictionaries() const;
  QStringList suggestions(const QString &word) const;
  void queuedSuggestions(const QString &word) const;
  void setLangs(const QStringList &dicts);

  void clear();
  void load(const QStringList &dicts);

private:
  HunspellPersonalDict *m_personal; ///< Пользовательский словарь.
  mutable QMutex m_mutex;           ///< Мьютекс защищающий доступ к словарям.
  QList<Hunspell*> m_list;          ///< Список загруженных объектов Hunspell.
  QString m_path;                   ///< Путь для поиска файлов словарей.
  QThreadPool *m_pool;              ///< Пул для запуска потоков.
};


class HunspellLoader : public QRunnable
{
public:
  HunspellLoader(HunspellChecker *hunspell, const QStringList &dicts);
  void run();

private:
  HunspellChecker *m_hunspell; ///< Указатель на объект HunspellChecker.
  QStringList m_dicts;         ///< Полные пути к файлас словарей, без расширений.
};


class HunspellSuggestions : public QObject, public QRunnable
{
  Q_OBJECT

public:
  HunspellSuggestions(const HunspellChecker *hunspell, const QString &word);
  void run();

signals:
  void ready(const QString &word, const QStringList &words);

private:
  const HunspellChecker *m_hunspell; ///< Указатель на объект HunspellChecker.
  QString m_word;                    ///< Слово для которого необходимо подобрать варианты.
};


class HunspellPersonalDict
{
public:
  HunspellPersonalDict(const QString &path);
  ~HunspellPersonalDict();
  bool add(const QString &word);
  inline Hunspell *hunspell() const { return m_hunspell; }

private:
  bool open(QIODevice::OpenMode flags);
  bool read();
  bool write();
  void close();
  void load();

  Hunspell *m_hunspell; ///< Словарь, может быть 0 если не загружен, в случае если пользователь не добавлял свои слова.
  int m_count;          ///< Число файлов в словаре.
  QByteArray m_data;    ///< Тело DIC файла.
  QFile *m_aff;         ///< AFF файл.
  QFile *m_dic;         ///< DIC файл.
  QString m_try;        ///< TRY опция в AFF файле.
};

#endif
