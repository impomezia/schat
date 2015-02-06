(function() {
  'use strict';

  var template = null;
  var rendered = false;
  var cacheTag = HistoryView.getTag(Settings.getId());

  Loader.loadTpl('qrc:/js/History/motd.mustache', function(err, tpl) {
    if (!err) {
      template = tpl;
    }
  });

  var ui = {
    switcher: document.getElementById('page-switcher-end'),
    counter:  document.createElement('div'),
    messages: document.createElement('div')
  };

  ui.messages.id = 'motd-messages';
  ui.counter.id  = 'motd-counter';
  ui.counter.textContent = '0';
  ui.switcher.appendChild(ui.counter);
  document.getElementById('page-body').appendChild(ui.messages);

  if (typeof ChatView !== 'undefined') {
    ChatView.feed.connect(onFeed);
  }


  Pages.onInfo.push(function() {
    if (Pages.current != 1) {
      return;
    }

    $(ui.switcher).removeClass('active');
    onInfo(SimpleChat.feed(Settings.getId(), FEED_NAME_INFO, 3), 300);
  });


  function onFeed(json) {
    if (!json || json.id != Settings.getId()) {
      return;
    }

    if (json.feed == FEED_NAME_INFO && json.cmd == 'get') {
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
    var tag = HistoryView.genTag(messages.map(function(message) { return message.id }));
    if (rendered && cacheTag === tag) {
      return;
    }

    if (Pages.current != 1 && cacheTag !== tag) {
      $(ui.switcher).addClass('active');
    }

    rendered = true;
    cacheTag = tag;
    HistoryView.setTag(Settings.getId(), tag);

    messages = messages.filter(function(message) { return !!message.text; });
    ui.counter.textContent = messages.length;

    if (!template || !messages.length) {
      ui.messages.innerHTML= '';
      return;
    }

    ui.messages.innerHTML = template.render({
      title: Utils.tr('history_motd_title'),
      messages: messages.map(function(item) {
        item.text = HistoryView.filter(item.id, item.text);
        return item;
      })
    });
  }
})();
