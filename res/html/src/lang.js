(function() {
  'use strict';

  function numpf(n, f, s, t) {
    // f - 1, 21, 31, ...
    // s - 2-4, 22-24, 32-34 ...
    // t - 5-20, 25-30, ...
    var n10 = n % 10;
    if ( (n10 == 1) && ( (n == 1) || (n > 20) ) ) {
      return f;
    } else if ( (n10 > 1) && (n10 < 5) && ( (n > 20) || (n < 10) ) ) {
      return s;
    } else {
      return t;
    }
  }

  var timeago = {
    en: {
      prefixAgo: null,
      prefixFromNow: null,
      suffixAgo: "ago",
      suffixFromNow: "from now",
      seconds: "less than a minute",
      minute: "about a minute",
      minutes: "<b>%d</b> minutes",
      hour: "about an hour",
      hours: "about <b>%d</b> hours",
      day: "a day",
      days: "<b>%d</b> days",
      month: "about a month",
      months: "<b>%d</b> months",
      year: "about a year",
      years: "<b>%d</b> years",
      wordSeparator: " ",
      numbers: []
    },
    ru: {
      prefixAgo: null,
      prefixFromNow: "через",
      suffixAgo: "назад",
      suffixFromNow: null,
      seconds: "меньше минуты",
      minute: "минуту",
      minutes: function(value) { return numpf(value, "<b>%d<b/> минуту", "<b>%d</b> минуты", "<b>%d</b> минут"); },
      hour: "час",
      hours: function(value) { return numpf(value, "<b>%d</b> час", "<b>%d</b> часа", "<b>%d</b> часов"); },
      day: "день",
      days: function(value) { return numpf(value, "<b>%d</b> день", "<b>%d</b> дня", "<b>%d</b> дней"); },
      month: "месяц",
      months: function(value) { return numpf(value, "<b>%d</b> месяц", "<b>%d</b> месяца", "<b>%d</b> месяцев"); },
      year: "год",
      years: function(value) { return numpf(value, "<b>%d</b> год", "<b>%d</b> года", "<b>%d</b> лет"); }
    }
  };

  function tr(key, param) {
    if (key === 'timeago') {
      var lang = SimpleChat.translate('lang');
      return timeago[timeago.hasOwnProperty(lang) ? lang : 'en'];
    }

    return SimpleChat.translate(key, param);
  }

  function TR(key) {
    $('[data-tr="' + key + '"]').html(tr(key));
  }

  function retranslate() {
    $('[data-tr]').each(function() {
      TR($(this).attr('data-tr'));
    });
  }

  if (typeof SimpleChat !== 'undefined') {
    SimpleChat.retranslated.connect(function() { retranslate(); });
  }

  window.tr              = tr;
  window.TR              = TR;
  schat.lang.retranslate = retranslate;

  // backward compatibility.
  window.Utils = window.Utils || {};
  window.Utils.tr          = tr;
  window.Utils.TR          = TR;
  window.Utils.retranslate = retranslate;
})();