/* Simple Chat
 * Copyright (c) 2008-2015 Alexander Sedov <imp@schat.me>
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

(function(){
  function onAdd(id) {
    var elements = document.getElementById(id).getElementsByClassName('youtube');
    if (!elements.length) {
      return;
    }

    Array.prototype.forEach.call(elements, function(item) {
      if (item.getAttribute('data-done') == 1) {
        return;
      }

      item.setAttribute('data-done', '1');
      var vid = item.getAttribute('data-youtube-v');

      $.ajax({
        url: 'https://www.googleapis.com/youtube/v3/videos?id=' + vid + '&key=AIzaSyB7nwNkbwwchMD2Xd2hIl1iIsznwrq3YKI&part=snippet',
        dataType: 'json',
        success: function(data) {
          if (!data || !data.items || !data.items.length) {
            return;
          }

          var snippet = data.items[0].snippet;
          item.textContent = snippet.title;

          var thumb = document.createElement('a');
          thumb.setAttribute('class', 'youtube-thumbnail');
          thumb.href  = item.href;
          thumb.title = snippet.description;
          thumb.style.backgroundImage = 'url("' + snippet.thumbnails.default.url + '")';

          item.parentNode.insertBefore(thumb, item.nextSibling);

          alignChat();
        }
      })
    });
  }

  Messages.onAdd.push(onAdd);
})();
