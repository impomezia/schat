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

#include <QAction>
#include <QContextMenuEvent>
#include <QEvent>
#include <QMenu>
#include <QTimer>
#include <QToolBar>
#include <QToolButton>

#if defined(Q_OS_WIN32)
# include <qt_windows.h>
#endif

#include "actions/MenuBuilder.h"
#include "ChatAlerts.h"
#include "ChatCore.h"
#include "ChatIcons.h"
#include "ChatNotify.h"
#include "ChatSettings.h"
#include "ChatUrls.h"
#include "client/ChatClient.h"
#include "client/ClientChannels.h"
#include "client/SimpleClient.h"
#include "debugstream.h"
#include "feeds/ServerFeed.h"
#include "messages/ChannelMessage.h"
#include "net/SimpleID.h"
#include "NetworkManager.h"
#include "sglobal.h"
#include "ui/AuthIcon.h"
#include "ui/MainToolBar.h"
#include "ui/SoundButton.h"
#include "ui/TabBar.h"
#include "ui/TabCreator.h"
#include "ui/tabs/AboutTab.h"
#include "ui/tabs/ChannelTab.h"
#include "ui/tabs/ChatView.h"
#include "ui/tabs/PrivateTab.h"
#include "ui/tabs/ProgressTab.h"
#include "ui/tabs/ServerTab.h"
#include "ui/tabs/SettingsTab.h"
#include "ui/tabs/UserView.h"
#include "ui/tabs/WelcomeTab.h"
#include "ui/TabsToolBar.h"
#include "ui/TabWidget.h"
#include "ui/TrayIcon.h"

TabWidget *TabWidget::m_self = 0;

TabWidget::TabWidget(QWidget *parent)
  : QTabWidget(parent)
  , m_serverTab(0)
  , m_tray(0)
{
  m_self = this;

  m_tabBar = new TabBar(this);
  setTabBar(m_tabBar);
  setDocumentMode(true);

# if !defined(Q_OS_MAC)
  setStyleSheet(LS("QToolBar { margin:0px; border:0px; }"));
# endif

  int fontSize = ChatCore::settings()->value(ChatSettings::kChatViewFontSize).toInt();
  if (fontSize == -1)
    fontSize = fontInfo().pixelSize();

  QString fontFamily = ChatCore::settings()->value(ChatSettings::kChatViewFontFamily).toString();
  if (fontFamily.isEmpty() || fontFamily == LS("\"\""))
    fontFamily = fontInfo().family();

  QWebSettings::globalSettings()->setFontSize(QWebSettings::DefaultFontSize,         fontSize);
  QWebSettings::globalSettings()->setFontSize(QWebSettings::DefaultFixedFontSize,    fontSize);
  QWebSettings::globalSettings()->setFontFamily(QWebSettings::StandardFont,          fontFamily);
  QWebSettings::globalSettings()->setFontFamily(QWebSettings::SerifFont,             fontFamily);
  QWebSettings::globalSettings()->setFontFamily(QWebSettings::SansSerifFont,         fontFamily);
  QWebSettings::globalSettings()->setAttribute(QWebSettings::LocalStorageEnabled,    true);
  QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, ChatCore::settings()->value(SETTINGS_LABS_DEVELOPER_EXTRAS).toBool());

  m_authIcon = new AuthIcon();

  add(new AboutTabCreator());
  add(new SettingsTabCreator());
  add(new WelcomeTabCreator());
  add(new ProgressTabCreator());

  tab(PROGRESS_TAB);

  createToolBars();

  if (!ChatCore::isReady())
    connect(ChatCore::i(), SIGNAL(ready()), SLOT(onReady()));
  else
    onReady();

  connect(this, SIGNAL(tabCloseRequested(int)), SLOT(closeTab(int)));
  connect(this, SIGNAL(currentChanged(int)), SLOT(currentChanged(int)));
  connect(ChatNotify::i(), SIGNAL(notify(Notify)), SLOT(notify(Notify)));
}


TabWidget::~TabWidget()
{
  m_self = 0;

  ChatAlerts::reset();
  delete m_authIcon;
  qDeleteAll(m_creators);
}


AbstractTab *TabWidget::widget(int index) const
{
  return static_cast<AbstractTab*>(QTabWidget::widget(index));
}


