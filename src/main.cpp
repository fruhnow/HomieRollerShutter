#include <Homie.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 12
#define DHTTYPE 11                        //DHT11
#define INTERVAL 60                       //s

DHT_Unified dht(DHTPIN, DHTTYPE);


const int rollo1pin1 = 14;
const int rollo1pin2 = 2;
const int rollo2pin1 = 4;
const int rollo2pin2 = 13;

HomieNode     rollo1node("rollo1", "switch");
HomieNode     rollo2node("rollo2", "switch");
HomieNode     temperatureNode("temperature", "temperature");
HomieNode     humidityNode("humidity", "humidity");
unsigned long lastSent = 0;

void loopHandler() {
  sensors_event_t event;  
   if (millis() - lastSent >= INTERVAL * 1000UL || lastSent == 0) {
     dht.humidity().getEvent(&event);
     float humidity = event.relative_humidity;
     Homie.getLogger() << "Humidity is " << humidity << endl;
     dht.temperature().getEvent(&event);
     float temperature = event.temperature;
     Homie.getLogger() << "Temperature is " << temperature << endl;
     temperatureNode.setProperty("temperature").send(String(temperature));
     humidityNode.setProperty("humidity").send(String(humidity));
     lastSent = millis();
  }
}

void configurePins() {
  pinMode(rollo1pin1, OUTPUT);
  pinMode(rollo1pin2, OUTPUT);
  pinMode(rollo2pin1, OUTPUT);
  pinMode(rollo2pin2, OUTPUT);
  digitalWrite(rollo1pin1, LOW);
  digitalWrite(rollo1pin2, LOW);
  digitalWrite(rollo2pin1, LOW);
  digitalWrite(rollo2pin2, LOW);

}

bool rollo1OnHandler(const HomieRange& range, const String& value) {
  if(value != "UP" && value != "DOWN" && value != "STOP") {
    Homie.getLogger() << "Rollo #2 received illega arguement." << endl;
    return false;
  }

  if(value == "DOWN"){
    digitalWrite(rollo1pin1, LOW);
    digitalWrite(rollo1pin2, HIGH);
  } else if(value == "UP"){
    digitalWrite(rollo1pin1, LOW);
    digitalWrite(rollo1pin2, LOW);
  } else {
    digitalWrite(rollo1pin1, HIGH);
    digitalWrite(rollo1pin2, HIGH);
  }
  rollo1node.setProperty("on").send(value);
  Homie.getLogger() << "Rollo #1 received " << value << endl;

  return true;
}

bool rollo2OnHandler(const HomieRange& range, const String& value) {
  if(value != "UP" && value != "DOWN" && value != "STOP") {
    Homie.getLogger() << "Rollo #2 received illega arguement." << endl;
    return false;
  }
  if(value == "DOWN"){
    digitalWrite(rollo2pin1, LOW);
    digitalWrite(rollo2pin2, HIGH);
  } else if(value == "UP"){
    digitalWrite(rollo2pin1, LOW);
    digitalWrite(rollo2pin2, LOW);
  } else {
    digitalWrite(rollo2pin1, HIGH);
    digitalWrite(rollo2pin2, HIGH);
  }
  rollo1node.setProperty("on").send(value);
  Homie.getLogger() << "Rollo #2 received " << value << endl;

  return true;
}

void setup() {
  Serial.begin(9600);
  Serial << endl << endl;
  dht.begin();
  configurePins();
  rollo1node.advertise("on").settable(rollo1OnHandler);
  rollo2node.advertise("on").settable(rollo2OnHandler);
  Homie.setLoopFunction(loopHandler);
  temperatureNode.advertise("temperature");
  humidityNode.advertise("humidity");
  Homie_setFirmware("roller-shutter-wintergarten", "1.0.0"); // The underscore is not a typo! See Magic bytes
  Homie.setup();
}

void loop() {
  Homie.loop();
}