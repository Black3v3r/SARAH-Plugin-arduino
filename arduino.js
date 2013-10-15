exports.action = function(data, callback, config, SARAH) {
  var act = data.act;
  var config = config.modules.arduino;
  console.log("act: " + act);
  console.log("value: " + data.value);
  console.log("sensor: " + data.sensor);
  console.log("actor: " + data.actor);
  var adresse = config.IP_arduino + act + '/?';
  if (act == 'get') {
  	adresse += 'sensor=' + data.sensor;
  }
  var errTts = "L'action a échoué";
  if (act == 'set') {
  	adresse += data.actor + '=' + data.value;
	if(data.value == '1'){
		errTts = "Je n'ai pas pu allumer la lumière.";
	}else{
		errTts = "Je n'ai pas pu éteindre la lumière.";
	}
  }
  
  console.log("adresse: " + adresse);

  var request = require('request');
  request({ 'uri' : adresse , 'timeout' : 5000}, function (err, response, body){
  //request({ 'uri' : adresse }, function (err, response, body){
  if (err || response.statusCode != 200) {
      callback({'tts': errTts});
      return;
    }

    //callback({'tts' : body});
	callback({});
  });
}
