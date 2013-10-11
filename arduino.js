exports.action = function(data, callback, config, SARAH) {
  var act = data.act;
  // var config = config.modules.skype;
  console.log("act: " + act);
  console.log("value: " + data.value);
  console.log("sensor: " + data.sensor);
  console.log("actor: " + data.actor);
  var adresse = 'http://192.168.1.3/' + act + '/?';
  if (act == 'get') {
  	adresse += 'sensor=' + data.sensor;
  }
  if (act == 'set') {
  	adresse += data.actor + '=' + data.value;
  }
  
  console.log("adresse: " + adresse);

  var request = require('request');
  request({ 'uri' : adresse }, function (err, response, body){
    if (err || response.statusCode != 200) {
      callback({'tts': "L'action a échoué"});
      return;
    }
    callback({'tts' : body});
  });
}