ClientChannel TabWidget::channel(const QByteArray &id) const
{
  if (m_channels.contains(id) && m_channels.value(id)->channel()->id() == id)
    return m_channels.value(id)->channel();

  return ClientChannel();
}


/*!
 * Создание или открытие новой вкладки.
 *
 * \param name    Внутреннее имя вкладки или идентификатор канала.
 * \param options Опции. \sa TabWidget::TabOptions.
 * \param data    Дополнительные данные для вкладки.
 */
AbstractTab *TabWidget::tab(const QByteArray &name, int options, const QVariant &data)
{
  if (Channel::isCompatibleId(name))
    return channelTab(name, options & CreateTab, options & ShowTab);

  if (!m_creators.contains(name))
    return 0;

  AbstractTab *tab    = m_pages.value(name);
  bool created        = false;
  TabCreator *creator = m_creators.value(name);

  if (!tab && (options & CreateTab)) {
    created = true;
    tab     = m_creators.value(name)->create(data, this);

    m_pages[name] = tab;
    tab->setOnline();
    addTab(tab, tab->icon(), tab->text());

    ChatNotify::start(Notify::PageOpen, name);
  }

  if (!tab)
    return 0;

  if (!created)
    creator->reload(tab, data);

  if (options & ShowTab)
    setCurrentIndex(indexOf(tab));

  return tab;
}


/*!
 * Добавление нового класса помощника для создания вкладок.
 */
void TabWidget::add(TabCreator *creator)
{
  const QByteArray name = creator->name();

  if (name.isEmpty()) {
    delete creator;
    return;
  }

  TabCreator *exists = m_creators.value(name);
  if (exists)
    delete exists;

  m_creators[name] = creator;
}


void TabWidget::closePage(const QByteArray &name)
{
  AbstractTab *tab = m_pages.value(name);
  if (tab)
    closeTab(indexOf(tab));;
}


/*!
 * Создание или повторная инициализация вкладки канала.
 *
 * \param id     Идентификатор канала.
 * \param create \b true если необходимо создать канал.
 * \param show   \b true если необходимо выбрать эту вкладку.
 *
 * \return Возвращает указатель на вкладку или 0 в случае ошибки.
 */
ChannelBaseTab *TabWidget::channelTab(const QByteArray &id, bool create, bool show)
{
  SLOG_DEBUG("id =" << SimpleID::encode(id) << "create =" << create << "show =" << show);

  if (!Channel::isCompatibleId(id))
    return 0;

  ChannelBaseTab *tab = 0;

  if (m_channels.contains(id)) {
    tab = m_channels.value(id);
    create = false;
  }

  ClientChannel channel = ChatClient::channels()->get(id);
  if (!channel) {
    if (!m_prefetch.contains(id))
      m_prefetch.append(id);

    return 0;
  }

  if (create) {
    if (channel->type() == SimpleID::UserId)
      tab = new PrivateTab(channel, this);
    else if (channel->type() == SimpleID::ChannelId)
      tab = new ChannelTab(channel, this);

    if (tab) {
      m_channels[id] = tab;
      tab->setOnline();
      addTab(tab, tab->icon(), channel->name());
      connect(tab, SIGNAL(actionTriggered(bool)), SLOT(openTab()));

      if (channel->type() == SimpleID::ChannelId && isAutoPin(channel->id()))
        tab->pin();

      if (m_autoPin.contains(id)) {
        m_autoPin.removeAll(id);
        tab->pin();
        emit pinned(tab);
      }
    }

    closePage(PROGRESS_TAB);
    closePage(WELCOME_TAB);
  }

  if (show && tab)
    setCurrentIndex(indexOf(tab));

  return tab;
}


int TabWidget::channelsCount(int type) const
{
  if (!type)
    return m_channels.size();

  int count = 0;
  QMapIterator<QByteArray, ChannelBaseTab*> i(m_channels);
  while (i.hasNext()) {
    i.next();
    if (SimpleID::typeOf(i.key()) == type)
      count++;
  }

  return count;
}


/*!
 * Добавление нового сообщения.
 *
 * \param message Сообщение.
 * \param create  \b true если при необходимости будет создана новая вкладка.
 */
void TabWidget::add(const Message &message, bool create)
{
  if (!m_self)
    return;

  m_self->addImpl(message, create);
}


