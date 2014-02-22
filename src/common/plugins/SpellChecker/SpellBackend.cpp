/* $Id: SpellBackend.cpp 2969 2012-08-04 01:54:38Z IMPOMEZIA $
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

#include <QCoreApplication>

#include "HunSpellChecker.h"
#include "SpellBackend.h"

SpellBackend* SpellBackend::m_self = 0;

SpellBackend* SpellBackend::instance()
{
  if (!m_self)
    m_self = new HunspellChecker(QCoreApplication::instance());

  return m_self;
}

SpellBackend::SpellBackend(QObject *parent) :
  QObject(parent)
{
}


bool SpellBackend::add(const QString &word)
{
  Q_UNUSED(word);
  return false;
}


bool SpellBackend::available() const
{
  return false;
}


bool SpellBackend::isCorrect(const QString &word) const
{
  Q_UNUSED(word);
  return true;
}


bool SpellBackend::writable() const
{
  return false;
}


QStringList SpellBackend::dictionaries() const
{
  return QStringList();
}


QStringList SpellBackend::suggestions(const QString &word) const
{
  Q_UNUSED(word);
  return QStringList();
}


void SpellBackend::queuedSuggestions(const QString &word) const
{
  Q_UNUSED(word);
}


QString SpellBackend::actuallLang() const
{
  return QString();
}


void SpellBackend::setLangs(const QStringList &dicts)
{
  Q_UNUSED(dicts)
}
