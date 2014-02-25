(function() {
  'use strict';

  var create = window.schat.ui.createElement;
  var modal  = document.getElementById('modal');

  var dialog  = create('div', {class:'modal-dialog'});
  var content = dialog.appendChild(create('div', {class:'modal-content'}));

  var header  = content.appendChild(create('div', {class:'modal-header'}));
  header.appendChild(create('button', {class:'close', 'data-dismiss':'modal', 'aria-hidden':'true'}, '&times;'));
  var title = header.appendChild(create('h4', {class:'modal-title'}));

  var body = content.appendChild(create('div', {class:'modal-body'}));
  var footer = content.appendChild(create('div', {class:'modal-footer'}));

  modal.appendChild(dialog);

  schat.ui.modal = {
    element: modal,
    header: header,
    title: title,
    body: body,
    footer: footer,
    current: null,
    dialog: dialog
  };

  $(document).ready(function() {
    $(modal).on('hidden.bs.modal', function() {
      if (schat.ui.modal.current) {
        try {
          schat.ui.modal.current.hidden();
        }
        catch (e) {}
      }

      title.textContent  = '';
      body.textContent   = '';
      footer.textContent = '';
      footer.style.display = 'block';

      schat.ui.modal.current = null;
    })
  });

  $(modal).on('shown.bs.modal', function() {
    if (schat.ui.modal.current) {
      try {
        schat.ui.modal.current.shown();
      }
      catch (e) {}
    }
  })
})();