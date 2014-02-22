
var Console = {
  current: 'try',

  /*
   * Обновление имени сервера.
   */
  setName: function(name) {
    var tag = $('.server-name');
    if (tag.length)
      tag.text(name);
  }
};


/*
 * Чтение данных фидов.
 */
Console.feed = {
  /*
   * Чтение новых данных.
   */
  read: function(json) {
    if (json.type == 'reply') {
      if (!json.path.length)
        return;

      var path = json.path;
      if (path == 'try')
        path = 'tryAccess';

      var cmd = json.cmd;
      if (cmd == FEED_METHOD_DELETE)
        cmd = 'del';

      try {
        Console.feed[json.feed][cmd][path](json);
      }
      catch (e) {}
    }
    else if (json.type == 'body') {
      try {
        Console.feed[json.feed]['body'](json.data, json.id, json.status);
      }
      catch (e) {}
    }
  },


  /*
   * Чтение ответов на "get", "put", "post" и "delete" запросы.
   */
  console: {
    get: {},
    put: {},
    post: {},
    del: {}
  },

  storage: {
    get: {},
    put: {},
    post: {},
    del: {}
  },

  server: {
    get: {},
    put: {},
    post: {},
    del: {}
  },

  users: {
    get: {},
    put: {},
    post: {},
    del: {}
  }
};


/*
 * Чтение ответа на "get" запрос "console/try".
 */
Console.feed.console.get.tryAccess = function(json) {
  Loader.spinner.remove('loading/try');
  var page = $('#page');

  if (json.status == 200) {
    Console.home.load();
  }
  else if (json.status == 402) {
    Console.login.load();
  }
  else if (json.status == 403) {
    page.html('<div class="alert alert-error" data-tr="console_bad_server">' + Utils.tr('console_bad_server') + '</div>');
    $('#page a').on('click', function(event) {
      event.preventDefault();
      ConsoleView.openUrl($(this).attr('href'));
    });
  }
  else {
    page.html('<div class="alert alert-error"><strong>' + json.status + '</strong> ' + SimpleChat.statusText(json.status) + '</div>');
  }
};


$(document).ready(function() {
  Loader.spinner.add('loading/try');
  Modal.init();

  SimpleChat.get(SimpleChat.serverId(), 'console/try');
});


if (typeof ConsoleView === 'undefined') {
  ConsoleView = {
    toPassword: function(password)     { return password; },
    expirationText: function(days)     { return days;},
    setName: function(channelId, name) { return false; },
    openUrl: function(url)             {}
  }

}
else {
  ConsoleView.feed.connect(Console.feed.read);
  ConsoleView.serverRenamed.connect(Console.setName);
}
