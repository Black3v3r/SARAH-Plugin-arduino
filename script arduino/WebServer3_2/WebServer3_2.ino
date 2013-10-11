/*
  Web Server 3.2
 
 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)
 
 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 
set:
http://arduino/set/?lcd=message
get:
http://arduino/get/?sensor=a0

Pour écrire sur les pins A0 => A5, ajouter 14 au n° de la pin.
E.g: http://arduino/set/?a14=255 va écrire 255 sur la pin A0

 */

#include <SPI.h>
#include <Ethernet.h>
#include <RCSwitch.h>
#include <IRremote.h>

RCSwitch mySwitch = RCSwitch();

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,3);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

// Initialize variables
unsigned long time;
int e = 0;
int i;
String ln = String("\r\n");

int RECV_PIN = 7;
IRrecv irrecv(RECV_PIN);
decode_results results;


//Fonction annexes

void parse(String request, String *body, int *err);
void action_get(String request, String *body, int *err);
void action_set(String request, String *body, int *err);


void setup() {
  //Pins
  pinMode(0,OUTPUT);
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  mySwitch.enableReceive(0);  // 433Mhz Rx
  mySwitch.enableTransmit(3); // 433Mhz Tx

  irrecv.enableIRIn(); // Start the IR receiver

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  // Ethernet.begin(mac);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // Serial.println("LOL");
  // delay(10);
  
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    // set variables defaut values
    i = 0;
    String get = String();
    String body = String();
    int err = 200;
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char* errors[][22] = {};
        *errors[200]="OK";
        *errors[400]="Bad request";
        *errors[401]="Unauthorized";
        *errors[404]="Not Found";
        //*errors[500]="Internal Server Error";
        char c = client.read();
        if (i == 0) {
          get += c;
        }
        Serial.write(c);
        /*if you've gotten to the end of the line (received a newline
        character) and the line is blank, the http request has ended,
        so you can send a reply*/
        if (c == '\n' && currentLineIsBlank) {
          // parse the request
          parse(get, &body, &err);
          Serial.println("From parse function");
            Serial.println("Body:");
            Serial.println(body);
            Serial.println("Erreur: ");
            Serial.println(err);
          // send a standard http response header
          if (err == 200) {
            client.println("HTTP/1.1 200 OK");
          } else {
            client.println("HTTP/1.1 400 Bad Request");
          }
          client.println("Content-Type: text/html");
          client.println("Connnection: close");
          client.println();
          client.println(body);
          Serial.print("HTTP/1.1 ");
          Serial.print(err);
          Serial.print(" ");
          Serial.println(*errors[err]);
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
          i++;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
    Serial.println();
  }

  if (mySwitch.available()) {
    int value = mySwitch.getReceivedValue();
    if (value == 0) {
      Serial.print("Unknown encoding");
    } else {
      Serial.print("Received ");
      Serial.print( mySwitch.getReceivedValue() );
      Serial.print(" / ");
      Serial.print( mySwitch.getReceivedBitlength() );
      Serial.print("bit ");
      Serial.print("Protocol: ");
      Serial.println( mySwitch.getReceivedProtocol() );
    }
    mySwitch.resetAvailable();
  }

  if (irrecv.decode(&results)) {
    irrecv.resume(); // Receive the next value
    time = millis();
    Serial.println(millis());
    e = 1;
  }
  if (((millis() - time) > 400) && e) {
    Serial.println(millis() - time);
    Serial.println(results.value, HEX);
        if (client.connect("192.168.1.4", 8080)) {
    Serial.println("connecting...");
    // send the HTTP PUT request:
    client.print("GET /sarah/ir?ref=arduino&code=");
    client.print(results.value, HEX);
    client.print("&type=");
    client.print(results.decode_type);
    client.println(" HTTP/1.1");
    client.println("Host: 192.168.1.4");
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();

  } 
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    Serial.println("disconnecting.");
    client.stop();
  }
  client.stop();
  
    e = 0;
  }

  /*if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
    // if there's a successful connection:
    if (client.connect("192.168.1.4", 8080)) {
    Serial.println("connecting...");
    // send the HTTP PUT request:
    client.print("GET /sarah/ir?ref=arduino&code=");
    client.print(results.value, HEX);
    client.print("&type=");
    client.print(results.decode_type);
    client.println(" HTTP/1.1");
    client.println("Host: 192.168.1.4");
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();

  } 
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    Serial.println("disconnecting.");
    client.stop();
  }
  client.stop();
  }*/
}