void TabWidget::pin(const QByteArray &id)
{
  if (!m_self)
    return;

  AbstractTab *tab = m_self->tab(id, NoOptions);
  if (tab && tab->options() & AbstractTab::CanBePinned) {
    tab->pin();
    emit m_self->pinned(tab);
  }
  else if (!m_self->m_autoPin.contains(id))
    m_self->m_autoPin.append(id);
}


void TabWidget::showDialog(QWidget *widget)
{
  if (!m_self)
    return;

  ChannelBaseTab *tab = qobject_cast<ChannelBaseTab *>(m_self->currentWidget());
  if (tab)
    tab->chatView()->showDialog(widget);
}


void TabWidget::stopAlert()
{
  ChannelBaseTab *tab = qobject_cast<ChannelBaseTab *>(currentWidget());
  if (!tab)
    return;

  if (tab->alerts())
    tab->stopAlert();
}


/*!
 * Проверка состояния вкладки, для принятия решения отображать оповещения или нет.
 *
 * \param id Идентификатор вкладки.
 *
 * \return \b true если окно чата активно и выбрана вкладка с идентификатором равным \p id.
 */
bool TabWidget::isActive(const QByteArray &id)
{
  if (!m_self)
    return false;

  AbstractTab *tab = m_self->widget(m_self->currentIndex());
  if (tab && tab->id() != id)
    return false;

  return isActiveChatWindow();
}


bool TabWidget::isActiveChatWindow()
{
  QWidget *widget = m_self->parentWidget();
  if (widget->isMinimized() || widget->isHidden())
    return false;

# if defined(Q_OS_WIN32)
  if (widget->isActiveWindow()) {
    HWND active = GetForegroundWindow();
    if (active == (HWND) widget->window()->internalWinId() || ::IsChild(active, (HWND) widget->window()->internalWinId()))
      return true;
    else
      return false;
  }
  else
    return false;
# else
  return widget->isActiveWindow();
# endif
}


bool TabWidget::isCurrent(AbstractTab *tab)
{
  if (tab && m_self)
    return m_self->indexOf(tab) == m_self->currentIndex();

  return false;
}


/*!
 * Обработка закрытия вкладки.
 */
void TabWidget::closeTab(int index)
{
  SCHAT_DEBUG_STREAM(this << "closeTab()" << index)

  if (index == -1)
    return;

  AbstractTab *tab = widget(index);
  if (!tab)
    return;

  const QByteArray &id = tab->id();

  // Закрытие канала.
  if (m_channels.contains(id)) {
    ChatNotify::start(Notify::ChannelTabClosed, id);
    m_channels.remove(id);
  }

  removeTab(index);

  if (!(tab->options() & AbstractTab::NoDelete)) {
    if (m_pages.contains(id)) {
      ChatNotify::start(Notify::PageClosed, id);
      m_pages.remove(id);
    }

    QTimer::singleShot(0, tab, SLOT(deleteLater()));
  }
  else
    tab->setOnline(false);

  lastTab();
}



bool TabWidget::event(QEvent *event)
{
  if (event->type() == QEvent::WindowActivate)
    stopAlert();

  return QTabWidget::event(event);
}


void TabWidget::changeEvent(QEvent *event)
{
  if (event->type() == QEvent::LanguageChange)
    retranslateUi();

  QTabWidget::changeEvent(event);
}


/*!
 * Контекстное меню для вкладки.
 */
void TabWidget::contextMenuEvent(QContextMenuEvent *event)
{
  const int index = tabAt(event->globalPos());
  if (index == -1) {
    QTabWidget::contextMenuEvent(event);
    return;
  }

  QMenu menu(this);
  QAction *pin     = 0;
  QAction *close   = 0;
  AbstractTab *tab = widget(index);

  if (tab->bindMenu(&menu))
    menu.addSeparator();

  if (isCloseable(tab->id())) {
    if (tab->options() & AbstractTab::CanBePinned) {
      pin = menu.addAction(SCHAT_ICON(Pin), tr("Pin tab"));
      pin->setCheckable(true);
      pin->setChecked(tab->options() & AbstractTab::Pinned);
    }

    if (count() > 1)
      close = menu.addAction(SCHAT_ICON(Remove), tr("Close tab"));
  }

  if (!menu.actions().isEmpty()) {
    QAction *action = menu.exec(event->globalPos());
    if (action == 0)
      return;

    if (action == pin) {
      if (tab->options() & AbstractTab::Pinned) {
        unpin(tab);
      }
      else {
        tab->pin();
        emit pinned(tab);
      }
    }
    if (action == close) {
      unpin(tab);
      closeTab(index);
    }
  }
}


