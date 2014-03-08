(function() {
  'use strict';

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
      var a = document.createElement('a');
      a.setAttribute('class', 'img-thumbnail img-thumbnail-spinner');

      block.appendChild(a);
    }

    container.firstChild.appendChild(block);
  }

  Messages.onAdd.push(onAdd);

  if (typeof window.PreviewPlugin === 'undefined') {
    window.PreviewPlugin = {
      findByOID: function(id) { return null; }
    }
  }
  else {
  }
})();