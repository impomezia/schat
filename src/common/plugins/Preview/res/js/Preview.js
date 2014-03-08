if (typeof PreviewPlugin === 'undefined') {
  PreviewPlugin = {
    findByOID: function(id) { return null; }
  }
}
else {
}


(function() {
  'use strict';

  function onAdd(id) {
    console.log(PreviewPlugin.findByOID(id));
  }

  Messages.onAdd.push(onAdd);
})();