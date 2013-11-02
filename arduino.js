exports.action = function(data, callback, config, SARAH) {
  var act = data.act;
  var config = config.modules.arduino;
  var errTts = "Je n'ai pas pu ";
  var errReq = "Il semble y avoir un problème dans la requête, ";
  var e = 0;
  console.log("act: " + act);
  console.log("value: " + data.value);
  console.log("sensor: " + data.sensor);
  console.log("actor: " + data.actor);

  if (!isset(config.IP_arduino)) {
    errTts = errReq + "l'ipé de l'arduino est mal configurée";
    e = 1;
  }

  var adresse = config.IP_arduino + act + '/?';
  if (act == 'get') {
      if (isset(data.sensor)) {
      errTts = errReq + "capteur manquant";
      e = 1;
    } else {
      adresse += 'sensor=' + data.sensor;
      errTts += " récupérer ";
      switch (data.sensor) {
        case "a0":
          errTts += "la température";
          break;
        default:
          errTts += "les informations du capteur";
      }
    }
  } else if (act == 'set') {
    if (!isset(data.actor)) {
      errTts = errReq + "acteur manquant";
      e = 1;
    } else if (!isset(data.value)) {
      errTts = errReq + "valeur manquante";
      e = 1;
    } else {
      adresse += data.actor + '=' + data.value;
      if (data.value == 0) {
        errTts += "éteindre ";
      } else {
        errTts += "allumer ";
      }
      switch (data.actor) {
        case "r44":
          errTts += "la lumière";
          break;
      }
    }
  } else {
  errTts = errReq + "action manquante";
  e = 1;
  }

  if (e == 0) {
    var request = require('request');
  request({ 'uri' : adresse }, function (err, response, body){
    if (err || response.statusCode != 200) {
      callback({'tts': errTts});
      return;
    }
    callback({'tts' : body});
  });
  } else {
    callback({'tts' : errTts});
  }
  
}

function isset (variable) {
  return (typeof variable != 'undefined');
}