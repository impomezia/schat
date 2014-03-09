(function() {
  'use strict';

  var cache = {};

  /**
   * Обновление состояния элемента.
   *
   * @param item
   * @param elem
   */
  function update(item, elem) {
    if (typeof item === 'string')
      item = PreviewPlugin.findById(item);

    console.log(item);

    if (item === null || item.state === 2) {
      elem.setAttribute('class', 'img-thumbnail img-thumbnail-error');
      return;
    }

    if (item.state === 0) {
      elem.setAttribute('class', 'img-thumbnail img-thumbnail-spinner');
      return;
    }
  }


  /**
   * Слот для обработки изменения состояния сообщения.
   *
   * @param id
   */
  function onChanged(id) {
    if (!cache.hasOwnProperty(id))
      return;

    var list = cache[id];
    var item = PreviewPlugin.findById(id);

    for (var i = 0; i < list.length; ++i) {
      update(item, list[i]);
    }
  }


  /**
   * Обработка добавления нового сообщения.
   *
   * @param id Идентификатор сообщения.
   */
  function onAdd(id) {
    var images = PreviewPlugin.findByOID(id);
    if (images === null)
      return;

    var container = document.getElementById(id);
    if (container === null)
      return;

    var block = document.createElement('div');
    block.setAttribute('class', 'thumbnail-block');

    for (var i = 0; i < images.length; ++i) {
      id = images[i];

      var a = document.createElement('a');

      var item = PreviewPlugin.findById(id);
      if (item !== null) {
        a.setAttribute('href',  item.url);
        a.setAttribute('title', item.url);
      }

      update(item, a);

      cache[id] = cache[id] || [];
      cache[id].push(a);

      block.appendChild(a);
    }

    container.firstChild.appendChild(block);
  }

  Messages.onAdd.push(onAdd);

  if (typeof window.PreviewPlugin === 'undefined') {
    window.PreviewPlugin = {
      findById: function(id)  { return null; },
      findByOID: function(id) { return null; }
    }
  }
  else {
    PreviewPlugin.changed.connect(onChanged);
  }
})();