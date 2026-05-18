#include <WiFi.h>
#include <WebServer.h>

const char* = "ESP32-Access-Point-G3";
const char* = "12345678"

WebServer server(80);

// void handleRoot() {
//   String html = "<html><body><h1>Hello World from ESP32 AP!</h1></body></html>";
//   server.send(200, "text/html", html)
            
// }


void handleRoot() {

  int humidity = random(50, 99);
  float temperature = random(250, 350) / 10.0;

  String json = "{";
  json += "\"id\" : \"G1\",";
  json += "\"timestamp\":\"2026-05-11 12:55:34\",";
  json += "\"temperature\":28.5,";
  json += "\"humidity\":65,";
  json += "\"flags\":{,";
  json += "\"batteryLow\":true,";
  json += "\"motionDetected\":false,";
  json += "\"relayOn\":true,";
  json += "\"sensorError\":false,";
  json += "\"maintenanceMode\"true:,";
  json += "}";
  json += "}";

  srver.send(200, "aplication/json", json);
  
}
//json += "\"\:,";

void setup() {
  // put your setup code here, to run once:
  serial.begin(115200);

  // Set ESP32 to Access Point mode
  WiFi.softAP(ssid, password);

  Serial.println("Access Point Started");
  Serial.println("IP Address: ");
  Serial.println(WiFi.softAPIP()); //  usually 192.168.4.1

  // Define route for the rootpage
  server.on("/", handleRoot)

  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

  server.handleClient(); // Listen for incoming requests
}
