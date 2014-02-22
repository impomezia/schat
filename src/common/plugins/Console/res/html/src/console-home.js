
Console.home = {
  /*
   * Загрузка шаблона страницы.
   */
  load: function() {
    $.ajax({
      url: 'home.html',
      isLocal: true,
      dataType: 'html',
      success: Console.home.loaded
    });
  },


  /*
   * Обработка успешной загрузки страницы.
   */
  loaded: function(data) {
    $('#page').html(data);

    Console.current = 'home';
    Utils.retranslate();

    var channel = SimpleChat.channel(SimpleChat.serverId());
    if (channel !== null)
      Console.setName(channel.Name);

    $('#nav-logout').on('click.home',                     Console.home.logout);
    $('body').on('click.name', '#name-ok',                Console.home.setServerName);
    $('#oauth-ok').on('click.oauth-ok',                   Console.home.setOAuthServer);
    $('.auth-checkbox').on('click.oauth',                 Console.home.setAuthMethods);
    $('#main-channel-block :checkbox').on('click.policy', Console.home.setPolicy);

    Utils.adjustWidth($('#about-server-block .field-row-label'));
    Utils.adjustWidth($('#users-online-block .field-row-label'));
    Console.home.online();
  },


  /*
   * Успешное подключение к серверу.
   */
  online: function() {
    Console.home.getFeed(FEED_NAME_SERVER);
    Console.home.getFeed(FEED_NAME_USERS);

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

      encryption.html('<span class="green-text">' + protocol + '</span>');
      var days = data.daysToExpiry;

      if (days > 0 && days <= 42)
        encryption.append(' <span class="orange-text">' + ConsoleView.expirationText(days) + '</span>');

      if (days <= 0)
        encryption.append(' <span class="red-text" data-tr="console_cert_expired">' + Utils.tr('console_cert_expired') + '</span>');
    }
    else {
      encryption.addClass('red-text');
      encryption.attr('data-tr', 'no-encryption');
      encryption.text(Utils.tr('no-encryption'));
    }
  },


  /*
   * Загрузка фида.
   * Функция пытается использовать кэшированную версию фида и создаёт запрос к серверу для проверки изменился фид или нет.
   *
   * \param name Имя фида.
   */
  getFeed: function(name) {
    Console.feed[name].body(SimpleChat.feed(SimpleChat.serverId(), name, 3), SimpleChat.serverId(), 300);

    SimpleChat.feed(SimpleChat.serverId(), name, 1);
  },


  /*
   * Подготовка фида к чтению.
   */
  prepare: function(name, status) {
    if (status == 300) {
      Loader.spinner.add('loading/' + name);
      return true;
    }

    Loader.spinner.remove('loading/' + name);
    return status == 200;
  },


  /*
   * Выход из консоли.
   *
   * \param event Событие клика по ссылке выхода.
   */
  logout: function(event) {
    event.preventDefault();

    Loader.spinner.add('loading/logout');
    SimpleChat.request(SimpleChat.serverId(), FEED_METHOD_DELETE, CONSOLE_FEED_ME_REQ);
  },


  /*
   * Установка имени сервера.
   *
   * \param event Событие клика по кнопке OK в модальном окне.
   */
  setServerName: function(event) {
    event.preventDefault();
    ConsoleView.setName(SimpleChat.serverId(), $('#name-edit').val());
    $('#modal').modal('hide');
  },


  /*
   * Установка адреса сервера OAuth авторизации.
   *
   * \param event Событие клика по кнопке OK.
   */
  setOAuthServer: function(event) {
    event.preventDefault();
    $('#oauth-group').removeClass('error');
    SimpleChat.request(SimpleChat.serverId(), FEED_METHOD_PUT, SERVER_FEED_OAUTH_REQ, {'value':$('#oauth-server').val()});
  },


  /*
   * Установка доступных методов регистрации.
   */
  setAuthMethods: function() {
    var auth = [];
    if ($('#anonymous-auth').is(':checked'))
      auth.push('anonymous');

    if ($('#oauth-auth').is(':checked'))
      auth.push('oauth');

    SimpleChat.request(SimpleChat.serverId(), FEED_METHOD_PUT, SERVER_FEED_AUTH_REQ, {'value':auth});
  },


  /*
   * Установка политики основного канала.
   */
  setPolicy: function() {
    var policy = 0;
    if ($('#use-main-channel').is(':checked'))
      policy |= 1;

    if (policy & 1 && $('#forced-join').is(':checked'))
      policy |= 2;

    if (policy & 2 && $('#disable-leave').is(':checked'))
      policy |= 4;

    Console.home.showPolicy(policy);

    SimpleChat.request(SimpleChat.serverId(), FEED_METHOD_PUT, SERVER_FEED_POLICY_REQ, {'value':policy});
  },


  /*
   * Отображение текущей политики основного канала.
   */
  showPolicy: function(policy) {
    policy = policy || 0;

    var useMain    = $('#use-main-channel');
    var forcedJoin = $('#forced-join');
    var noLeave    = $('#disable-leave');

    useMain.attr('checked',    !!(policy & 1));
    forcedJoin.attr('checked', !!(policy & 2));
    noLeave.attr('checked',    !!(policy & 4));

    if (policy & 1)
      forcedJoin.parent().show();
    else
      forcedJoin.parent().hide();

    if (policy & 1 && policy & 2)
      noLeave.parent().show();
    else
      noLeave.parent().hide();
  }
};


