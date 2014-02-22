
var Auth = {
  tr: {
    en: {
      generic_error: '<b>An unexpected error occurred.</b>',
      success: '<b>You have successfully logged into a Simple Chat.</b>'
    },
    ru: {
      generic_error: '<b>Произошла непредвиденная ошибка.</b>',
      success: '<b>Вы успешно вошли в Simple Chat.</b>'
    }
  },

  id: '',
  lang: 'en',

  setError: function()
  {
    $('#spinner').hide();
    $('#alert').addClass('alert-error');
    $('#alert').show();
    $('#alert').html(Auth.tr[Auth.lang].generic_error);
  },

  setSuccess: function(data)
  {
    $('#spinner').hide();
    $('#alert').addClass('alert-success');
    $('#alert').show();
    $('#alert').html(Auth.tr[Auth.lang].success);

    $.JSONView(data, $('#raw'))
  }
};


$(document).ready(function() {
  Auth.id = $('body').attr('id');
  var lang = $('html').attr('lang');
  if (lang == 'ru')
    Auth.lang = lang;

  $.ajax({
    url: '/state/' + Auth.id,
    dataType: 'json',
    success: function(data) {
      Auth.setSuccess(data);
    },
    error: function() {
      Auth.setError();
    }
  });
});

$.extend(jQuery,
  {
    JSONView:function (json, container) {
      var ob;
      if (typeof json == 'string')
        ob = JSON.parse(json);
      else
        ob = json;
      var p, l = [], c = container;
      var repeat = function (s, n) {
        return new Array(n + 1).join(s);
      };

      var r = function (o, isar, s) {
        for (var n in o) {
          var p = o[n];
          switch (typeof p) {
            case 'function':
              break;
            case 'string':
              if (isar)
                l.push({ Text:'<span class="jsonstring">"' + htmlspecialchars(p) + '"</span><span class="jsontag">,</span>', Step:s });
              else
                l.push({ Text:'<span class="jsonname">"' + htmlspecialchars(n) + '"</span><span class="jsontag">: </span><span class="jsonstring">"' + htmlspecialchars(p) + '"</span><span class="jsontag">,</span>', Step:s });
              break;
            case 'boolean':
              if (isar)
                l.push({ Text:'<span class="jsonboolean">"' + p + '"</span><span class="jsontag">,</span>', Step:s });
              else
                l.push({ Text:'<span class="jsonname">"' + htmlspecialchars(n) + '"</span><span class="jsontag">: </span><span class="jsonboolean">' + p + '</span><span class="jsontag">,</span>', Step:s });
              break;
            case 'number':
              if (isar)
                l.push({ Text:'<span class="jsonnumber">' + p + '</span><span class="jsontag">,</span>', Step:s });
              else
                l.push({ Text:'<span class="jsonname">"' + htmlspecialchars(n) + '"</span><span class="jsontag">: </span><span class="jsonnumber">' + p + '</span><span class="jsontag">,</span>', Step:s });
              break;
            case 'object':
              if (p === null) {
                if (isar)
                  l.push({ Text:'<span class="jsonnull">' + p + '</span><span class="jsontag">,</span>', Step:s });
                else
                  l.push({ Text:'<span class="jsonname">"' + htmlspecialchars(n) + '"</span><span class="jsontag">: </span><span class="jsonnull">' + p + '</span><span class="jsontag">,</span>', Step:s });
              }
              else if (p.length == undefined) {
                //object
                if (!isar) {
                  l.push({ Text:'<span class="jsonname">"' + htmlspecialchars(n) + '"</span><span class="jsontag">:</span>', Step:s });
                }
                l.push({ Text:'<span class="jsontag">{</span>', Step:s });
                r(p, false, s + 1);
                l.push({ Text:'<span class="jsontag">},</span>', Step:s });
              }
              else {
                //array
                if (!isar) {
                  l.push({ Text:'<span class="jsonname">"' + htmlspecialchars(n) + '"</span><span class="jsontag">:</span>', Step:s });
                }
                l.push({ Text:'<span class="jsontag">[</span>', Step:s });
                r(p, true, s + 1);
                l.push({ Text:'<span class="jsontag">],</span>', Step:s });
              }
              break;
            default:
              break;
          }
        }
        var last = l.pop();
        var ct = ',</span>';
        if (last.Text.substr(last.Text.length - ct.length) == ct)
          l.push({ Text:last.Text.replace(ct, '</span>'), Step:last.Step });
        else
          l.push(last);
      };

      if (ob.length == undefined) {
        //object
        l.push({ Text:'<span class="jsontag">{</span>', Step:0 });
        r(ob, false, 1);
        l.push({ Text:'<span class="jsontag">}</span>', Step:0 });
      }
      else {
        //array
        l.push({ Text:'<span class="jsontag">[</span>', Step:0 });
        r(ob, true, 1);
        l.push({ Text:'<span class="jsontag">]</span>', Step:0 });
      }

      var f = true;
      c.addClass('KelpJSONView');
      c.append('<ol></ol>');
      c = c.find('ol');
      for (var index in l) {
        var jobject = l[index];
        if (f) {
          c.append($('<li class="jsonhighlight">' + repeat('&nbsp; ', jobject.Step) + jobject.Text + '</li>'));
          f = false;
        }
        else {
          c.append($('<li>' + repeat('&nbsp; ', jobject.Step) + jobject.Text + '</li>'));
          f = true;
        }
      }
    }
  });

