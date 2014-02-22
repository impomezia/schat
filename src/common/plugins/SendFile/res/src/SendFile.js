/* $Id: SendFile.js 3678 2013-06-08 02:18:53Z IMPOMEZIA $
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

var SendFileUtils = {
  button: function(name, id)
  {
    return '<a class="btn btn-small btn-file-' + name + '" data-tr="file-' + name + '" href="chat-sendfile:'
       + name + '/' + id + '">' + Utils.tr('file-' + name) + '</a>';
  },

  setStateTr: function(id, text)
  {
    var state = $('#' + id + ' .file-state');
    state.html(Utils.tr(text));
    state.attr('data-tr', text);
    state.removeClass('file-state-inactive');
  },

  setStateText: function(id, text)
  {
    var state = $('#' + id + ' .file-state');
    state.html(text);
    state.removeAttr('data-tr');
    state.removeClass('file-state-inactive');
  },


  progress: function(id, text, percent)
  {
    SendFileUtils.setStateText(id, text);
    $('#' + id + ' .file-progress .bar').css('width', percent + '%');
  },


  /*
   * Установка иконки файла.
   *
   * \param id Идентификатор передачи файла.
   */
  setFileIcon: function(id)
  {
    var imageId = $('#' + id).data('imageId');
    try {
      SendFile.fileIcon(id).assignToHTMLImageElement(document.getElementById(imageId));
      $('#' + imageId).show();
    }
    catch (e) {
      $('#' + imageId).hide();
    }
  },


  /*
   * Обновление информации о состоянии передачи файла.
   *
   * \param id Идентификатор передачи файла.
   */
  updateState: function(id)
  {
    if (document.getElementById(id) === null)
      return;

    var role    = SendFile.role(id);
    var state   = SendFile.state(id);
    var buttons = $('#' + id + ' .file-buttons');

    if (state != 'U') {
      $('#' + id + ' .file-name').removeClass('file-only-name');
      $('#' + id + ' .btn-small').remove();
      $('#' + id + ' .file-progress').hide();
    }

    // Ожидание действия пользователя.
    if (state == 'W') {
      if (role) {
        SendFileUtils.setStateText(id, SimpleChat.bytesToHuman(SendFile.size(id)));
        buttons.append(SendFileUtils.button('saveas', id));
      }
      else
        SendFileUtils.setStateTr(id, 'file-waiting');
    }
    // Обработка отмены передачи файла.
    else if (state == 'c') {
      SendFileUtils.setStateTr(id, 'file-cancelled');
    }
    // Подключение.
    else if (state == 'C') {
      SendFileUtils.setStateTr(id, 'file-connecting');
      $('#' + id + ' .file-name').text(SendFile.fileName(id));
    }
    // Передача файла.
    else if (state == 'T') {
      $('#' + id + ' .file-progress').show();
      var progress = SendFile.progressInfo(id);
      if (progress.hasOwnProperty('text'))
        SendFileUtils.progress(id, progress.text, progress.percent);
    }
    // Завершение передачи файла.
    else if (state == 'F') {
      if (role) {
        var urls = SendFile.fileUrls(id);
        SendFileUtils.setStateText(id, '<span data-tr="file-received">' + Utils.tr('file-received') + '</span> ' +
              '<a href="' + urls.dir + '" data-tr="file-show">' + Utils.tr('file-show') + '</a>');

        buttons.append(SendFileUtils.button('open', id));
        $('#' + id + ' .btn-file-open').attr('href', urls.file);
      }
      else
        SendFileUtils.setStateTr(id, 'file-sent');
    }

    if (state == 'W' || state == 'C' || state == 'T')
      buttons.append(SendFileUtils.button('cancel', id));

    alignChat();
  }
};


/*
 * Добавление сообщения с информацией о входящем или отправленном файле.
 */
Messages.addFileMessage = function(json)
{
  if (document.getElementById(json.Id) !== null)
    return;

  var imageId = SimpleChat.randomId();

  var block = document.createElement('div');
  block.id = json.Id;
  block.setAttribute('class', 'container ' + json.Type + '-type');
  block.setAttribute('data-time', json.Date);

  var html = '<div class="blocks ' + json.Direction + '">';
  html += '<div class="file-sender">' + DateTime.template(json.Date, json.Day) + Messages.nameBlock(json.Author) + '</div>';
  html += '<div class="file-icon"><img id="' + imageId + '" src="" width="16" height="16" alt="" /></div>';
  html += '<div class="file-block"><span class="file-name file-only-name">' + json.File + '</span><br><span class="file-state file-state-inactive">&nbsp;</span></div>';
  html += '<div class="file-buttons btn-group"></div>';
  html += '<div class="file-progress"><div class="bar"></div></div><div style="clear:both;"></div>';
  html += '</div>';

  block.innerHTML = html;

  Messages.addHintedRawMessage(block, json.Hint);
  if (json.WeakId === false)
    ChatView.setLastMessage(json.Date);

  $('#' + json.Id).data('imageId', imageId);
  SendFileUtils.setFileIcon(json.Id);
};


/*
 * Обновление идентификатора и отметки времени исходящей передачи файла.
 */
Messages.upgradeFileMessage = function(json) {
  if (document.getElementById(json.InternalId) === null)
    return;

  if (document.getElementById(json.Id) === null) {
    $('#' + json.InternalId).attr('id', json.Id);
    $('#' + json.Id + ' .btn-file-cancel').attr('href', 'chat-sendfile:cancel/' + json.Id);
  }

  DateTime.update(json);
  ChatView.setLastMessage(json.Date);
};


if (typeof SendFile === 'undefined') {
  SendFile = {
    state: function(id) { return 'U'; },
    role: function(id) { return 1; },
    size: function(id) { return 0; },
    fileName: function(id) { return ''; },
    progressInfo: function(id) { return {}; },
    fileUrls: function(id) { return {}; }
  };
}
else {
  SendFile.progress.connect(SendFileUtils.progress);
  SendFile.stateChanged.connect(SendFileUtils.updateState);
}

Messages.onAdd.push(SendFileUtils.updateState);
