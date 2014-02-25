(function() {
  'use strict';

  function RoomsNavbar() {
    this.serverName = document.getElementById('server-name');
    this.joinName   = document.getElementById('join-name');
    this.joinBtn    = document.getElementById('join');
    this.createBtn  = document.getElementById('create');
  }

  RoomsNavbar.prototype.setName = function(name) {
    this.serverName.textContent = name;
  };

  window.schat.ui.RoomsNavbar = RoomsNavbar;
})();