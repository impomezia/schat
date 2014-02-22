/* $Id: EmoticonSelector.cpp 2718 2012-05-29 13:52:14Z IMPOMEZIA $
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

#include <QGridLayout>
#include <qmath.h>
#include <QMovie>

#include "EmoticonSelector.h"
#include "Emoticons.h"
#include "sglobal.h"
#include "ChatNotify.h"

EmoticonLabel::EmoticonLabel(Emoticon emoticon, QWidget *parent)
  : QLabel(parent)
  , m_ready(false)
{
  QMovie *movie = new QMovie(emoticon->file(), QByteArray(), this);
  setMovie(movie);
  setAlignment(Qt::AlignCenter);

  m_text = emoticon->texts().first();
  setToolTip(LS("<span>") + m_text + LS("</span>"));

  movie->start();

  updateStyleSheet();
}


void EmoticonLabel::enterEvent(QEvent *event)
{
  QLabel::enterEvent(event);
  updateStyleSheet(true);
}


void EmoticonLabel::leaveEvent(QEvent *event)
{
  QLabel::leaveEvent(event);
  updateStyleSheet();
}


void EmoticonLabel::mousePressEvent(QMouseEvent *event)
{
  m_ready = true;
  QLabel::mousePressEvent(event);
}


void EmoticonLabel::mouseReleaseEvent(QMouseEvent *event)
{
  if (!m_ready)
    return;

  ChatNotify::start(Notify::InsertText, QChar(QChar::Nbsp) + m_text + QChar(QChar::Nbsp));
  QLabel::mouseReleaseEvent(event);
}


void EmoticonLabel::updateStyleSheet(bool hover)
{
  setStyleSheet(LS("EmoticonLabel {background: ") + (hover ? LS("#eee") : LS("#fff")) + LS("; border-right: 1px solid #eee; border-bottom: 1px solid #eee;}"));
}


EmoticonSelector::EmoticonSelector(const QString &theme, Emoticons *emoticons, QWidget *parent)
  : QFrame(parent)
  , m_emoticons(emoticons)
  , m_theme(theme)
{
  m_layout = new QGridLayout(this);
  m_layout->setMargin(0);
  m_layout->setSpacing(0);

  setStyleSheet(LS("EmoticonSelector {border-top: 1px solid #eee; border-left: 1px solid #eee;}"));

  fill();
}


void EmoticonSelector::fill()
{
  QList<Emoticon> src = m_emoticons->theme(m_theme);
  if (src.isEmpty())
    return;

  int row = 0;
  int col = 0;
  int perRow = (int) qSqrt(src.size());
  if (perRow * perRow == src.size())
    perRow--;

  foreach (Emoticon emoticon, src) {
    EmoticonLabel *label = new EmoticonLabel(emoticon, this);
    m_layout->addWidget(label, row, col);
    if (col == perRow) {
      col = 0;
      row++;
    }
    else
      col++;
  }
}