void parse(String request, String *body, int *err) {
  request = request.substring(4, request.length() - 11);
  Serial.println(request);

if (request.charAt(1) == 'g') {
  // Get
  action_get(request, &*body, &*err);
}
// Set
else if (request.charAt(1) == 's') {

  action_set(request, &*body, &*err);
  
}
else {
  Serial.println("Erreur de lecture de la requête");
  *err = 400;
}
}


void action_get(String request, String *body, int *err) {
  Serial.println("get");
  char pinChar[request.substring(14).length() + 1];
  request.substring(14).toCharArray(pinChar, sizeof(pinChar));
  int pin = atoi(pinChar);
  // digital read
  if ((request.charAt(13) == 'd') || (request.charAt(13) == 'D')) {
    switch (pin) {
      default:
      *body += (digitalRead(pin)) + ln;
    }
  }
  // analog read
  else if ((request.charAt(13) == 'a') || (request.charAt(13) == 'A')) {
    switch (pin) {
      case 1337:
      *body += map(analogRead(14), 0, 1023, 100, 0) + ln;
      break;
      default:
      *body += analogRead(pin) + ln; 
    }
  }
  else {
    Serial.println("Erreur de lecture de la requête");
    *err = 400;
  }
}


void action_set(String request, String *body, int *err) {
  Serial.println("set");
  char pinChar[request.substring(7, request.indexOf("=")).length() + 1];
  request.substring(7, request.indexOf("=")).toCharArray(pinChar, sizeof(pinChar));
  int pin = atoi(pinChar);
  char valueChar[request.substring(request.indexOf("=") + 1).length() + 1];
  request.substring(request.indexOf("=") + 1).toCharArray(valueChar, sizeof(valueChar));
  int value = atoi(valueChar);
  Serial.print("pin: ");
  Serial.println(pin);
  Serial.print("value: ");
  Serial.println(value);
  if ((request.charAt(6) == 'd') || (request.charAt(6) == 'D')) {
    Serial.println("digital");
    switch (pin) {
      default:
      digitalWrite(pin, value);
    }
  }
  else if ((request.charAt(6) == 'a') || (request.charAt(6) == 'A')) {
    switch (pin) {
      default:
      analogWrite(pin, value);
    }
  }
  else if (request.charAt(6) == 'r') {
    Serial.println("RCSwitch");
    int group = pin / 10;
    int num = pin % 10;

    switch (pin) {
      case 00:
      Serial.println("Hello, World !");
        if (value == 1) {
          Serial.println("On");
          mySwitch.switchOn(4, 4);
        } else {
          Serial.println("Off");
          mySwitch.switchOff(4, 4);
        }
      break;
      /*case 99:
        Serial.println("Store");
        if(value == 1){
          mySwitch.switchOn("10101", "00000");
        } else {
          mySwitch.switchOff("10101", "00000");
        }
      break;*/
      default: 
        Serial.println("default");
        Serial.print("group: ");
        Serial.println(group);
        Serial.print("num: ");
        Serial.println(num);
        switch (value) {
          case 0:
            mySwitch.switchOff(group, num);
            Serial.println("Off");
            break;
          case 1:
            mySwitch.switchOn(group, num);
            Serial.println("On");
            break;
          default: 
            *err = 400;
       }
    }
  }
  else if (request.charAt(6) == 'l') { // Prévu pour l'utilisation d'un écran LCD, mais manque probable de place dans la NAND pour toutes les librairies.
    Serial.print("message: ");
    for (int i = 0; i <= sizeof(valueChar) - 2; ++i) {
      Serial.print(valueChar[i]);
    }
    Serial.println();
    // Ecriture du message
  }
  else {
  Serial.println("Erreur de lecture de la requête");
  *err = 400;
  }
}