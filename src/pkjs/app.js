var Clay = require('pebble-clay');
var clayConfig = require('./config');
//var clay = new Clay(clayConfig);
var clay = new Clay(clayConfig, null, { autoHandleEvents: false });

Pebble.addEventListener('ready', function(e) {
    var timezone_offset = new Date().getTimezoneOffset();  // time-zone offset from UTC, in minutes, for the current locale.
    var options = {"timezone": timezone_offset};

    console.log('storing options: ' + JSON.stringify(options));
    Pebble.sendAppMessage(options,
        function(e) {
            console.log('successfully sent options to pebble');
        },
        function(e) {
            console.log('failed to send options to pebble. Error: ' + e.error.message);
        }
    );

});

//*
// as autoHandleEvents set to false need a basic showConfigurationa ction
Pebble.addEventListener('showConfiguration', function(e) {
    Pebble.openURL(clay.generateUrl());
});

// want to override timeout to be integer value (not string)
// Clay does not support ints - https://github.com/pebble/clay/issues/9
Pebble.addEventListener('webviewclosed', function(e) {
    if (e && !e.response) {
    return;
    }

    // Get the keys and values from each config item
    var dict = clay.getSettings(e.response);
    //dict.TIME_OUT_PERIOD = parseInt(dict.TIME_OUT_PERIOD)

    // Send settings values to watch side
    Pebble.sendAppMessage(dict, function(e) {
        console.log('Sent config data to Pebble');
    }, function(e) {
        console.log('Failed to send config data!');
        //console.log(JSON.stringify(e));
    });
});
//*/
