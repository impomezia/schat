/* $Id: Emoticons.cpp 3089 2012-09-16 15:35:14Z IMPOMEZIA $
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

#include <QFile>

#include "ChatCore.h"
#include "ChatSettings.h"
#include "Emoticons.h"
#include "Extension.h"
#include "JSON.h"
#include "sglobal.h"

Emoticons::Emoticons(QObject *parent)
  : QObject(parent)
{
}


/*!
 * Загрузка смайлов.
 */
bool Emoticons::load(Extension *extension)
{
  if (!extension)
    return false;

  QFile file(extension->root() + LS("/emoticons.json"));
  if (!file.exists())
    return false;

  if (!file.open(QIODevice::ReadOnly))
    return false;

  QVariantList data = JSON::parse(file.readAll()).toMap().value(LS("emoticons")).toList();
  if (data.isEmpty())
    return false;

  QList<Emoticon> theme;
  foreach (const QVariant &object, data) {
    Emoticon emoticon = Emoticon(new EmoticonData(extension->root(), extension->id(), object.toMap()));
    add(emoticon, theme);
  }

  m_themes[extension->id()] = theme;
  makeIndex();

  return true;
}


/*!
 * Получение смайла по текстовому сокращению.
 */
Emoticon Emoticons::get(const QString &key)
{
  return m_emoticons.value(key);
}


QString Emoticons::find(const QString &text, int pos)
{
  QChar c(text.at(pos));
  QMapIterator<QString, Emoticon> i(m_emoticons);
  while (i.hasNext()) {
    i.next();
    if (i.key().at(0) != c)
      continue;

    int index = text.indexOf(i.key());
    if (index != -1 && index == pos)
      return i.key();
  }

  return QString();
}


/*!
 * Получение списка тем смайлов.
 * Первой в списке идёт последняя загруженная тема.
 */
QStringList Emoticons::themes() const
{
  QStringList out;
  QStringList list = ChatCore::settings()->value(LS("Emoticons")).toStringList();
  for (int i = list.size() - 1; i >= 0; --i) {
    if (m_themes.contains(list.at(i)))
      out.append(list.at(i));
  }

  return out;
}


/*!
 * Добавление смайла.
 */
void Emoticons::add(Emoticon emoticon, QList<Emoticon> &theme)
{
  if (!emoticon->isValid())
    return;

  foreach (const QString &text, emoticon->texts()) {
    m_emoticons[text] = emoticon;
  }

  if (!emoticon->isHidden())
    theme.append(emoticon);
}


/*!
 * Создание индекса из первых символов смайлов.
 */
void Emoticons::makeIndex()
{
  m_index.clear();
  QMapIterator<QString, Emoticon> i(m_emoticons);
  while (i.hasNext()) {
    i.next();
    if (!m_index.contains(i.key().at(0)))
      m_index.append(i.key().at(0));
  }
}
