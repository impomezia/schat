(function() {
  'use strict';

  var template = null;

  Loader.loadTpl('qrc:/js/History/motd.mustache', function(err, tpl) {
    if (!err) {
      template = tpl;
    }
  });

  var ui = {
    messages: document.createElement('div')
  };

  ui.messages.id = 'motd-messages';
  document.getElementById('page-body').appendChild(ui.messages);


  if (typeof ChatView !== 'undefined') {
    ChatView.feed.connect(onFeed);
  }


  Pages.onInfo.push(function() {
    if (Pages.current != 1) {
      return;
    }

    onInfo(SimpleChat.feed(Settings.getId(), FEED_NAME_INFO, 3), 300);
  });


  function onFeed(json) {
    if (!json || json.id != Settings.getId()) {
      return;
    }

    if (json.feed == FEED_NAME_INFO) {
      onInfo(json.data, json.status);
    }
    else if (json.name == 'messages/fetch' && json.data.format == 'motd') {
      render(json.data.messages);
    }
  }


  function onInfo(json, status) {
    var messages = json.motd || [];
    if (!messages.length) {
      return render(messages);
    }

    SimpleChat.get(Settings.getId(), 'messages/fetch', { messages: messages, format: 'motd' });
  }


  function render(messages) {
    messages = messages.filter(notEmpty);

    if (!template || !messages.length) {
      ui.messages.innerHTML= '';
      return;
    }

    ui.messages.innerHTML = template.render({
      title: Utils.tr('history_motd_title'),
      messages: messages
    });

    function notEmpty(element) {
      return !!element.text;
    }
  }
})();
