/* $Id: SpellCheckerWidget.cpp 2977 2012-08-06 03:43:06Z IMPOMEZIA $
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

#include <QLabel>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QListWidget>
#include <QEvent>

#include "ChatCore.h"
#include "ChatSettings.h"
#include "sglobal.h"
#include "SpellBackend.h"
#include "SpellChecker.h"
#include "SpellCheckerWidget.h"

SpellCheckerWidget::SpellCheckerWidget(QWidget *parent)
  : QWidget(parent)
{
  m_label = new QLabel(this);
  m_active = new QCheckBox(this);
  m_active->setChecked(ChatCore::settings()->value(LS("SpellChecker/Active")).toBool());

  m_advanced = new QCheckBox(this);
  m_advanced->setChecked(ChatCore::settings()->value(LS("SpellChecker/Advanced")).toBool());

  m_list = new QListWidget(this);
  m_list->setFrameShape(QFrame::NoFrame);
  m_list->setAlternatingRowColors(true);
  m_list->setSpacing(1);
  m_list->setDragDropMode(QAbstractItemView::InternalMove);
  m_list->installEventFilter(this);

  QGridLayout *listLay = new QGridLayout();
  listLay->addWidget(m_list);
  listLay->setContentsMargins(16, 0, 0, 0);

  QVBoxLayout *layout = new QVBoxLayout();
  layout->addWidget(m_active);
  layout->addWidget(m_advanced);
  layout->addLayout(listLay);
  layout->setContentsMargins(10, 0, 3, 0);

  QVBoxLayout *mainLay = new QVBoxLayout(this);
  mainLay->addWidget(m_label);
  mainLay->addLayout(layout);
  mainLay->setContentsMargins(0, 12, 0, 0);

  build();
  updateVisibility();
  retranslateUi();

  connect(m_active, SIGNAL(clicked(bool)),SLOT(activeClicked(bool)));
  connect(m_advanced, SIGNAL(clicked(bool)),SLOT(advancedClicked(bool)));
  connect(m_list, SIGNAL(itemChanged(QListWidgetItem*)),SLOT(reloadDictionaries()));
}


bool SpellCheckerWidget::eventFilter(QObject *watched, QEvent *event)
{
  if (watched == m_list && event->type() == QEvent::ChildRemoved)
    reloadDictionaries();

  return QWidget::eventFilter(watched, event);
}


void SpellCheckerWidget::changeEvent(QEvent *event)
{
  if (event->type() == QEvent::LanguageChange)
    retranslateUi();

  QWidget::changeEvent(event);
}


void SpellCheckerWidget::activeClicked(bool checked)
{
  ChatCore::settings()->setValue(LS("SpellChecker/Active"), checked);
  updateVisibility();

  SpellChecker::reload();
}


void SpellCheckerWidget::advancedClicked(bool checked)
{
  ChatCore::settings()->setValue(LS("SpellChecker/Advanced"), checked);
  updateVisibility();

  reloadDictionaries();
}


void SpellCheckerWidget::reloadDictionaries()
{
  QStringList dictionaries = checked();
  if (dictionaries.isEmpty())
    dictionaries.append(LS("null"));

  ChatCore::settings()->setValue(LS("SpellChecker/Dictionaries"), dictionaries);
  SpellChecker::reload();
}


QStringList SpellCheckerWidget::checked() const
{
  QStringList out;
  for (int row = 0; row < m_list->count(); ++row) {
    QListWidgetItem *item = m_list->item(row);
    if (item && item->checkState() == Qt::Checked)
      out.append(item->data(Qt::UserRole).toString());
  }

  return out;
}


/*!
 * Создание списка словарей.
 */
void SpellCheckerWidget::build()
{
  QStringList result;
  QStringList available = SpellBackend::instance()->dictionaries();
  QStringList checked   = ChatCore::settings()->value(LS("SpellChecker/Dictionaries")).toStringList();
  if (checked.isEmpty())
    checked = SpellChecker::detect();

  foreach (const QString &name, checked) {
    if (available.contains(name))
      result.append(name);
  }

  foreach (const QString &name, available) {
    if (!result.contains(name))
      result.append(name);
  }

  fill(result, checked);
}


/*!
 * Заполнение списка доступных языков.
 *
 * \param dictionaries Отсортированный список словарей.
 * \param checked      Список выбранных словарей.
 */
void SpellCheckerWidget::fill(const QStringList &dictionaries, const QStringList &checked)
{
  foreach (const QString &name, dictionaries) {
    QString text = name;
    QLocale locale(name);
    if (locale.language() != QLocale::C) {
#     if QT_VERSION >= 0x040800
      text = QString(LS("%1 (%2)")).arg(locale.nativeLanguageName(), locale.nativeCountryName());
#     else
      text = QString(LS("%1 (%2)")).arg(QLocale::languageToString(locale.language()), QLocale::countryToString(locale.country()));
#     endif
    }

    QListWidgetItem *item = new QListWidgetItem(text, m_list);
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
    item->setCheckState(checked.contains(name) ? Qt::Checked : Qt::Unchecked);
    item->setData(Qt::UserRole, name);
  }
}


void SpellCheckerWidget::retranslateUi()
{
  m_label->setText(LS("<b>") + tr("Spell checking") + LS("</b>"));
  m_active->setText(tr("Enable spell checking"));
  m_advanced->setText(tr("Advanced setup"));
}


void SpellCheckerWidget::updateVisibility()
{
  m_list->setVisible(m_active->isChecked() && m_advanced->isChecked());
  m_advanced->setVisible(m_active->isChecked());
}
