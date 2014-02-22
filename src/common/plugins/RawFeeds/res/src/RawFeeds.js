/* $Id: RawFeeds.js 3678 2013-06-08 02:18:53Z IMPOMEZIA $
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

RawFeeds = {
  /*
   * Формирование HTML кода со ссылками на фиды.
   */
  headers: function(json) {
    var html = '<div class="feeds-headers">';
    for (var key in json) if (json.hasOwnProperty(key)) {
      html += RawFeeds.feed(key, json[key]);
    }

    html += '</div>';
    return html;
  },

  /*
   * Формирование HTML кода для отображения информации об одиночном фиде.
   */
  feed: function(name, data) {
    return '<div class="feed-header">' + RawFeeds.icon(data.cached) + DateTime.template(data.date, true) + ' <a href="#" class="feed-name">' + name + '</a></div>';
  },

  /*
   * Базовая информация о фиде.
   */
  feedInfo: function(json) {
    return '<div class="feed-info">Last modified: <b>' + DateTime.template(json.date, true) + '</b><br>Size: <b>' + json.size + '</b></div>';
  },

  /*
   * Функциональность кнопки для показа сырого содержимого фида.
   */
  raw: function(json) {
    if (!json.Text.length)
      return;

    var id = json.Id;
    $('#' + id + ' .raw-feeds-title').append(' <a class="btn btn-toggle" id=""><i class="icon-plus-small"></i></a>');
    $.JSONView(json.Text, $('#' + id + " .jsonoutput"));

    $('#' + id + ' .btn-toggle').on('click', function(event) {
      var c = $(this).children();
      if (c.attr('class') == 'icon-plus-small') {
        $('#' + id + ' .jsonoutput').show();
        c.attr('class', 'icon-minus-small');
      }
      else {
        $('#' + id + ' .jsonoutput').hide();
        c.attr('class', 'icon-plus-small');
      }

      alignChat();
    });
  },

  /*
   * Иконка фида в зависимости от того кэширован он или нет.
   */
  icon: function(cached) {
    return '<i class="icon-feed' + (cached ? '-cached' : '') + '"></i> ';
  }
};


Messages.addRawFeedsMessage = function(json)
{
  var block = document.createElement('div');
  block.id = json.Id;
  block.setAttribute('class', 'container ' + json.Type + '-type');

  var html = '<div class="blocks ';
  if (json.Extra !== undefined)
    html += json.Extra;

  html += '"><div class="alert ' + (json.Status.Code == 200 ? 'alert-info' : 'alert-error') +'">' +
          '<a class="close" data-dismiss="alert" href="#">×</a>';

  html += '<div class="raw-feeds-header">';
  html += '<h3 class="raw-feeds-title">' + json.Title + '</h3>';
  if (json.Status.Code != 200)
    html += ' <span class="raw-feeds-bad-status">' + json.Status.Code + ' ' + json.Status.Desc + '</span>';

  html += '</div>';

  var command = json.Command;
  if (command == 'headers')
    html += RawFeeds.headers(json.Data);
  else if (command == 'feed')
    html += RawFeeds.feedInfo(json.Data);

  html += '<div class="jsonoutput" style="display:none;"></div>';

  html += '</div>';
  html += '</div>';

  block.innerHTML = html;

  Messages.addRawMessage(block, json.Hint.Day);
  RawFeeds.raw(json);

  if (command == 'feed')
    $('#' + json.Id + ' .raw-feeds-title').addClass('raw-feeds-title-icon');

  alignChat();
};


$(document).ready(function() {
  $('body').on('click', '.feed-name', function(event) {
    event.preventDefault();
    SimpleChat.feed(Settings.id, $(this).text(), 2);
  });
});
