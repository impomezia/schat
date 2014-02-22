/*
 * Чтение ответа на "put" запрос "storage/password".
 */
Console.feed.storage.put.password = function(json) {
  console.log(json);

  $('#password-spinner').addClass('hide');
  clearTimeout(Console.password.timeout);

  if (json.status == 200) {
    Loader.spinner.add('loading/try');
    SimpleChat.get(SimpleChat.serverId(), 'console/try');
  }
  else
    Console.password.setError(json.status);
};


/*
 * Форма изменения пароля.
 */
Console.password = {
  timeout: null, // Таймаут отображения спиннера.

  /*
   * Загрузка шаблона страницы.
   */
  load: function() {
    $.ajax({
      url: 'password.html',
      isLocal: true,
      success: Console.password.loaded
    });
  },


  /*
   * Обработка успешной загрузки формы.
   */
  loaded: function(data) {
    $('#page').html(data);

    Console.current = 'password';
    Utils.retranslate();
    Console.password.adjustWidth();

    var newPassword = $('#new-password');
    newPassword.focus();

    $('#save').on('click.password', function(event) {
      event.preventDefault();

      var error = Console.password.verify();
      if (error !== '') {
        Console.password.setError(error);
        return;
      }

      Console.password.timeout = setTimeout(function() {
        $('#password-spinner').removeClass('hide');
      }, 400);

      SimpleChat.request(SimpleChat.serverId(), 'put', 'storage/password', {'value':ConsoleView.toPassword($('#new-password').val())});
    });

    $('#cancel').on('click.password', function(event) {
      event.preventDefault();

      Loader.spinner.add('loading/try');
      SimpleChat.get(SimpleChat.serverId(), 'console/try');
    });
  },


  /*
   * Проверка введённых данных.
   */
  verify: function() {
    var password = $('#new-password').val();
    if (password !== $('#confirm-new-password').val())
      return 'console_password_mismatch';

    var size = password.length;
    if (size == 0)
      return 'console_empty_password';

    if (size < 8)
      return 'console_password_short';

    return '';
  },


  /*
   * Установка ошибки.
   *
   * \param err числовой код ошибки или строка для динамического перевода.
   */
  setError: function(err) {
    $('#new-password-group').addClass('error');
    $('#confirm-new-password-group').addClass('error');
    var error = $('#error');

    if (typeof err === 'number') {
      error.removeAttr('data-tr');
      error.html('<strong>' + err + '</strong> ' + SimpleChat.statusText(err));
    }
    else {
      error.attr('data-tr', err);
      error.html(Utils.tr(err));
    }

    error.fadeIn('fast');
  },


  /*
   * Подгонка ширины элементов.
   */
  adjustWidth: function() {
    Utils.adjustWidth($('.control-label'));
    Utils.adjustWidth($('button'));
  }
};

if (typeof SimpleChat !== 'undefined')
  SimpleChat.retranslated.connect(Console.password.adjustWidth);
