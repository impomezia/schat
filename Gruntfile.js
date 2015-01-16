module.exports = function(grunt) {
  'use strict';

  grunt.initConfig({
    uglify: {
      all: {
        files: {
          'res/html/js/bootstrap-tooltip.min.js': ['res/html/src/bootstrap-tooltip.js'],
          'res/html/js/bootstrap-modal.min.js': ['res/html/src/bootstrap-modal.js'],
          'res/html/js/jquery.timeago.min.js': ['res/html/src/jquery.timeago.js'],
          'res/html/js/jquery.timeago.en.min.js': ['res/html/src/jquery.timeago.en.js'],
          'res/html/js/jquery.timeago.ru.min.js': ['res/html/src/jquery.timeago.ru.js'],
          'src/common/plugins/RawFeeds/res/js/RawFeeds.min.js': ['src/common/plugins/RawFeeds/res/src/KelpJSONView.js', 'src/common/plugins/RawFeeds/res/src/RawFeeds.js'],
          'src/common/plugins/Profile/res/js/Profile.min.js': ['src/common/plugins/Profile/res/src/Profile.js'],
          'src/common/plugins/YouTube/res/js/YouTube.min.js': ['src/common/plugins/YouTube/res/src/YouTube.js'],
          'src/common/plugins/SendFile/res/js/SendFile.min.js': ['src/common/plugins/SendFile/res/src/SendFile.js'],
          'src/common/plugins/Channels/res/js/bootstrap-dropdown.min.js': ['src/common/plugins/Channels/res/src/bootstrap-dropdown.js'],
          'src/common/plugins/Channels/res/js/Channels.min.js': ['src/common/plugins/Channels/res/src/Channels.js'],
          'src/common/plugins/Channels/res/js/ChannelsUser.min.js': ['src/common/plugins/Channels/res/src/ChannelsUser.js'],
          'src/common/plugins/Channels/res/js/ListView.min.js': [
            'res/html/js/jquery.timeago.js',
            'res/html/src/lib/base.js',
            'res/html/src/lib/utils.js',
            'res/html/src/lib/phpjs.js',
            'res/html/src/lang.js',
            'res/html/src/lib/ui-base.js',
            'res/html/src/lib/ui-modal.js',
            'res/html/src/lib/ui-index.js',
            'src/common/plugins/Channels/res/src/rooms-ui-navbar.js',
            'src/common/plugins/Channels/res/src/rooms-base.js'
          ],
          'src/common/plugins/Console/res/js/console.min.js': [
            'src/common/plugins/Console/res/html/src/console.js',
            'src/common/plugins/Console/res/html/src/console-login.js',
            'src/common/plugins/Console/res/html/src/console-password.js',
            'src/common/plugins/Console/res/html/src/console-home.js'
          ]
        }
      }
    },
    cssmin: {
      all: {
        files: {
          'res/html/css/about.min.css': ['res/html/src/about.css'],
          'res/html/css/PluginsView.min.css': ['res/html/src/PluginsView.css'],
          'res/html/css/ChatView.min.css': ['res/html/src/ChatView.css'],
          'res/html/css/server.min.css': ['res/html/src/server.css'],
          'res/html/css/user.min.css': ['res/html/src/user.css'],
          'res/html/css/channel.min.css': ['res/html/src/channel.css'],
          'src/common/plugins/RawFeeds/res/css/RawFeeds.min.css': ['src/common/plugins/RawFeeds/res/src/RawFeeds.css'],
          'src/common/plugins/Profile/res/css/flags.min.css': ['src/common/plugins/Profile/res/src/flags.css'],
          'src/common/plugins/YouTube/res/css/YouTube.min.css': ['src/common/plugins/YouTube/res/src/YouTube.css'],
          'src/common/plugins/SendFile/res/css/SendFile.min.css': ['src/common/plugins/SendFile/res/src/SendFile.css'],
          'src/common/plugins/Channels/res/css/bootstrap-dropdown.min.css': ['src/common/plugins/Channels/res/src/bootstrap-dropdown.css'],
          'src/common/plugins/Channels/res/css/Channels.min.css': ['src/common/plugins/Channels/res/src/Channels.css'],
          'src/common/plugins/Console/res/css/console.min.css': ['src/common/plugins/Console/res/html/src/console.css'],
          'src/common/plugins/Console/res/css/bootstrap.min.css': [
            'src/common/plugins/Console/res/src/bootstrap.css',
            'src/common/plugins/Console/res/src/bootstrap-override.css'
          ]
        }
      }
    },
    less: {
      all: {
        options: {
          compress: true,
          paths: ['res/less/bootstrap', 'res/less']
        },
        files: {
          'src/common/plugins/Channels/res/css/bootstrap.min.css': ['src/common/plugins/Channels/res/src/bootstrap.less'],
          'src/common/plugins/Channels/res/css/ListView.min.css': ['src/common/plugins/Channels/res/src/ListView.less']
        }
      }
    }
  });

  grunt.loadNpmTasks('grunt-contrib-uglify');
  grunt.loadNpmTasks('grunt-contrib-cssmin');
  grunt.loadNpmTasks('grunt-contrib-less');

  grunt.registerTask('default', ['uglify', 'cssmin', 'less']);
};
