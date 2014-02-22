
var ChannelsList = {
  /*
   * Динамический перевод интерфейса.
   */
  retranslate: function() {
    $('#join-name').attr('placeholder', Utils.tr('channels_channel_name'))
  },


  /*
   * Успешное подключение к серверу.
   */
  online: function() {
    var channel = SimpleChat.channel(SimpleChat.serverId());
    if (channel !== null)
      ChannelsList.setName(channel.Name);

    ChannelsList.get();
  },


  /*
   * Загрузка списка каналов.
   */
  get: function() {
    SimpleChat.feed(SimpleChat.serverId(), FEED_NAME_LIST, 1);
    Loader.spinner.add('loading/channels');
  },


  /*
   * Обновление имени сервера.
   */
  setName: function(name) {
    $('#server-name').text(name);
  }
};


/*
 * Чтение данных фидов.
 */
ChannelsList.feed = {
  /*
   * Чтение новых данных.
   */
  read: function(json) {
    if (json.feed == FEED_NAME_LIST) {
      if (json.type == 'body') {
        ChannelsList.feed.list(json.data, json.status);
      }
      else if (json.type == 'reply' && json.path == 'id') {
        CreateWindow.feed(json.data, json.status);
      }
    }
  },


  /*
   * Чтение фида FEED_NAME_LIST.
   */
  list: function(json, status) {
    $('.channel-item').remove();

    if (status == 200 || status == 303) {
      var channels     = json.channels;
      var channelsList = $('#channels-list');

      for (var i = 0; i < channels.length; i++) {
        var channel = channels[i];
        if (channel.length < 4)
          continue;

        channelsList.append(ChannelsList.feed.listItem(channel));
      }
    }

    Loader.spinner.remove('loading/channels');
  },


  /*
   * Формирование HTML кода для канала.
   */
  listItem: function(list) {
    var title = list[3];
    if (title == '')
      title = '<span data-tr="channels_no_title">' + Utils.tr('channels_no_title') + '</span>';

    var options = list[4];

    return '<div class="channel-item">' +
             '<div class="channel-item-header">' +
               '<i class="icon-' + (options & 4 ? 'secure' : 'channel') + '"></i>&nbsp;<a href="' + ChannelsView.toUrl(list[0], list[1]) + '" class="nick color-' + (options & 4 ? 'green' : 'default') + '" id="' + list[0] + '">' + list[1] + '</a>' +
               '<span class="badge">' + list[2] + (options & 2  ? '</span>&nbsp;<i class="icon-pin"></i>' : '') +
             '</div>' +
             '<div class="channel-item-title">' + title + '</div>' +
           '</div>'
  }
};


/*
 * Диалог создания канала.
 */
var CreateWindow = {
  text: null,


  /*
   * Загрузка диалога создания канала.
   */
  load: function(html) {
    $('#modal-body').html(html);

    $('#channel-create [data-tr]').each(function() {
      Utils.TR($(this).attr('data-tr'));
    });

    Utils.adjustWidth($('#channel-create .control-label'));

    var nameEdit = $('#create-name');
    nameEdit.focus();

    $('#create-submit').on('click.create', function(event) {
      event.preventDefault();

      CreateWindow.text = nameEdit.val().replace(/\s{2,}/g, ' ').trim();
      var text = CreateWindow.text;

      if (text.length == 0)
        return;

      if (text.length < 3) {
        CreateWindow.setError('channels_bad_name');
        return;
      }

      SimpleChat.request(SimpleChat.serverId(), FEED_METHOD_GET, LIST_FEED_ID_REQ, {'value':text});
    });

    nameEdit.on('input',function(e){
      CreateWindow.clearError();
    });
  },


  /*
   * Очистка состояния ошибки.
   */
  clearError: function() {
    $('#create-name-row').removeClass('error');
    $('.alert').hide();
  },


  /*
   * Установка состояния ошибки.
   */
  setError: function(text) {
    var createError = $('#create-error');
    createError.text(Utils.tr(text));
    createError.attr('data-tr', text);

    $('#create-name-row').addClass('error');
    $('.alert').show();
    $('#create-name').focus();
  },


  /*
   * Чтение ответа на запрос LIST_FEED_ID_REQ.
   */
  feed: function(json, status) {
    if (!('#channel-create').length)
      return;

    if (status == 403) {
      ChannelsView.create(json.value, CreateWindow.text, $('#access-checkbox').is(':checked'));
      $('#modal').modal('hide');
    }
    else if (status == 400)
      CreateWindow.setError('channels_bad_name');
    else if (status == 200)
      CreateWindow.setError('channels_already_exists');


  }
}


/*
 * Создание модального диалога для создания канала.
 */
Modal.create.create = function(event) {
  var h3 = $('#modal-header h3');
  h3.text(Utils.tr('channels_create_title'));
  h3.attr('data-tr', 'channels_create_title');

  $.ajax({
    url: 'create.html',
    isLocal: true,
    dataType: 'html',
    success: CreateWindow.load
  });
}


$(document).ready(function() {
  Utils.retranslate();
  ChannelsList.retranslate();

  ChannelsList.online();
  Modal.init();

  /*
   * Обработка нажатия на кнопку входа в канал.
   */
  $('#join').on('click.join', function(event) {
    event.preventDefault();

    var joinName = $('#join-name');
    var name     = joinName.val();

    if (name.length >= 3) {
      ChannelsView.join(name);
      joinName.val('');
    }
  });


  /*
   * Обработка нажатия на ссылку входа в канал по его идентификатору.
   */
  $('body').on('click.channel', '.nick', function (event) {
    event.preventDefault();
    ChannelsView.join($(this).attr('id'));
  });
});


if (typeof SimpleChat !== 'undefined') {
  SimpleChat.retranslated.connect(ChannelsList.retranslate);
  SimpleChat.online.connect(ChannelsList.online);
}


if (typeof ChannelsView === 'undefined') {
  ChannelsView = {
    join:   function(name)               {},
    create: function(id, name, _private) {},
    toUrl:  function(id, name)           { return '#'; }
  }
}
else {
  ChannelsView.feed.connect(ChannelsList.feed.read);
  ChannelsView.serverRenamed.connect(ChannelsList.setName);
}
