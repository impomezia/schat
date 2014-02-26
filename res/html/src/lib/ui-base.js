(function() {
  'use strict';

  window.schat.ui.main = document.getElementById('main');

  window.schat.ui.createElement = function(tagName, attrs, data) {
    var el = document.createElement(tagName);
    for (var attr in attrs) if (attrs.hasOwnProperty(attr)) {
      el.setAttribute(attr, attrs[attr]);
    }

    if (typeof data === 'string') {
      el.innerHTML = data;
    }
    else if (typeof data === 'object' && data.hasOwnProperty('text'))
      el.textContent = data.text;

    if (attrs && attrs.hasOwnProperty('data-tr'))
      el.innerHTML = tr(attrs['data-tr']);

    return el;
  };
})();