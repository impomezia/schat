var Page = {
  addProvider: function(name, data) {
    $('#providers').append('<div class="provider">' +
                             '<a href="' + data.url + ' "><i class="provider-' + name + '"></i> ' + data.htmlName + '</a>' +
                           '</div>');
  }
};

$(document).ready(function() {
  $.ajax({
    url: '/providers',
    dataType: 'json',
    success: function(data) {
      $('#providers-header .icon-spinner').remove();
      for (var i = 0; i < data.order.length; i++) {
        var provider = data.order[i];
        if (data.providers.hasOwnProperty(provider))
          Page.addProvider(provider, data.providers[provider]);
      }
    },
    error: function() {
      $('#providers-header .icon-spinner').remove();
    }
  });
});