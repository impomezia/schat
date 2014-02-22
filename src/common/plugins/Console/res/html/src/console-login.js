/*
 * Чтение ответа на "get" запрос "console/login".
 */
Console.feed.console.get.login = function(json) {
  if (Console.current != 'login')
    return;

  $('#login-spinner').addClass('hide');
  clearTimeout(Console.login.timeout);

  if (json.status == 200) {
    if ($('#password').val() == 'admin')
      Console.password.load();
    else
      Console.home.load();
  }
  else if (json.status == 402)
    Console.login.setError('console_incorect_password');
  else
    Console.login.setError(json.status);
};


/*
 * Форма ввода пароля для авторизации.
 */
Console.login = {
  timeout: null, // Таймаут отображения спиннера.

  /*
   * Загрузка шаблона страницы.
   */
  load: function() {
    $.ajax({
      url: 'login.html',
      isLocal: true,
      success: Console.login.loaded
    });
  },


  /*
   * Обработка успешной загрузки формы.
   */
  loaded: function(data) {
    $('#page').html(data);

    Console.current = 'login';
    Utils.retranslate();

    var password = $('#password');
    password.focus();

    $('#login').on('click.login', function(event) {
      event.preventDefault();

      if (password.val().length) {
        Console.login.removeError();
        Console.login.timeout = setTimeout(function() {
          $('#login-spinner').removeClass('hide');
        }, 400);

        SimpleChat.get(SimpleChat.serverId(), 'console/login', {password: ConsoleView.toPassword(password.val())});
      }
      else
        Console.login.setError('console_empty_password');
    });
  },


  /*
   * Установка ошибки.
   *
   * \param err числовой код ошибки или строка для динамического перевода.
   */
  setError: function(err) {
    $('#password-group').addClass('error');
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
   * Удаление ошибки.
   */
  removeError: function() {
    $('#password-group').removeClass('error');
    var error = $('#error');
    error.removeAttr('data-tr');
    error.hide();
  }
};
