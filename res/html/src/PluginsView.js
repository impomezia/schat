/* $Id: PluginsView.js 3289 2012-11-28 09:31:20Z IMPOMEZIA $
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

var Plugins = {
  add: function(data) {
    var html = '<div class="plugin-wrapper" id="' + data.id + '">' +
                  '<div class="plugin" style="background-image:url(' + data.icon + ');">' +
                    '<div class="plugin-details">' +
                      '<div class="plugin-header"><span class="plugin-title">' + data.title + '</span><span class="plugin-version">' + data.version + '</span></div>' +
                      '<div class="plugin-description">' + data.desc + '</div>' +
                      '<div class="restart-controls">' +
                        '<span class="restart-text"></span> ' +
                        '<a class="restart-link" data-tr="restart_now" href="#">' + Plugins.tr('restart_now') + '</a>' +
                      '</div>' +
                    '</div>' +
                    '<div class="plugin-controls">' +
                      '<div class="checkbox enable-checkbox">' +
                        '<label>' +
                          '<input type="checkbox"> ' +
                          '<span class="enable-checkbox-text">' +
                            '<span class="enabled-text" data-tr="enabled">' + Plugins.tr('enabled') + '</span>' +
                            '<span class="enable-text" data-tr="enable">' + Plugins.tr('enable') + '</span>' +
                          '</span>' +
                        '</label>' +
                      '</div>' +
                      (data.configurable === true ? '<div class="settings"><a class="settings-link" href="#" data-tr="settings">' + Plugins.tr('settings') + '</a></div>' : '') +
                    '</div>' +
                  '</div>' +
                '</div>';

    $('#plugins-list').append(html);

    var id = data.id;
    Plugins.enable(id, data.enabled);

    $('#' + id + ' input:checkbox').click(function() {
      var checked = $('#' + id + ' input:checkbox').prop('checked');
      PluginsView.enable(id, checked);
      Plugins.enable(id, checked);
    });

    $('#' + id + ' .restart-link').click(function() {
      PluginsView.restart();
    });

    $('#' + id + ' .settings-link').on('click', function() {
      PluginsView.configure(id);
    });
  },


  enable: function(id, enable) {
    var checkbox    = $('#' + id + ' input:checkbox');
    var enabledText = $('#' + id + ' .enabled-text');
    var enableText  = $('#' + id + ' .enable-text');

    if (enable) {
      checkbox.attr('checked', 'checked');
      $('#' + id).addClass('plugin-enabled');
      enabledText.show();
      enableText.hide();
    }
    else {
      checkbox.removeAttr('checked');
      $('#' + id).removeClass('plugin-enabled');
      enabledText.hide();
      enableText.show();
    }

    var state = PluginsView.state(id);
    if (state == 0)
      Plugins.setRestartText(id, '', false);
    else
      Plugins.setRestartText(id, state == 1 ? 'will_be_enabled' : 'will_be_disabled', true);
  },


  setRestartText: function(id, tr, show) {
    var description = $('#' + id + ' .plugin-description');
    var text        = $('#' + id + ' .restart-text');
    var controls    = $('#' + id + ' .restart-controls');

    if (show) {
      description.hide();
      controls.show();

      text.attr('data-tr', tr);
      text.html(Plugins.tr(tr));
    }
    else {
      description.show();
      controls.hide();
    }
  },


  retranslate: function() {
    $('[data-tr]').each(function() {
      Plugins.TR($(this).attr("data-tr"));
    });
  },

  tr: function(key) {
    return SimpleChat.translate(key);
  },

  TR: function(key) {
    $("[data-tr='" + key + "']").html(Plugins.tr(key));
  },

  scrollTo: function(id) {
    document.getElementById(id).scrollIntoView();
  }
};

$(document).ready(function() {
  var plugins = PluginsView.list();
  for (var i = 0; i < plugins.length; i++) {
    Plugins.add(plugins[i]);
  }
});

if (typeof PluginsView === 'undefined') {
  PluginsView = {
    list: function()             { return []; },
    enable: function(id, enable) {},
    state: function(id)          { return 0; },
    restart: function()          {},
    configure: function(id)      {}
  };
}
else {
  SimpleChat.retranslated.connect(Plugins.retranslate);
  PluginsView.scrollTo.connect(Plugins.scrollTo);
}
