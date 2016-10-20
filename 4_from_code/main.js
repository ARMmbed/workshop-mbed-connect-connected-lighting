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
    api.putResourceSubscription(endpoint, '/pir/0/count', function(err) {
      console.log('Set subscription for pir count', err);
    });
    
    api.putResourceValue(endpoint, '/led/0/permanent_status', 1, function(err) {
      if (err) return console.error('Failed to set status...', err);
      
      console.log('Set status to 1...');
      
      api.putResourceValue(endpoint, '/led/0/color', 0xffa500, function(err) {
        if (err) return console.error('Failed to set color...', err);
        
        console.log('Set color to orange!');
      });
    });



  }, { parameters: { type: 'light-system' } });
});

// Notifications
api.on('notification', function(notification) {
  console.log('Got a notification', notification);
});
