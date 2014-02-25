(function() {
  'use strict';

  var utils = window.schat.utils;

  utils.isFunction =  function(obj) {
    return typeof obj === 'function';
  };

  utils.isArray = Array.isArray || function(obj) {
    return Object.prototype.toString.call(obj) == '[object Array]';
  };

  utils.randomId = function() {
    var buffer = new Array(16);
    uuid.v4(null, buffer, 0);

    return base32.encode(buffer);
  };

  utils.format = function(text, data) {
    return text.replace(/{(\d+)}/g, function(match, number) {
      return typeof data[number] !== 'undefined' ? data[number] : match;
    });
  };
})();
