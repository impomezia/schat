/* Simple Chat
 * Copyright (c) 2008-2015 Alexander Sedov <imp@schat.me>
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

// Объект связанный с обработкой таблицы компьютеров.
var Hosts = {
  // Обработка получения тела фида hosts.
  body: function(json)
  {
    Hosts.read(json);
  },


  // Базовый слот обрабатывающий новые данные фидов.
  feed: function(json)
  {
    if (Pages.current != 1 || !json.hasOwnProperty('type') || json.own !== true || json.feed !== FEED_NAME_HOSTS)
      return;

    try {
      Hosts[json.type](json.data);
    }
    catch (e) {}
  },


  // Обработка одиночного хоста.
  processSingleHost: function(key, json)
  {
    var id = "#" + key;
    if (!$(id).length) {
      var out = '<tr class="host-row" id="' + key + '">' +
                  '<td class="os-cell">' +
                    '<i class="' + ((json.online === true ? "icon-status" : "icon-status-offline")) + ' host-status"></i>' +
                    '<a class="host-os" title="' +  htmlspecialchars(json.osName) + '"><i class="icon-os os-' + Pages.os(json.os) + '"></i></a>' +
                  '</td>' +
                  '<td class="host-name">' +
                    htmlspecialchars(json.name) +
                  '</td>' +
                  '<td>' +
                    '<a class="host-info" rel="tooltip" data-original-title="' + Utils.table({'version': json.version, 'last_ip': json.host}) + '">' +
                    '<i class="icon-info"></i></a> <span class="last-activity timeago" title="' + json.date + '">' + DateTime.template(json.date, true) + '</span>' +
                  '</td>' +
                  '<td>' +
                    '<a onclick="" class="btn btn-small btn-unlink" data-tr="unlink" data-id="' + key + '">Unlink</a>' +
                  '</td>' +
                '</tr>';

      $('#account-table > tbody').append(out);
    }
    else
      $(id).show();
  },


  // Чтение тела фида hosts.
  read: function(json)
  {
    if (json === false)
      return;

    $('.host-row').remove();

    for (var key in json) if (json.hasOwnProperty(key) && key.length == 34) {
      Hosts.processSingleHost(key, json[key]);
    }

    Loader.loadJS('qrc:/js/jquery.timeago.' + Utils.tr('lang') + '.js', function() {
      $('.timeago').timeago();
    });

    $('#hosts-content p').show();
    $('#hosts-content #fieldset').show();
    $('.host-row:hidden').remove();
    $('a[rel=tooltip]').tooltip();

    Loader.spinner.remove('loading/hosts');
    Utils.retranslate();
  },


  reload: function()
  {
    if (Pages.current != 1)
      return;

    if (SimpleChat.isOnline())
      Loader.spinner.add('loading/hosts');

    var channel = SimpleChat.channel(Settings.getId());
    if (channel !== null)
      $('#info-content > h1').html(Messages.nameTemplate(channel));

    Utils.TR('my_computers');
    Hosts.read(SimpleChat.feed(FEED_NAME_HOSTS, 1));
  },

  progress: false
};

Pages.onInfo.push(Hosts.reload);

try {
  ChatView.feed.connect(Hosts.feed);
  ChatView.reload.connect(Hosts.reload);
} catch (e) {}


/*
 * Авторизационный диалог.
 */
var AuthDialog = {
  /*
   * Показ диалога.
   */
  show: function()
  {
    Modal.current = 'auth';
    Modal.create.auth(null);

    $('#modal').modal();
  },

  /*
   * Формирование кнопок с провайдерами.
   */
  providers: function()
  {
    var providers = Auth.providers();
    if (!providers.hasOwnProperty('order'))
      return;

    for (var i = 0; i < providers.order.length; i++) {
      var provider = providers.order[i];
      if (providers.providers.hasOwnProperty(provider))
        AuthDialog.addProvider(provider, providers.providers[provider]);
    }

    $('#modal-header h3 .icon-spinner').remove();
    Utils.adjustWidth($('#providers .btn'));
  },

  /*
   * Добавление одиночного провайдера.
   */
  addProvider: function(name, data) {
    $('#providers').append('<a class="btn" href="' + data.url + ' "><i class="provider-' + name + '"></i> ' + data.htmlName + '</a>');
  },

  /*
   * Скрытие диалога.
   */
  hide: function()
  {
    $('#modal').modal('hide');
  }
};


