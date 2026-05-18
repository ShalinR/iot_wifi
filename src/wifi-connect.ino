#include <WiFi.h> 

const char* ssid = "Dialog 4G 048";
const char* password = "7c6eeBC1";
const int setup_delay = 1000;
const int loop-delay = 5000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(setup_delay);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");

  }

  Serial.println();
  Serial.println("WiFi connected successfully!");
  Serial.print("SSID: ");
  Serial.println(ssid);

  Serial.print("ESP32 IP Address: " );
  Serial.println(WiFi.loacalIP());
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("ESP32 is connected to WiFi");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.loacalIP());
  delay(loop_delay);
}
