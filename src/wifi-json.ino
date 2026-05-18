#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Dialog 4G 048";
const char* password = "7c6eeBC1";

const int setup_delay = 1000;
const int loop_delay = 5000;
const int HTTP_port = 80;

WebServer server(HTTP_port);

void handleRoot() {

  int humidity = random(50, 99);
  float temperature = random(250, 350) / 10.0;

  String json = "{";
  json += "\"id\":\"G2\",";
  json += "\"timestamp\":\"2026-05-11 12:55:34\",";
  json += "\"temperature\":" + String(temperature) + ",";
  json += "\"humidity\":" + String(humidity) + ",";
  json += "\"flags\":{";
  json += "\"batteryLow\":true,";
  json += "\"motionDetected\":false,";
  json += "\"relayOn\":true,";
  json += "\"sensorError\":false,";
  json += "\"maintenanceMode\":true";
  json += "}";
  json += "}";

  server.send(200, "application/json", json);
}

void setup() {

  Serial.begin(115200);
  delay(setup_delay);

  Serial.println("Connecting to WiFi...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected successfully!");

  Serial.print("SSID: ");
  Serial.println(ssid);

  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  // Root endpoint
  server.on("/", handleRoot);

  server.begin();

  Serial.println("JSON Web Server Started");
}

void loop() {

  server.handleClient();

  Serial.println("ESP32 is connected to WiFi");

  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  delay(loop_delay);
}