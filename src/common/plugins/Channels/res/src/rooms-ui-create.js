(function() {
  'use strict';

  var title  = schat.ui.modal.title;
  var footer = schat.ui.modal.footer;
  var body   = schat.ui.modal.body;
  var create = window.schat.ui.createElement;

  function RoomCreateDialog() {
    title.textContent = tr('channels-create-title');
    title.setAttribute('data-tr', 'channels-create-title');

    body.innerHTML =
        '<div class="form-horizontal">' +
          '<div id="name-group" class="form-group">' +
            '<label data-tr="channels-name-label" for="create-name" class="col-sm-3 control-label">' + tr('channels-name-label') + '</label>' +
            '<div class="col-sm-9">' +
              '<input type="text" class="form-control" id="create-name" maxlength="64" autofocus="autofocus">' +
            '<div id="name-alert" class="alert alert-danger"></div>' +
            '</div>' +
          '</div>' +
          '<div class="form-group">' +
            '<div class="col-sm-offset-3 col-sm-9">' +
              '<div class="checkbox">' +
                '<label><input id="access-checkbox" type="checkbox"> <span data-tr="channels-access-checkbox">' + tr('channels-access-checkbox') + '</span></label>' +
              '</div>' +

            '</div>' +
          '</div>' +
        '</div>';


    this.createBtn   = footer.appendChild(create('button', {type:'button', class:'btn btn-primary', 'data-tr':'channels-create'}));
    footer.appendChild(create('button', {type:'button', class:'btn btn-default', 'data-dismiss':'modal', 'data-tr':'channels-cancel'}));

    this.nameEdit    = document.getElementById('create-name');
    this.aclCheckbox = document.getElementById('access-checkbox');
    this.nameGroup   = document.getElementById('name-group');
    this.nameAlert   = document.getElementById('name-alert');

    var that = this;

    $(this.nameEdit).on('input', function() {
      that.nameGroup.setAttribute('class', 'form-group');
      that.nameAlert.style.display = 'none';
      that.nameAlert.textContent = '';
    });

    $(this.nameEdit).keydown(function(event){
      if(event.keyCode == 13)
        that.createBtn.click();
    });
  }


  RoomCreateDialog.prototype.setError = function(error) {
    this.nameGroup.setAttribute('class', 'form-group has-error');
    this.nameAlert.style.display = 'block';
    this.nameAlert.textContent = tr(error);
    this.nameEdit.focus();
  };


  RoomCreateDialog.prototype.name = function() {
    return this.nameEdit.value.replace(/\s{2,}/g, ' ').trim();
  };


  window.schat.ui.RoomCreateDialog = RoomCreateDialog;
})();