Modal.create.auth = function(event)
{
  $('#modal-header h3 *').remove();
  $('#modal-body *').remove();

  $('#modal-header h3').append('<span></span>');
  $('#modal-header h3').append(' <i class="icon-spinner"></i>');
  $('#modal-header h3 span').text(Utils.tr('sign_in_with'));
  $('#modal-header h3 span').attr('data-tr', 'sign_in_with');

  $('#modal-body').append('<div id="providers"></div>');

  if (Auth.anonymous()) {
    $('#modal-body').append('<div id="anonymous-auth"><a data-tr="or_anon_connect" href="#">' + Utils.tr('or_anon_connect') + '</a></div>');

    $('#anonymous-auth a').on('click', function(event) {
      event.preventDefault();
      Auth.open();
    });
  }

  AuthDialog.providers();
};


Modal.shown.auth = function()
{
  Utils.adjustWidth($('#providers .btn'));
};


Modal.hidden.auth = function()
{
  Auth.cancel();
};


if (typeof Auth === 'undefined') {
  Auth = {
    providers: function() { return {}; },
    cancel: function()    {},
    anonymous: function() { return true; },
    open: function()      {},
    retry: function()     {}
  }
}
else {
  Auth.providersReady.connect(AuthDialog.providers);
}


var ServerInfo = {
  interval: null,

  /*
   * Подключение клиента к серверу.
   */
  online: function() {
    Settings.id = SimpleChat.serverId();
    $('#server-peer').text(SimpleChat.serverPeer());

    if (Pages.current === 1)
      ServerInfo.setInterval();

    Utils.TR('activity');
    Utils.TR('activity_sent');
    Utils.TR('activity_receved');
    Utils.TR('server_version');
    Utils.TR('encryption');

    var data = SimpleChat.encryption();
    var encryption = $('#server-encryption');

    if (data !== null) {
      encryption.removeAttr('data-tr');
      encryption.removeClass('red-text');

      var protocol = '';
      if (data.protocol == 2)
        protocol = 'TLS v1';
      else if (data.protocol == 0)
        protocol = 'SSL v3';

      encryption.html(protocol + ' <a href="' + data.url + '">' + data.CN + '</a> <span id="encryption-cipher">' + data.cipher + '</span>');
    }
    else {
      encryption.addClass('red-text');
      encryption.attr('data-tr', 'no-encryption');
      encryption.text(Utils.tr('no-encryption'));
    }

    ServerInfo.server(SimpleChat.feed(Settings.id, FEED_NAME_SERVER, 0));
  },


  /*
   * Отключение клиента от сервера.
   */
  offline: function() {
    $('#server-peer').text('');

    ServerInfo.clearInterval();
  },


  /*
   * Обновление информации о полученном и переданном трафике.
   */
  updateActivity: function()
  {
    var data = SimpleChat.traffic();
    $('#sent').html(SimpleChat.bytesToHuman(data.tx));
    $('#receved').html(SimpleChat.bytesToHuman(data.rx));
  },


  /*
   * Запуск периодического обновления информации о трафике.
   */
  setInterval: function() {
    ServerInfo.updateActivity();

    if (ServerInfo.interval === null)
      ServerInfo.interval = window.setInterval(ServerInfo.updateActivity, 1000);
  },


  /*
   * Остановка обновления информации о трафике.
   */
  clearInterval: function() {
    if (ServerInfo.interval !== null) {
      window.clearInterval(ServerInfo.interval);
      ServerInfo.interval = null;
    }
  },


  /*
   * Выравнивание ширины элементов.
   */
  adjustWidth: function() {
    Utils.adjustWidth($('#server-activity .field-row-label'));
    Utils.adjustWidth($('#server-feed .field-row-label'));
  },


  /*
   * Чтение фидов.
   */
  feed: function(json) {
    if (json === false || json.status !== 200 || json.name !== FEED_NAME_SERVER)
      return;

    ServerInfo.server(json.data);
  },


  /*
   * Чтение фида FEED_NAME_SERVER.
   */
  server: function(json) {
    if (json === false)
      return;

    $('#server-version').text(json.version);
  }
};


$(document).ready(function() {
  $('body').on('click', '.retry-auth', function(event) {
    event.preventDefault();

    Auth.retry();
  });

  $('body').on('click', '.btn-unlink', function(event) {
    event.preventDefault();

    SimpleChat.request(SimpleChat.id(), FEED_METHOD_DELETE, FEED_NAME_HOSTS + '/' + $(this).attr('data-id'), {'options':2});
  });
});


Pages.onInfo.push(ServerInfo.setInterval);
Pages.onInfo.push(ServerInfo.adjustWidth);
Pages.onMessages.push(ServerInfo.clearInterval);

if (typeof SimpleChat !== 'undefined') {
  SimpleChat.offline.connect(ServerInfo.offline);
  SimpleChat.online.connect(ServerInfo.online);
  SimpleChat.retranslated.connect(ServerInfo.adjustWidth);

  ChatView.feed.connect(ServerInfo.feed);
}
