var TOKEN = 'YOUR_ACCESS_TOKEN';

var CloudApi = require('mbed-connector-api');
var api = new CloudApi({
  accessKey: process.env.TOKEN || TOKEN
});


// Start notification channel
api.startLongPolling(function(err) {
  if (err) return console.error(err);

  // Find all lights
  api.getEndpoints(function(err, devices) {
    if (err) return console.error(err);
    if (devices.length === 0) return console.error('No lights found...');

    console.log('Found', devices.length, 'lights', devices);
    
    // Only one light for now...
    var endpoint = devices[0].name;
    
    // YOUR CODE HERE



  }, { parameters: { type: 'light-system' } });
});

// Notifications
api.on('notification', function(notification) {
  console.log('Got a notification', notification);
});
