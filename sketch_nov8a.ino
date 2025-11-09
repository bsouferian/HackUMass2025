#include <WiFi.h>
#include <WebServer.h>

// Wi-Fi AP credentials
const char* ssid = "ESP32-Movement-Controller";
const char* password = "1"; // optional, can be left blank

// LED pins
const int ledPins[4] = {12, 14, 27, 26};
String motorStates[4] = {"off", "off", "off", "off"};
String mvmnt[4] = {"Forward", "Backward", "Turn-CW", "Turn-CCW"};

// Web server on port 80
WebServer server(80);

// Function to toggle a motor
void toggleMotor(int index) {
  if (motorStates[index] == "on") {
    // Turn it off if already on
    motorStates[index] = "off";
    digitalWrite(ledPins[index], LOW); //replace this with turning off the associated motors with mvmnt[i]
  } else {
    // Turn the clicked motor ON and all others OFF
    for (int i = 0; i < 4; i++) {
      if (i != index) {
        motorStates[i] = "off";
        digitalWrite(ledPins[i], LOW); //replace this with turning off the associated motors with mvmnt[i]
      }
    }
    motorStates[index] = "on";
    digitalWrite(ledPins[index], HIGH); //replace this with the function for activating mvmnt[index]
  }
  handleRoot();
}

// Function to display web page
void handleRoot() {
  String html = "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<style>body { font-family: Arial; text-align: center; }";
  html += ".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px; margin: 5px; font-size: 20px; cursor: pointer; }";
  html += ".off { background-color: #555555; }</style></head><body>";
  html += "<h1>ESP32 Movement Controller</h1>";

  for(int i = 0; i < 4; i++) {
    html += "<p>" + mvmnt[i] + " - State: " + motorStates[i] + "</p>";
    if(motorStates[i] == "off") {
      html += "<p><a href=\"/" + mvmnt[i] + "\"><button class=\"button\">ON</button></a></p>";
    } else {
      html += "<p><a href=\"/" + mvmnt[i] + "\"><button class=\"button off\">OFF</button></a></p>";
    }
  }

  html += "</body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);

  // Initialize LED pins
  for(int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  // Start Access Point
  WiFi.softAP(ssid, password);
  Serial.println("Access Point started");
  Serial.print("Connect to: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  // Define routes using toggleMotor
  server.on("/", handleRoot);
  server.on("/Forward", [](){ toggleMotor(0); });
  server.on("/Backward", [](){ toggleMotor(1); });
  server.on("/Turn-CW", [](){ toggleMotor(2); });
  server.on("/Turn-CCW", [](){ toggleMotor(3); });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
