(function() {
  'use strict';

  schat.ui.MainWidget.prototype.toUrl = function(id, name, options) {
    return '<a href="' + ChannelsView.toUrl(id, name) + '" class="nick color-' + (options & 4 ? 'green' : 'default') + '" id="' + id + '">' + schat.utils.encode(name) + '</a>';
  };

  var navbar = new schat.ui.RoomsNavbar();
  var rooms  = new schat.ui.MainWidget();

  if (typeof ChannelsView === 'undefined') {
    window.ChannelsView = {
      join:   function(name)               {},
      create: function(id, name, _private) {},
      toUrl:  function(id, name)           { return '#'; }
    }
  }
  else {
    ChannelsView.feed.connect(function(json)      { feed(json) });
    ChannelsView.serverRenamed.connect(function() { navbar.setName.apply(navbar, arguments); });
  }


  if (typeof SimpleChat !== 'undefined') {
    SimpleChat.online.connect(function() { online(); });
    SimpleChat.retranslated.connect(function() { retranslate(); });
  }


  /**
   * Успешное подключение к серверу.
   */
  function online() {
    var channel = SimpleChat.channel(SimpleChat.serverId());
    if (channel !== null)
      navbar.setName.call(navbar, channel.Name);

    SimpleChat.feed(SimpleChat.serverId(), 'list',  1);
    SimpleChat.feed(SimpleChat.serverId(), 'stats', 1);
  }


  /**
   * Чтение новых данных фидов.
   */
  function feed(json) {
    if (json.feed == 'list') {
      if (json.type == 'body' && (json.status == 200 || json.status == 303))
        rooms.setRooms(json.data.channels);
      else if (json.type == 'reply' && json.path == 'id') {
        createRoom(json.data, json.status);
      }
    }
    else if (json.feed == 'stats' && json.type == 'body' && (json.status == 200 || json.status == 303))
      rooms.setStats(json.data);
  }


  function createRoom(json, status) {
    var dialog = schat.ui.modal.current;
    if (!dialog instanceof schat.ui.RoomCreateDialog)
      return;

    if (status == 403) {
      ChannelsView.create(json.value, dialog.name(), $(dialog.aclCheckbox).is(':checked'));
      $(schat.ui.modal.element).modal('hide');
    }
    else if (status == 400)
      dialog.setError('channels-bad-name');
    else if (status == 200)
      dialog.setError('channels-already-exists');
  }


  function retranslate() {
    navbar.joinName.setAttribute('placeholder', tr('channels-channel-name'));
    rooms.setStats(SimpleChat.feed(SimpleChat.serverId(), 'stats', 3));
  }


  function ready() {
    schat.lang.retranslate();
    retranslate();

    online();

    /**
     * Обработка нажатия на кнопку входа в канал.
     */
    $(navbar.joinBtn).on('click.join', function(event) {
      event.preventDefault();

      var name = navbar.joinName.value;

      if (name.length >= 3) {
        ChannelsView.join(name);
        navbar.joinName.value = '';
      }
    });


    /**
     * Обработка нажатия на ссылку входа в канал по его идентификатору.
     */
    $('body').on('click.channel', '.nick', function (event) {
      event.preventDefault();
      ChannelsView.join($(this).attr('id'));
    });


    /**
     * Создание диалога создания комнаты.
     */
    $(navbar.createBtn).on('click.create', function(event) {
      event.preventDefault();

      var dialog = new schat.ui.RoomCreateDialog();
      schat.ui.modal.current = dialog;

      $(schat.ui.modal.element).modal();

      $(dialog.createBtn).on('click.create', function(event) {
        event.preventDefault();

        var name = dialog.name();
        if (name.length == 0)
          return;

        if (name.length < 3) {
          dialog.setError('channels-bad-name');
          return;
        }

        SimpleChat.request(SimpleChat.serverId(), 'get', 'list/id', {value:name});
      })
    })
  }

  $(document).ready(ready);
})();