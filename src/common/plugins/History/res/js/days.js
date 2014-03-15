/* $Id: days.js 3684 2013-06-09 21:00:44Z IMPOMEZIA $
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

Messages.days = [];

/*
 * Создание при необходимости блока с сообщениями за 1 день.
 */
Messages.day = function(day) {
  if (document.getElementById('day-' + day) !== null)
    return;

  var block = document.createElement('div');
  block.id = 'day-' + day;
  block.innerHTML =
             '<div class="day-header expanded" id="day-header-' + day + '">' +
               '<div class="day-day"><a href="#" data-day="' + day + '">' + SimpleChat.day(day) + '</a></div>' +
               '<div class="day-close"><a class="close" href="#">&times;</a></div>' +
               '<div class="day-badge">0</div>' +
             '</div>' +
             '<div class="day-body" id="day-body-' + day + '"></div>';

  if (Messages.days.indexOf(day) == -1) {
    Messages.days.push(day);
    Messages.days.sort();
  }

  var index = Messages.days.indexOf(day);
  if (index == Messages.days.length - 1) {
    document.getElementById('Chat').appendChild(block);
  }
  else {
    var before = document.getElementById('day-' + Messages.days[index + 1]);
    before.parentNode.insertBefore(block, before);
  }
};


Messages.addRawMessage = function(block, day) {
  document.getElementById('day-body-' + day).appendChild(block);
  Messages.add(block.id);
};


/*
 * Очистка страницы.
 */
Messages.reload = function()
{
  if (Pages.current != 0)
    return;

  $('.day').remove();
  Messages.days = [];
  alignChat();
};


DateTime.update = function(json) {
  var date  = new Date(json.Date);
  var block = $('#' + json.Id + ' .date-time-block');

  if (json.Day === true)
    block.children('.day').text(DateTime.day(date));

  block.children('.time').text(DateTime.time(date));
  block.children('.seconds').text(DateTime.seconds(date));

  var id = $('#' + json.Id);
  id.attr('data-time', json.Date);
  if (json.Hint.Hint == 'before' && document.getElementById(json.Hint.Id) !== null) {
    id.detach().insertBefore('#' + json.Hint.Id);
  }
  else {
    id.detach().appendTo('#day-' + json.Hint.Day + ' .day-body');
  }

  alignChat();
};


/*
 * Перевод дат при смене языка.
 */
Messages.retranslate = function() {
  $('[data-day]').each(function() {
    $(this).text(SimpleChat.day($(this).attr('data-day')));
  });
};

(function(){
  if (typeof ChatView !== 'undefined')
    ChatView.reload.connect(Messages.reload);

  if (typeof SimpleChat !== 'undefined')
    SimpleChat.retranslated.connect(Messages.retranslate);
})();

$(document).ready(function() {
  var body = $('body');

  /*
   * Сворачивание или разворачивание блока сообщений.
   */
  body.on('click', '.day-day a', function(event) {
    event.preventDefault();
    var prefix = '#day-' + $(this).attr('data-day');

    var header = $(prefix + ' .day-header');
    if (header.hasClass('expanded')) {
      header.removeClass('expanded');
      $(prefix + ' .day-body').hide();
    }
    else {
      header.addClass('expanded');
      $(prefix + ' .day-body').show();
      $(prefix + ' .day-badge').text(0);
      $(prefix + ' .day-badge').hide();
    }

    alignChat();
  });

  /*
   * Удаление блока сообщений.
   */
  body.on('click', '.day-close a', function(event) {
    event.preventDefault();

    var day = $(this).parent().parent().attr('id').substring(11);
    var block = document.getElementById('day-' + day);
    block.parentNode.removeChild(block);
    ChatView.removeDay(day);

    var index = Messages.days.indexOf(day);
    if (index != -1)
      Messages.days.splice(index, 1);

    alignChat();
  });

  /*
   * Показ кнопки закрытия.
   */
  body.on('mouseenter', '.day-day', function(event) {
    $('#day-' + $(this).parent().attr('id').substring(11) + ' .day-close').fadeIn('fast');
  });

  /*
   * Сокрытие кнопки закрытия.
   */
  body.on('mouseleave', '.day-header', function(event) {
    $('#day-' + $(this).parent().attr('id').substring(4) + ' .day-close').fadeOut('fast');
  });
});