function htmlspecialchars (string, quote_style, charset, double_encode) {
  // http://kevin.vanzonneveld.net
  // +   original by: Mirek Slugen
  // +   improved by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
  // +   bugfixed by: Nathan
  // +   bugfixed by: Arno
  // +    revised by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
  // +    bugfixed by: Brett Zamir (http://brett-zamir.me)
  // +      input by: Ratheous
  // +      input by: Mailfaker (http://www.weedem.fr/)
  // +      reimplemented by: Brett Zamir (http://brett-zamir.me)
  // +      input by: felix
  // +    bugfixed by: Brett Zamir (http://brett-zamir.me)
  // %        note 1: charset argument not supported
  // *     example 1: htmlspecialchars("<a href='test'>Test</a>", 'ENT_QUOTES');
  // *     returns 1: '&lt;a href=&#039;test&#039;&gt;Test&lt;/a&gt;'
  // *     example 2: htmlspecialchars("ab\"c'd", ['ENT_NOQUOTES', 'ENT_QUOTES']);
  // *     returns 2: 'ab"c&#039;d'
  // *     example 3: htmlspecialchars("my "&entity;" is still here", null, null, false);
  // *     returns 3: 'my &quot;&entity;&quot; is still here'
  var optTemp = 0,
    i = 0,
    noquotes = false;
  if (typeof quote_style === 'undefined' || quote_style === null) {
    quote_style = 2;
  }

  if (typeof string !== 'string')
    return "";

  string = string.toString();
  if (double_encode !== false) { // Put this first to avoid double-encoding
    string = string.replace(/&/g, '&amp;');
  }
  string = string.replace(/</g, '&lt;').replace(/>/g, '&gt;');

  var OPTS = {
    'ENT_NOQUOTES': 0,
    'ENT_HTML_QUOTE_SINGLE': 1,
    'ENT_HTML_QUOTE_DOUBLE': 2,
    'ENT_COMPAT': 2,
    'ENT_QUOTES': 3,
    'ENT_IGNORE': 4
  };
  if (quote_style === 0) {
    noquotes = true;
  }
  if (typeof quote_style !== 'number') { // Allow for a single string or an array of string flags
    quote_style = [].concat(quote_style);
    for (i = 0; i < quote_style.length; i++) {
      // Resolve string input to bitwise e.g. 'ENT_IGNORE' becomes 4
      if (OPTS[quote_style[i]] === 0) {
        noquotes = true;
      }
      else if (OPTS[quote_style[i]]) {
        optTemp = optTemp | OPTS[quote_style[i]];
      }
    }
    quote_style = optTemp;
  }
  if (quote_style & OPTS.ENT_HTML_QUOTE_SINGLE) {
    string = string.replace(/'/g, '&#039;');
  }
  if (!noquotes) {
    string = string.replace(/"/g, '&quot;');
  }

  return string;
}