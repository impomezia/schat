/* $Id: SpellBackend.h 2969 2012-08-04 01:54:38Z IMPOMEZIA $
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

#ifndef SPELLBACKEND_H
#define SPELLBACKEND_H

#include <QObject>
#include <QStringList>

class SpellBackend : public QObject
{
  Q_OBJECT

public:
  static SpellBackend* instance();
  virtual bool add(const QString &word);
  virtual bool available() const;
  virtual bool isCorrect(const QString &word) const;
  virtual bool writable() const;
  virtual QString actuallLang() const;
  virtual QStringList dictionaries() const;
  virtual QStringList suggestions(const QString &word) const;
  virtual void queuedSuggestions(const QString &word) const;
  virtual void setLangs(const QStringList &dicts);

signals:
  void suggestionsReady(const QString &word, const QStringList &words);

protected:
  SpellBackend(QObject *parent = 0);

private:
  static SpellBackend *m_self;
};

#endif // SPELLBACKEND_H