void TabWidget::keyPressEvent(QKeyEvent *event)
{
  if (event->modifiers() == Qt::ControlModifier) {
    if (event->key() == Qt::Key_W) {
      int index = currentIndex();
      if (index != -1 && count() > 1) {
        AbstractTab *tab = widget(index);
        if (isCloseable(tab->id())) {
          unpin(tab);
          closeTab(index);
        }
      }
    }
  }

  QTabWidget::keyPressEvent(event);
}


/*!
 * Закрытие вкладок средней кнопкой мыши.
 */
void TabWidget::mouseReleaseEvent(QMouseEvent *event)
{
  const int index = tabAt(event->globalPos());
  if (index != -1 && count() > 1 && event->button() == Qt::MidButton) {
    AbstractTab *tab = widget(index);
    if (isCloseable(tab->id())) {
      unpin(tab);
      closeTab(index);
    }
  }
  else
    QTabWidget::mouseReleaseEvent(event);
}


/*!
 * Обработка вставки вкладки.
 */
void TabWidget::tabInserted(int index)
{
  widget(index)->setCloseButton(m_tabBar->tabButton(index, m_tabBar->closeButtonPosition()));
  QTabWidget::tabInserted(index);

  if (count() == 2) {
    AbstractTab *tab = widget(index == 0 ? 1 : 0);
    if (!(tab->options() & AbstractTab::Pinned))
      widget(index == 0 ? 1 : 0)->unpin();
  }
  else if (count() == 1)
    m_tabBar->setTabButton(index, m_tabBar->closeButtonPosition(), 0);
}


void TabWidget::tabRemoved(int index)
{
  QTabWidget::tabRemoved(index);

  if (count() == 1)
    m_tabBar->setTabButton(0, m_tabBar->closeButtonPosition(), 0);
}


/*!
 * Установка индекса на вкладку.
 */
void TabWidget::openTab()
{
  addChatTab(qobject_cast<AbstractTab *>(sender()));
}


void TabWidget::addChannel(const QByteArray &id, const QString &xName)
{
  const int type = SimpleID::typeOf(id);

  if (type == ChatId::ChannelId || m_prefetch.contains(id)) {
    m_prefetch.removeAll(id);
    channelTab(id, true, type == ChatId::ChannelId ? !m_channels.contains(id) : false);
  }
  else if (type == ChatId::UserId && !xName.isEmpty())
    channelTab(id, true, true);
}


/*!
 * Обработка изменения состояния клиента.
 */
void TabWidget::clientStateChanged(int state, int previousState)
{
  if (state == ChatClient::Error) {
    const QVariantMap error = ChatClient::io()->json().value(CLIENT_PROP_ERROR).toMap();
    const int status = error.value(CLIENT_PROP_ERROR_STATUS).toInt();

    if (status == Notice::Unauthorized && !m_pages.contains(WELCOME_TAB))
      ChatUrls::open(QUrl(LS("chat://settings/network")));
  }

  if ((state == ChatClient::Error || state == ChatClient::Offline) && previousState == ChatClient::Connecting)
    closePage(PROGRESS_TAB);
}


void TabWidget::currentChanged(int index)
{
  AbstractTab *tab = widget(index);
  if (!tab)
    return;

  ChatCore::setCurrentId(tab->id());
  stopAlert();

  emit pageChanged(tab);
}


void TabWidget::notify(const Notify &notify)
{
  const int type = notify.type();

  if (type == Notify::OpenChannel || type == Notify::OpenInfo) {
    const QByteArray id = notify.data().toByteArray();
    if (SimpleID::typeOf(id) == SimpleID::ChannelId) {
      ChannelBaseTab *tab = channelTab(id, false, false);
      if (!tab || !tab->isActive())
        ChatClient::channels()->join(id);
    }

    ChannelBaseTab *tab = channelTab(id);
    if (!tab)
      return;

    if (type == Notify::OpenInfo)
      tab->chatView()->evaluateJavaScript(LS("Pages.setPage(1);"));
    else
      tab->chatView()->evaluateJavaScript(LS("Pages.setPage(0);"));
  }
  else if (type == Notify::OpenAbout) {
    tab(ABOUT_TAB);
  }
  else if (type == Notify::OpenSettings) {
    tab(SETTINGS_TAB, CreateTab | ShowTab, notify.data());
  }
  else if (type == Notify::CopyRequest && currentIndex() != -1) {
    widget(currentIndex())->copy();
  }
}


