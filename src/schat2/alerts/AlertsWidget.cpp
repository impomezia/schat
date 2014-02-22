/* $Id: AlertsWidget.cpp 3843 2013-10-27 04:09:43Z IMPOMEZIA $
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

#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QDesktopServices>
#include <QEvent>
#include <QFileDialog>
#include <QGridLayout>
#include <QToolBar>

#include "alerts/AlertsWidget.h"
#include "alerts/AlertType.h"
#include "ChatAlerts.h"
#include "ChatCore.h"
#include "ChatSettings.h"
#include "Path.h"
#include "sglobal.h"
#include "ui/ChatIcons.h"

AlertsWidget::AlertsWidget(QWidget *parent)
  : QWidget(parent)
{
  m_combo = new QComboBox(this);

  m_sound = new QCheckBox(tr("Play sound"), this);
  m_toolBar = new QToolBar(this);
  m_toolBar->setIconSize(QSize(16, 16));
  m_file = new QComboBox(this);
  m_file->addItems(ChatAlerts::sounds());

  m_toolBar->addWidget(m_file);
  m_control = m_toolBar->addAction(QIcon(LS(":/images/play.png")), tr("Play"), this, SLOT(play()));
  m_add     = m_toolBar->addAction(QIcon(LS(":/images/add-gray.png")), tr("Add sounds"), this, SLOT(add()));

  m_popup = new QCheckBox(tr("Show popup window"), this);
  m_tray  = new QCheckBox(tr("Show notification in tray"), this);

  QGridLayout *options = new QGridLayout();
  options->addWidget(m_sound, 0, 0);
  options->addWidget(m_toolBar, 0, 1, 2, 1, Qt::AlignTop | Qt::AlignRight);
  options->addWidget(m_popup, 1, 0);
  options->addWidget(m_tray, 2, 0, 1, 2);
  options->setColumnStretch(0, 1);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(m_combo);
  layout->addLayout(options);
  layout->setMargin(0);

  QList<AlertType*> types = ChatAlerts::types();
  foreach (AlertType *type, types) {
    m_combo->addItem(type->icon(), type->name(), type->type());
  }

  indexChanged(0);

  connect(m_combo,   SIGNAL(currentIndexChanged(int)),     SLOT(indexChanged(int)));
  connect(m_file,    SIGNAL(currentIndexChanged(QString)), SLOT(soundChanged(QString)));
  connect(m_popup,   SIGNAL(clicked(bool)),                SLOT(popupClicked(bool)));
  connect(m_sound,   SIGNAL(clicked(bool)),                SLOT(soundClicked(bool)));
  connect(m_tray,    SIGNAL(clicked(bool)),                SLOT(trayClicked(bool)));
}


void AlertsWidget::changeEvent(QEvent *event)
{
  if (event->type() == QEvent::LanguageChange)
    retranslateUi();

  QWidget::changeEvent(event);
}


void AlertsWidget::add()
{
#if QT_VERSION >= 0x050000
  QStringList files = QFileDialog::getOpenFileNames(this, tr("Open"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), LS("*.wav"));
#else
  QStringList files = QFileDialog::getOpenFileNames(this, tr("Open"), QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation), LS("*.wav"));
#endif

  if (files.isEmpty())
    return;

  QString dest = Path::data() + LS("/sounds/");
  foreach (const QString &name, files) {
    const QString newName = dest + QFileInfo(name).fileName();
    if (QFile::exists(newName))
      QFile::remove(newName);

    QFile::copy(name, newName);
  }

  ChatAlerts::i()->loadSounds();

  QString text = m_file->currentText();
  m_file->clear();
  m_file->addItems(ChatAlerts::sounds());

  m_file->setCurrentIndex(m_file->findText(text));
}


void AlertsWidget::indexChanged(int index)
{
  AlertType *type = ChatAlerts::type(m_combo->itemData(index).toString());
  if (!type)
    return;

  m_tray->setChecked(type->tray());
  m_popup->setChecked(type->popup());
  m_sound->setChecked(type->sound());

  const QStringList supports = type->supports();
  m_tray->setVisible(supports.contains(LS("tray")));
  m_popup->setVisible(supports.contains(LS("popup")));
  setSoundVisible(supports.contains(LS("sound")));

  if (m_sound->isChecked())
    m_file->setCurrentIndex(m_file->findText(type->value(LS("file")).toString()));
}


void AlertsWidget::play()
{
  if (m_file->currentIndex() == -1)
    return;

  ChatAlerts::play(m_file->currentText());
}


void AlertsWidget::popupClicked(bool checked)
{
  setValue(LS("popup"), checked);
}


void AlertsWidget::soundChanged(const QString &file)
{
  if (file.isEmpty())
    return;

  setValue(LS("file"), file);
}


void AlertsWidget::soundClicked(bool checked)
{
  setValue(LS("sound"), checked);
  m_toolBar->setVisible(checked);

}


void AlertsWidget::trayClicked(bool checked)
{
  setValue(LS("tray"), checked);
}


void AlertsWidget::retranslateUi()
{
  m_tray->setText(tr("Show notification in tray"));
  m_popup->setText(tr("Show popup window"));
  m_sound->setText(tr("Play sound"));
  m_control->setText(tr("Play"));
  m_add->setText(tr("Add sounds"));
}


void AlertsWidget::setSoundVisible(bool visible)
{
  m_sound->setVisible(visible);
  m_toolBar->setVisible(visible && m_sound->isChecked());
}


void AlertsWidget::setValue(const QString &key, const QVariant &value)
{
  AlertType *type = ChatAlerts::type(m_combo->itemData(m_combo->currentIndex()).toString());
  if (!type)
    return;

  ChatCore::settings()->setValue(LS("Alerts/") + type->type() + LC('.') + key, value);

  if (key == LS("sound") && value == true)
    m_file->setCurrentIndex(m_file->findText(type->value(LS("file")).toString()));
}
