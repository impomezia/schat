/* $Id: about.js 3313 2012-12-09 22:03:33Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright (c) 2008-2012 IMPOMEZIA <schat@impomezia.com>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

var AboutUtils = {
  retranslate: function() {
    $("[data-tr]").each(function() {
      AboutUtils.TR($(this).attr("data-tr"));
    });
  },

  tr: function(key) {
    return SimpleChat.translate(key);
  },

  TR: function(key) {
    $("[data-tr='" + key + "']").html(AboutUtils.tr(key));
  },

  toggle: function() {
    var body = $('#' + $(this).attr('id') + '-body');
    if (body.is(':hidden')) {
      body.show();
      $(this).removeClass('toggle-expand');
      $(this).addClass('toggle');

      if (body.attr('id') == 'other-body')
        AboutUtils.adjustWidth($('#other-body .field-row-label'));
    }
    else {
      body.hide();
      $(this).removeClass('toggle');
      $(this).addClass('toggle-expand');
    }
  },

  version: function(text) {
    var ver = About.version(text);
    if (ver === null)
      return '';

    return '<div class="field-row">' +
           '  <span class="field-row-label">' + text + '</span>' +
           '  <span class="field-row-value">' + ver + '</span>' +
           '</div>';
  },

  adjustWidth: function(obj) {
    if (!obj.length || obj.width() < 1 || !obj.is(':visible'))
      return;

    obj.css('width', '');

    var max = 0;
    var current = 0;

    obj.each(function() {
      current = $(this).width();
      if (current > max)
        max = current;
    });

    obj.width(max + 4);
  }
};


$(document).ready(function() {

  $('.header').click(AboutUtils.toggle);

  AboutUtils.retranslate();
  $('#version').html('<a href="http://wiki.schat.me/Simple_Chat_' + About.version('app') + '">' + About.version('app') + '</a>');
  $('#preferences').html(About.path('preferences'));

  var third_parties = $('#other-body');
  third_parties.append(AboutUtils.version('Qt'));
  third_parties.append(AboutUtils.version('QtWebKit'));
  third_parties.append(AboutUtils.version('WebKit'));
});

if (typeof About === "undefined") {
  About = {
    path: function(type) { return type; },
    version: function(type) { return type; }
  };
}

if (typeof SimpleChat === "undefined") {
  SimpleChat = {
    translate: function(key) { return key; }
  };
}
else {
  SimpleChat.retranslated.connect(AboutUtils.retranslate);
}
