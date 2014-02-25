
Messages.addInviteMessage = function(json) {
  console.log(json);

  Messages.addAlertMessage(json);

  $('#' + json.Id + ' .alert-box-buttons').html(
      '<a href="' + json.Url + '" class="btn btn-small btn-invite" data-tr="channels-accept" data-id="' + json.Id + '">' + Utils.tr('channels-accept') + '</a> ' +
      '<a href="#" class="btn btn-small btn-close-invite"          data-tr="channels-reject" data-id="' + json.Id + '">' + Utils.tr('channels-reject') + '</a>'
  )

  $('#' + json.Id + ' .btn-invite').data('channel', json.Channel);
}


$(document).ready(function() {
  $('body').on('click', '.btn-close-invite', function(event) {
    event.preventDefault();
    $('#' + $(this).attr('data-id')).remove();
    alignChat();
  });


  $('body').on('click', '.btn-invite', function(event) {
    var channel = $(this).data('channel');
    $('#' + $(this).attr('data-id')).remove();

    SimpleChat.pinTab(channel);
    alignChat();
  });
});
