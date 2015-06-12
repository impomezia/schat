/* $Id: History.js 3841 2013-10-27 00:22:25Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright (c) 2008-2013 IMPOMEZIA <schat@impomezia.com>
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


/*
 * Ожидание загрузки всех сообщений.
 */
function HistoryScroll(type, data) {
  this.id       = '';
  this.messages = {};
  this.count    = 0;

  if (type == 'since' && data.hasOwnProperty('day'))
    this.id = 'day-' + data.day;
  else if (type == 'last' && data.user === true)
    this.id = 'Chat';

  for (var i = 0; i < data.messages.length; i++) {
    var id = data.messages[i];
    if (document.getElementById(id) === null && History.unhandled.indexOf(id) == -1 && this.messages[id] !== true) {
      this.messages[id] = true;
      this.count++;
    }
  }

  if (this.count == 0) {
    History.done();
    History.unhandled = [];

    if (this.id != '') {
      Settings.scrollTo = this.id;
    }
  }
  else
    History.scroll = this;

  this.removeAll = function(ids) {
    for (var i = 0; i < ids.length; i++) {
      if (this.messages.hasOwnProperty(ids[i])) {
        delete this.messages[ids[i]];
        this.count--;
      }
    }

    if (this.count == 0) {
      History.scroll = null;
      History.unhandled = [];
      History.done();

      if (this.id != '')
        Settings.scrollTo = this.id;
    }
  };
}


var History = {
  date: 0,       /// Дата самого старого полученного сообщения.
  message: null, /// Идентификатор самого старого полученного сообщения.
  scroll: null,  /// Отложенный скролл ожидающий загрузки сообщений.
  top: false,    /// true если полученны все сообщения.
  unhandled: [], /// Необработанные сообщения.

  /*
   * Показ виджета истории.
   */
  show: function() {
    if (document.getElementById('history') === null) {
      var chat  = document.getElementById('Chat');
      var block = document.createElement('div');
      block.id = 'history-bar';
      block.innerHTML = '<div id="history"></div>';
      chat.insertBefore(block, chat.firstChild);
    }

    document.getElementById('history-bar').style.display = 'block';
  },


  /*
   * Сокрытие виджета истории.
   */
  hide: function() {
    $('#history-bar').hide();

    alignChat();
    Loader.spinner.remove('history_loading');
  },


  /*
   * Обработка клика для загрузки сообщений.
   */
  click: function(event) {
    event.preventDefault();
    if (!SimpleChat.isOnline())
      return;

    History.loading(Settings.id);
    SimpleChat.get(Settings.id, 'messages/last', { before: History.date, user: true });
  },


  /*
   * Уведомление пользователя о загрузке сообщений.
   */
  loading: function(id) {
    if (Settings.id != id)
      return;

    Loader.spinner.add('history_loading');
  },


  /*
   * Обработка получения данных фидов.
   */
  feed: function(json) {
    if (json.id != Settings.getId())
      return;

    if (json.type == 'reply' && json.cmd == 'get') {
      if (json.name == 'messages/last')
        History.last(json);
      else if (json.name == 'messages/since')
        History.since(json);
    }
  },


  /*
   * Обработка ответа на "get" запрос к "messages/last".
   */
  last: function(json) {
    History.date    = 0;
    History.message = null;

    if (json.status == 200 && json.data.messages.length) {
      var id = json.data.messages[0];
      if ($('#' + id).length)
        History.date = $('#' + id).attr('data-time');
      else
        History.message = id;

      if (json.data.count < 20)
        History.top = true;

      alignChat();
      new HistoryScroll('last', json.data);
    }
    else if (json.status != 303)
      History.hide();
  },


  /*
   * Обработка ответа на "get" запрос к "messages/since".
   */
  since: function(json) {
    if (json.status == 200 && json.data.messages.length)
      new HistoryScroll('since', json.data);
    else
      History.done();
  },


  /*
   * Показ ссылки для загрузки сообщений.
   */
  done: function() {
    var bar = document.getElementById('history-bar');
    if (!History.top) {
      History.show();
      document.getElementById('history').innerHTML = '<a class="history-more btn btn-small" href="#"><i class="icon-history-more"></i> <span data-tr="history_more">' + Utils.tr('history_more') + '</span></a>';
    }
    else if (bar !== null)
      bar.style.display = 'none';

    alignChat();
    Loader.spinner.remove('history_loading');
  },


  /*
   * Обработка добавления сообщения.
   */
  onAdd: function(id) {
//    History.count(id);
  },

  onBulkAdd: function(added) {
    for (var i = 0; i < added.length; i++) {
      if (History.message !== null && History.message == added[i]) {
        History.date    = document.getElementById(added[i]).getAttribute('data-time');
        History.message = null;
      }
    }

    if (History.scroll instanceof HistoryScroll)
      History.scroll.removeAll(added);
  },


  count: function(id) {
    if (document.getElementById(id) === null)
      return;

    var time = $('#' + id).attr('data-time');
    if (time === undefined)
      return;

    var day = DateTime.dayId(new Date(parseInt(time)));
    if (day.length != 10)
      return;

    if (document.getElementById('day-' + day) === null)
      return;

    if ($('#day-' + day + ' .day-header').hasClass('expanded'))
      return;

    var badge = $('#day-' + day + ' .day-badge');
    var count = parseInt(badge.text());
    count++;
    badge.text(count);

    if (count == 1)
      badge.fadeIn('fast');
  },


  /*
   * Обработка не поддерживаемых сообщений.
   */
  unhandledMessage: function(json) {
    var id = json.OID || json.Id;
    if (History.scroll instanceof HistoryScroll) {
      var ids = [id];
      History.scroll.removeAll(ids);
    }
    else if (History.unhandled.indexOf(id) == -1)
      History.unhandled.push(id);
  },


  /*
   * Очистка страницы.
   */
  reload: function() {
    History.message = null;
    History.date    = 0;
    History.top     = true;

    History.done();
    History.top = false;
  }
};


if (typeof HistoryView === 'undefined') {
  HistoryView = {
    isAutoLoad: function(id)       { return true; },
    filter: function(id, text)     { return text; },
    genTag: function(messages)     { return '' },
    getTag: function(channel)      { return '' },
    setTag: function(channel, tag) {}
  }
}
else {
  HistoryView.loading.connect(History.loading);
  ChatView.feed.connect(History.feed);
  ChatView.reload.connect(History.reload);
}

Messages.onAdd.push(History.onAdd);
Messages.onBulkAdd.push(History.onBulkAdd);
Messages.unhandled.push(History.unhandledMessage);

$(document).ready(function() {
  $('body').on('click.history', '#history a', History.click);

  if (SimpleChat.isOnline()) {
    if (HistoryView.isAutoLoad(Settings.getId()))
      History.loading(Settings.id);
  }
  else
    History.done();
});
