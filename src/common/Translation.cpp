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

#include <QCoreApplication>
#include <QFileInfo>
#include <QLocale>
#include <QTranslator>
#include <QLibraryInfo>

#include "sglobal.h"
#include "Translation.h"

/*!
 * Конструктор класса Translation.
 */
Translation::Translation(QObject *parent)
  : QObject(parent),
  m_empty(true)
{
  m_language = tr("English");
  m_name = LS("en");
  m_prefix = LS("schat2_");
  m_core = new QTranslator(this);

  addOther(LS("qt"));
}


void Translation::addOther(const QString &name)
{
  if (m_others.contains(name))
    return;

  m_others[name] = new QTranslator(this);
  loadOther(name);
}


/*!
 * Загрузка языкового файла.
 * Допустимые значения для параметра \p name:
 * - auto или пустая строка, будет произведена попытка автоматически определить язык
 * в случае неудачи будет установлен английский язык.
 *
 * - Код языка.
 */
void Translation::load(const QString &name)
{
  clear();

  if (name == LS("auto") || name.isEmpty()) {
    QLocale locale = QLocale::system();
    if (locale.language() == QLocale::C)
      m_name = LS("en");
    else
#     if QT_VERSION >= 0x040800
      m_name = locale.uiLanguages().value(0).replace(QLatin1String("-"), QLatin1String("_"));
#     else
      m_name = locale.name();
#     endif
  }
  else if (name.endsWith(LS(".qm"))) {
    QFileInfo fileInfo = QFileInfo(name);
    m_name = fileInfo.baseName().mid(m_prefix.size());
    if (m_core->load(name)) {
      finalize();
      return;
    }
    else {
      load(m_name);
    }
  }
  else {
    m_name = name;
  }

  bool loaded = false;

  for (int i = 0; i < m_search.size(); ++i) {
    loaded = m_core->load(m_prefix + m_name, m_search.at(i));
    if (loaded)
      break;
  }

  if (loaded) {
    finalize();
  }
  else if (m_name != LS("en"))
    load(LS("en"));
}


/*!
 * Установка списка поиска файлов.
 */
void Translation::setSearch(const QStringList &search)
{
  m_search = search;
  m_search.removeAll(QString());
  m_search.removeDuplicates();
  m_search.append(QLibraryInfo::location(QLibraryInfo::TranslationsPath));
  m_search.append(LS(":/translations"));
}


void Translation::clear()
{
  if (m_empty)
    return;

  QCoreApplication::removeTranslator(m_core);
  foreach (QTranslator *other, m_others) {
    QCoreApplication::removeTranslator(other);
  }

  m_empty = true;
}


void Translation::finalize()
{
  m_empty = false;
  m_language = m_core->translate("Translation", "English");
  QCoreApplication::installTranslator(m_core);

  QMapIterator<QString, QTranslator*> i(m_others);
  while (i.hasNext()) {
    i.next();
    loadOther(i.key());
  }
}


void Translation::loadOther(const QString &name)
{
  if (m_empty)
    return;

  QTranslator *translator = m_others.value(name);
  if (!translator)
    return;

  for (int i = 0; i < m_search.size(); ++i) {
    if (translator->load(name + LS("_") + m_name, m_search.at(i))) {
      QCoreApplication::installTranslator(translator);
      return;
    }
  }
}
