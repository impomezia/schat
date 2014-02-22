/* $Id: LanguageBox.cpp 2216 2012-01-26 22:13:04Z IMPOMEZIA $
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

#include <QDir>
#include <QEvent>
#include <QTranslator>

#include "sglobal.h"
#include "Translation.h"
#include "ui/LanguageBox.h"

/*!
 * Конструктор класса LanguageBox.
 */
LanguageBox::LanguageBox(Translation *translation, QWidget *parent)
  : QComboBox(parent)
  , m_translation(translation)
{
  QString prefix = translation->prefix();
  addItem(QIcon(LS(":/translations/en.png")), LS("English"), LS(":/translations/") + prefix + LS("en.qm"));

  QStringList qmFiles = findQmFiles();
  bool canOverrideEnglish = true;

  for (int i = 0; i < qmFiles.size(); ++i) {
    QString file = qmFiles[i];
    QString langName = languageName(file);

    if (langName.isEmpty())
      continue;

    // Добавляем в список только уникальные языки.
    if (findText(langName) == -1) {
      addItem(QIcon(languageIcon(file)), langName, file);
    }
    else if (canOverrideEnglish && file.endsWith(prefix + LS("en.qm"))) {
      setItemIcon(0, QIcon(languageIcon(file)));
      setItemData(0, file);
      canOverrideEnglish = false;
    }
  }

  setCurrentIndex(findText(m_translation->language()));
}


bool LanguageBox::save()
{
  if (currentText() == m_translation->language())
    return false;

  m_translation->load(qmFile());
  return true;
}


QString LanguageBox::qmFile() const
{
  return itemData(currentIndex()).toString();
}


void LanguageBox::changeEvent(QEvent *event)
{
  if (event->type() == QEvent::LanguageChange)
    retranslateUi();

  QComboBox::changeEvent(event);
}


/*!
 * Возвращает путь к файлу изображения флага языка.
 */
QString LanguageBox::languageIcon(const QString &file) const
{
  QFileInfo fi(file);
  QString icon = fi.absolutePath() + LS("/") + fi.baseName().remove(0, m_translation->prefix().size()) + LS(".png");

  if (QFile::exists(icon))
    return icon;

  if (icon.endsWith(LS("en.png")))
    return LS(":/translations/en.png");

  return LS(":/translations/unknown.png");
}


QString LanguageBox::languageName(const QString &file) const
{
  QTranslator translator;
  translator.load(file);

  return translator.translate("Translation", "English");
}


/*!
 * Поиск qm файлов.
 */
QStringList LanguageBox::findQmFiles() const
{
  QStringList fileNames;

  foreach (QString d, m_translation->search()) {
    QDir dir(d);
    QStringList fn = dir.entryList(QStringList(m_translation->prefix() + LS("*.qm")), QDir::Files, QDir::Name);
    QMutableStringListIterator i(fn);
    while (i.hasNext()) {
        i.next();
        i.setValue(dir.filePath(i.value()));
    }
    fileNames += fn;
  }

  return fileNames;
}


void LanguageBox::retranslateUi()
{
  setCurrentIndex(findText(m_translation->language()));
}
