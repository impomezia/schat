/* $Id: HunSpellChecker.cpp 3542 2013-03-01 23:42:49Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright © 2008-2013 IMPOMEZIA <schat@impomezia.com>
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

#include <QDir>
#include <QTextCodec>
#include <QThreadPool>
#include <QFile>

#if defined(SCHAT_EXTERNAL_HUNSPELL)
# include <hunspell/hunspell.hxx>
#else
# include "hunspell/hunspell.hxx"
#endif

#include "HunSpellChecker.h"
#include "sglobal.h"
#include "SpellChecker.h"

HunspellChecker::HunspellChecker(QObject *parent)
  : SpellBackend(parent)
{
  m_pool = new QThreadPool(this);
  m_pool->setMaxThreadCount(1);

  m_path = SpellChecker::path();
  m_personal = new HunspellPersonalDict(SpellChecker::personalPath());
}


HunspellChecker::~HunspellChecker()
{
  clear();

  delete m_personal;
}


bool HunspellChecker::add(const QString &word)
{
  return m_personal->add(word);
}


bool HunspellChecker::available() const
{
  return true;
}


bool HunspellChecker::isCorrect(const QString &word) const
{
  if (m_personal->hunspell() && m_personal->hunspell()->spell(word.toUtf8().constData()) != 0)
    return true;

  if (!m_mutex.tryLock())
    return false;

  if (m_list.isEmpty()) {
    m_mutex.unlock();
    return true;
  }

  foreach (Hunspell *dic, m_list) {
    QByteArray encoded = QTextCodec::codecForName(dic->get_dic_encoding())->fromUnicode(word);
    if (dic->spell(encoded.constData()) != 0) {
      m_mutex.unlock();
      return true;
    }
  }

  m_mutex.unlock();
  return false;
}


QStringList HunspellChecker::dictionaries() const
{
  QStringList dict;
  QDir dir(m_path);
  if (dir.exists()) {
    QStringList entryList = dir.entryList(QStringList(LS("*.dic")), QDir::Files);
    foreach(const QString &file, entryList) {
      if (!file.startsWith(LS("hyph_")))
        dict << file.left(file.size() - 4);
    }
  }
  return dict;
}


QStringList HunspellChecker::suggestions(const QString &word) const
{
  QStringList words;

  if (!m_mutex.tryLock())
    return words;

  foreach (Hunspell *dic, m_list) {
    char **sugglist    = 0;
    QTextCodec *codec  = QTextCodec::codecForName(dic->get_dic_encoding());
    QByteArray encoded = codec->fromUnicode(word);

    int count = dic->suggest(&sugglist, encoded.constData());
    for (int i = 0; i < count; ++i) {
      QString w = codec->toUnicode(sugglist[i]);
      if (!words.contains(w))
        words.append(w);
    }

    dic->free_list(&sugglist, count);
  }

  m_mutex.unlock();
  return words;
}


void HunspellChecker::queuedSuggestions(const QString &word) const
{
  HunspellSuggestions *task = new HunspellSuggestions(this, word);
  connect(task, SIGNAL(ready(QString,QStringList)), SIGNAL(suggestionsReady(QString,QStringList)));
  m_pool->start(task);
}


void HunspellChecker::setLangs(const QStringList &dicts)
{
  QStringList files;
  foreach (const QString &name, dicts) {
    files.append(m_path + LC('/') + name);
  }

  m_pool->start(new HunspellLoader(this, files));
}


/*!
 * Очистка загруженных словарей.
 */
void HunspellChecker::clear()
{
  m_mutex.lock();
  qDeleteAll(m_list);
  m_list.clear();
  m_mutex.unlock();
}


/*!
 * Загрузка словарей.
 */
void HunspellChecker::load(const QStringList &dicts)
{
  if (dicts.isEmpty())
    return;

  m_mutex.lock();
  foreach (const QString &name, dicts) {
    if (QFile::exists(name + LS(".dic"))) {
      Hunspell *dic = new Hunspell(QString(name + LS(".aff")).toLocal8Bit().constData(), QString(name + LS(".dic")).toLocal8Bit().constData());
      if (QTextCodec::codecForName(dic->get_dic_encoding()))
        m_list.append(dic);
      else
        delete dic;
    }
  }
  m_mutex.unlock();
}


HunspellLoader::HunspellLoader(HunspellChecker *hunspell, const QStringList &dicts)
  : QRunnable()
  , m_hunspell(hunspell)
  , m_dicts(dicts)
{
}


void HunspellLoader::run()
{
  m_hunspell->clear();
  m_hunspell->load(m_dicts);
}


HunspellSuggestions::HunspellSuggestions(const HunspellChecker *hunspell, const QString &word)
  : QObject()
  , QRunnable()
  , m_hunspell(hunspell)
  , m_word(word)
{
}


void HunspellSuggestions::run()
{
  emit ready(m_word, m_hunspell->suggestions(m_word));
}


HunspellPersonalDict::HunspellPersonalDict(const QString &path)
  : m_hunspell(0)
  , m_count(0)
{
  m_aff = new QFile(path + LS(".aff"));
  m_dic = new QFile(path + LS(".dic"));

  read();
}


HunspellPersonalDict::~HunspellPersonalDict()
{
  if (m_hunspell)
    delete m_hunspell;

  delete m_aff;
  delete m_dic;
}


bool HunspellPersonalDict::add(const QString &word)
{
  if (word.isEmpty())
    return false;

  if (!m_try.contains(word.at(0)))
    m_try.append(word.at(0));

  m_count++;
  m_data += word.toUtf8() + '\n';

  if (!write())
    return false;

  if (!m_hunspell)
    load();

  m_hunspell->add(word.toUtf8().constData());
  return true;
}


bool HunspellPersonalDict::open(QIODevice::OpenMode flags)
{
  if (!m_aff->open(flags) || !m_dic->open(flags)) {
    close();
    return false;
  }

  return true;
}


/*!
 * Чтение словаря.
 *
 * \return \b false если прочитать не удалось.
 */
bool HunspellPersonalDict::read()
{
  if (!open(QIODevice::ReadOnly))
    return false;

  if (m_aff->readLine().simplified() != "SET UTF-8") {
    close();
    return false;
  }

  QByteArray line = m_aff->readLine().simplified();
  if (line.startsWith("TRY "))
    m_try = line.mid(4);

  m_count = m_dic->readLine().simplified().toInt();
  if (!m_count) {
    close();
    return false;
  }

  m_data = m_dic->readAll();

  close();
  load();
  return true;
}


/*!
 * Запись словаря.
 */
bool HunspellPersonalDict::write()
{
  if (!open(QIODevice::WriteOnly))
    return false;

  m_aff->write("SET UTF-8\n");
  if (!m_try.isEmpty())
    m_aff->write("TRY " + m_try.toUtf8() + '\n');

  m_dic->write(QByteArray::number(m_count) + '\n');
  m_dic->write(m_data);

  close();
  return true;
}


/*!
 * Закрытие открытых файлов.
 */
void HunspellPersonalDict::close()
{
  if (m_aff->isOpen())
    m_aff->close();

  if (m_dic->isOpen())
    m_dic->close();
}


void HunspellPersonalDict::load()
{
  m_hunspell = new Hunspell(m_aff->fileName().toUtf8().constData(), m_dic->fileName().toUtf8().constData());
}
