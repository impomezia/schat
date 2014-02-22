/* $Id: Profile.js 3656 2013-04-23 17:24:26Z IMPOMEZIA $
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

/*
 * Отображение поля "country".
 *
 * Код страны преобразуется из кода в название страны и изображение флага.
 */
Profile.Field.country = function(key, value) {
  if (value.length != 2)
    return;

  var name = Utils.tr('country-' + value);
  if (name == 'country-' + value)
    return;

  Profile.addRow(key, '<i class="flag-' + value + '"></i> <span data-tr="country-' + value + '">' + name + '</span>');
};


/*
 * Для отображения поля "city" используется тот же обработчик что и для поля "name".
 */
Profile.Field.city = Profile.Field.name;


/*
 * Отображение поля "site".
 *
 * Значение поля преобразуется в кликабельную ссылку.
 */
Profile.Field.site = function(key, value) {
  var addr = Utils.left(htmlspecialchars(value.replace(/\s+/gi, '')), 254);
  var title = addr;
  if (addr.indexOf('http://') == 0)
    title = addr.slice(7);
  else if (addr.indexOf('https://') == 0)
    title = addr.slice(8);
  else
    addr = 'http://' + addr;

  Profile.addRow(key, '<a href="' + addr + '">' + title + '</a>');
};


/*
 * Отображение поля "email".
 *
 * Значение поля преобразуется в кликабельную ссылку.
 */
Profile.Field.email = function(key, value) {
  var addr = Utils.left(htmlspecialchars(value.replace(/\s+/gi, '')), 254);
  if (addr.indexOf('mailto:') == 0)
    addr = addr.slice(7);

  Profile.addRow(key, '<a href="mailto:' + addr + '">' + addr + '</a>');
};


var ProfilePlugin = {
  /*
   * Обработка показа информации о подключении.
   *
   * Добавляется отображение гео информации.
   */
  process: function(key, json) {
    var id = '#' + key;
    if (!json.hasOwnProperty('geo')) {
      $(id + ' [class^="flag-"]').remove();
      return;
    }

    var country = json.geo.country.toLowerCase();
    if (country.length != 2)
      return;

    $(id).append(' <span><i class="flag-' + country + '" title="' + Utils.tr('country-' + country) + '"></i></span>');
  },


  /*
   * Обработка показа модального окна.
   *
   * Добавляется отображение гео информации.
   */
  connection: function(json) {
    var modal = $('#modal-body');

    if (typeof json.tz === "number") {
      var date = new Date();
      date = new Date(date.getTime() + date.getTimezoneOffset() * 60000 + json.tz * 1000);

      modal.append(Utils.row('field-tz', DateTime.time(date)));
    }

    if (json.hasOwnProperty('geo')) {
      var country = json.geo.country.toLowerCase();
      if (country.length == 2) {
        modal.append(Utils.row('field-country', '<i class="flag-' + country + '"></i> '
          + '<span data-tr="country-' + country + '">' + Utils.tr('country-' + country) + '</span>'));
      }

      if (json.geo.org != '') {
        var org = json.geo.org;
        var index = org.indexOf(' ');
        if (index != -1)
          modal.append(Utils.row('field-isp', Utils.left(org.slice(index), 100)));
      }
    }
  }
};

UserHooks.process.push(ProfilePlugin.process);
UserHooks.connection.push(ProfilePlugin.connection);
