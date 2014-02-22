/* $Id: YouTube.js 3686 2013-06-09 23:50:29Z IMPOMEZIA $
 * IMPOMEZIA Simple Chat
 * Copyright (c) 2008-2013 IMPOMEZIA <schat@impomezia.com>
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

var YouTube = {
  onAdd: function(id) {
    if (document.getElementById(id).getElementsByClassName('youtube').length == 0)
      return;

    var youtube = $('#' + id + ' .youtube');
    youtube.data('state', 0);

    youtube.each(function() {
      var vid = $(this).attr('data-youtube-v');
      $(this).after(' <i data-youtube-v="' + vid + '" class="icon-spinner-small"></i>');

      $.ajax({
        url: 'http://gdata.youtube.com/feeds/api/videos/' + vid + '?alt=json',
        dataType: 'json',
        success: function(data) {
          YouTube.setTitle(data, vid);
        },
        error: function() {
          $('.icon-spinner-small[data-youtube-v="' + vid + '"]').remove();
        }
      });
    });
  },

  setTitle: function(data, vid) {
    $('.icon-spinner-small[data-youtube-v="' + vid + '"]').remove();

    var youtube = $('.youtube[data-youtube-v="' + vid + '"]');
    youtube.text(data.entry["title"].$t);

    if (YouTube.isFlashInstalled()) {
      youtube.each(function() {
        if ($(this).data('state') === 0) {
          $(this).data('state', 1);

          var id = SimpleChat.randomId();
          $(this).after('<a class="btn btn-youtube" id="' + id + '"><i class="icon-plus-small"></i></a>');
          $('#' + id).data('state', 0);

          $('#' + id).on('click', function() {
            YouTube.click(id, vid);
          });
        }
      });
    }

    if (Settings.scrollTo === null)
      alignChat();
  },

  click: function(id, vid) {
    var button = $('#' + id);

    if (button.data('state') === 1) {
      button.html('<i class="icon-plus-small"></i>');
      button.data('state', 0);
      $('#' + id + '-player').remove();
      alignChat();
    }
    else {
      $('.btn-youtube').html('<i class="icon-plus-small"></i>');
      $('.btn-youtube').data('state', 0);
      $('.youtube-player').remove();

      button.html('<i class="icon-minus-small"></i>');
      button.data('state', 1);
      button.after(YouTube.player(id, vid));
      alignChat();
      document.getElementById(id).scrollIntoView();
    }
  },

  player: function(id, vid) {
    return '<div class="youtube-player" id="' + id + '-player">' +
      '<object width="480" height="360">' +
      '<param name="movie" value="https://www.youtube.com/v/' + vid + '?fs=1">' +
      '<param name="allowFullScreen" value="true">' +
      '<embed src="https://www.youtube.com/v/' + vid + '?fs=1"' +
      'type="application/x-shockwave-flash"' +
      'allowfullscreen="true"' +
      'width="480" height="360">' +
      '</embed></object></div>';
  },

  isFlashInstalled: function() {
    if (navigator.plugins && navigator.plugins.length > 0) {
      var type = 'application/x-shockwave-flash';
      var mimeTypes = navigator.mimeTypes;
      if (mimeTypes && mimeTypes[type] && mimeTypes[type].enabledPlugin && mimeTypes[type].enabledPlugin.description && SimpleChat.value('YouTube/EmbedVideo') == 'true')
        return true;
    }

    return false;
  }
};

Messages.onAdd.push(YouTube.onAdd);
