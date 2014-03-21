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

    if (item === null || item.state === 2) {
      elem.setAttribute('class', 'img-thumbnail img-thumbnail-error');
      return;
    }

    if (item.state === 0) {
      elem.setAttribute('class', 'img-thumbnail img-thumbnail-spinner');
      return;
    }

    if (item.state === 1) {
      elem.setAttribute('class', 'img-thumbnail');

      if (item.flags & 1) {
        elem.style.backgroundImage = 'url("' + item.orig + '")';
        elem.style.backgroundSize  = (item.width > 90 ? 90 : item.width) + 'px ' + (item.height > 90 ? 90 : item.height) + 'px';
      }
      else {
        elem.style.backgroundImage = 'url("' + item.thumb + '")';
      }

      elem.addEventListener('click', function(event) {

        if (ChatView.openDialog('preview', item))
          event.preventDefault();

      });
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

    var thumbs = container.getElementsByClassName('img-thumbnail img-thumbnail-spinner');
    if (thumbs.length != images.length)
      return;

    var queue = [];

    for (var i = 0; i < images.length; ++i) {
      id = images[i];

      var a    = thumbs[i];
      var item = PreviewPlugin.findById(id);

      cache[id] = cache[id] || [];
      cache[id].push(a);

      queue.push({item: item, elem: a});
    }

    for (i = 0; i < queue.length; ++i) {
      update(queue[i].item, queue[i].elem);
    }
  }


  Messages.previewRemover = function(container) {
    Messages.genericRemover(container);

    var block = container.firstChild;
    block.removeChild(block.children[3]);
  };


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