void TabWidget::onReady()
{
  SLOG_DEBUG("ready");

  m_serverTab = new ServerTab(this);
  m_serverTab->setVisible(false);

  m_tray = new TrayIcon(this);
  m_tabsToolBar->setEnabled(true);
  m_mainToolBar->setReady(true);

  connect(ChatClient::channels(), SIGNAL(channel(QByteArray,QString)), SLOT(addChannel(QByteArray,QString)));
  connect(ChatClient::io(), SIGNAL(clientStateChanged(int,int)), SLOT(clientStateChanged(int,int)));
  connect(m_serverTab, SIGNAL(actionTriggered(bool)), SLOT(openTab()));

  showWelcome();
}


/*!
 * \param id Идентификатор канала.
 *
 * \return \b true если эта вкладка должна быть автоматически закреплена.
 */
bool TabWidget::isAutoPin(const QByteArray &id) const
{
  if (ChatClient::channels()->mainId() != id)
    return false;

  const int policy = ChatClient::channels()->policy();
  if (!(policy & ServerFeed::MainChannelPolicy))
    return false;

  return (policy & ServerFeed::ForcedJoinPolicy || ChatCore::settings()->value(SETTINGS_AUTO_JOIN).toBool());
}


bool TabWidget::isCloseable(const QByteArray &id) const
{
  if (ChatClient::channels()->mainId() != id)
    return true;

  int policy = ChatClient::channels()->policy();
  return (!(policy & ServerFeed::ForcedJoinPolicy && policy & ServerFeed::DisabledPartPolicy));
}


/*!
 * Добавление вкладки и установка её текущей вкладкой.
 * Если вкладка была добавлена ранее, то она просто становится текущей.
 *
 * \param tab Указатель на вкладку.
 */
int TabWidget::addChatTab(AbstractTab *tab)
{
  if (tab == 0)
    return -1;

  int index = indexOf(tab);
  if (index == -1) {
    tab->setOnline();
    index = addTab(tab, tab->icon(), tab->text());
  }

  setCurrentIndex(index);
  return index;
}


int TabWidget::tabAt(const QPoint &pos) const
{
  return m_tabBar->tabAt(m_tabBar->mapFromGlobal(pos));
}


/*!
 * Добавление нового сообщения.
 */
void TabWidget::addImpl(const Message &message, bool create)
{
  const QByteArray& id = message.tab();
  if (!Channel::isCompatibleId(id))
    return;

  if (m_channels.contains(id)) {
    m_channels.value(id)->add(message);
  }
  else if (SimpleID::typeOf(id) == SimpleID::ServerId) {
    m_serverTab->chatView()->add(message);
  }
  else {
    ChannelBaseTab *tab = channelTab(id, create, false);
    if (tab)
      tab->add(message);
  }
}


/*!
 * Создание панелей инструментов.
 */
void TabWidget::createToolBars()
{
  m_tabsToolBar = new TabsToolBar(this);
  m_tabsToolBar->setEnabled(false);

  m_mainToolBar = new MainToolBar(this);

  setCornerWidget(m_tabsToolBar, Qt::TopLeftCorner);
  setCornerWidget(m_mainToolBar, Qt::TopRightCorner);
}


void TabWidget::lastTab()
{
  if (count() == 0)
    addChatTab(m_serverTab);
}


void TabWidget::retranslateUi()
{
  if (m_tray)
    m_tray->retranslateUi();
}


/*!
 * Отображает вкладку приветствия или прогресса подключения в зависимости от состояния клиента.
 */
void TabWidget::showWelcome()
{
  if (!ChatCore::networks()->isAutoConnect()) {
    tab(WELCOME_TAB);
    closePage(PROGRESS_TAB);
  }
  else
    tab(PROGRESS_TAB)->setText(tr("Connecting"));
}


/*!
 * Открепление вкладки, вызванное пользователем явно или при закрытии закреплённой вкладки.
 */
void TabWidget::unpin(AbstractTab *tab)
{
  tab->unpin();
  emit unpinned(tab);
}
