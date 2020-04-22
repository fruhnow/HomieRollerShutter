#include <Homie.h>

const int rollo1pin1 = 14;
const int rollo1pin2 = 2;
const int rollo2pin1 = 4;
const int rollo2pin2 = 13;

HomieNode     rollo1node("rollo1", "Rollo 1", "switch");
HomieNode     rollo2node("rollo2", "Rollo 2", "switch");

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
    Homie.getLogger() << "Rollo #2 received illegal argument." << endl;
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
    Homie.getLogger() << "Rollo #2 received illegal argument." << endl;
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
  configurePins();
  rollo1node.advertise("on").settable(rollo1OnHandler);
  rollo2node.advertise("on").settable(rollo2OnHandler);
  Homie_setFirmware("roller-shutter-wintergarten", "1.0.0"); // The underscore is not a typo! See Magic bytes
  Homie.setup();
}

void loop() {
  Homie.loop();
}