/*
 * Чтение ответа на "delete" запрос "console/me".
 */
Console.feed.console.del.me = function(json) {
  if (json.status == 200) {
    Loader.spinner.add('loading/try');
    SimpleChat.get(SimpleChat.serverId(), 'console/try');
  }

  Loader.spinner.remove('loading/logout');
};


/*
 * Обработка получения тела фида FEED_NAME_SERVER.
 */
Console.feed.server.body = function(json, id, status) {
  if (!Console.home.prepare(FEED_NAME_SERVER, status) || json === false)
    return;

  var version = $('#server-version');
  version.text(json.version);
  version.append(' <i class="icon-os os-' + Pages.os(json.os) + '"></i>');

  $('#oauth-auth').attr('checked', json.auth.indexOf('oauth') != -1);
  $('#anonymous-auth').attr('checked', json.auth.indexOf('anonymous') != -1);
  $('#oauth-server').val(json.oauth);

  Console.home.showPolicy(json.policy);

  $('#auth-block').fadeIn('fast');
  $('#main-channel-block').fadeIn('fast');
};


/*
 * Чтение ответа на "put" запрос "server/oauth".
 */
Console.feed.server.put.oauth = function(json) {
  if (json.status != 200 && json.status != 303)
    $('#oauth-group').addClass('error');
};


/*
 * Обработка получения тела фида FEED_NAME_USERS.
 */
Console.feed.users.body = function(json, id, status) {
  if (SimpleChat.serverId() != id || !Console.home.prepare(FEED_NAME_USERS, status) || json === false)
    return;

  var now  = $('#users-online-now');
  var peak = $('#users-online-peak');

  now.text(json.count);
  peak.text(json.peak.count);
  peak.append(' ' + DateTime.template(json.peak.date, true));
};


Modal.create.name = function(event)
{
  var h3 = $('#modal-header h3');
  h3.text(Utils.tr('console_server_name'));
  h3.attr('data-tr', 'console_server_name');

  $('#modal-body').append(
    '<form id="title-form">' +
      '<div id="name-group" class="control-group">' +
        '<input id="name-edit" type="text" maxlength="20" class="input-append">' +
        '<button id="name-ok" type="submit" class="btn btn-prepend" data-tr="ok">' + Utils.tr('ok') + '</button>' +
      '</div>' +
    '</form>'
  );

  var channel = SimpleChat.channel(SimpleChat.serverId());
  if (channel !== null)
    $('#name-edit').val(channel.Name);
};


Modal.shown.name = function()
{
  $('#name-edit').focus();
};


if (typeof SimpleChat !== 'undefined') {
  SimpleChat.online.connect(Console.home.online);
}
