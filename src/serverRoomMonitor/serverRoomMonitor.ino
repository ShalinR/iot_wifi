#include "DHT.h"

#define DHTPIN        4
#define DHTTYPE       DHT22
#define DOOR_PIN      27
#define LED_COOL      25   // yellow LED – normal state indicator
#define LED_ALERT     33   // red LED – temperature alarm
#define LED_DOOR      32   // yellow LED – door open indicator
#define LED_HUM       19   // blue LED – humidity alarm
#define FAN_PIN       26   // DC fan

#define TEMP_THRESHOLD   29.0   // °C – red alarm
#define HUM_THRESHOLD    60.0   // %  – blue alarm

DHT dht(DHTPIN, DHTTYPE);

unsigned long lastDHT = 0;
unsigned long lastRedBlink = 0;
unsigned long lastBlueBlink = 0;
bool redState = false;
bool blueState = false;

bool tempAlarm = false;
bool humAlarm = false;
int doorState = 0;            // stores latest door reading

const unsigned long BLINK_INTERVAL = 500;  // milliseconds on / off

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n=== LED Alert System ===");

  pinMode(LED_COOL, OUTPUT);
  pinMode(LED_ALERT, OUTPUT);
  pinMode(LED_DOOR, OUTPUT);
  pinMode(LED_HUM, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(DOOR_PIN, INPUT);   // assumes external pull-down or sensor HIGH when open

  digitalWrite(FAN_PIN, LOW);
  digitalWrite(LED_ALERT, LOW);
  digitalWrite(LED_HUM, LOW);
  digitalWrite(LED_COOL, LOW);
  digitalWrite(LED_DOOR, LOW);

  dht.begin();
  Serial.println("Setup complete.");
}

void loop() {
  // --- read sensors every 2 seconds ---
  if (millis() - lastDHT >= 2000) {
    lastDHT = millis();

    float temp = dht.readTemperature();
    float humidity = dht.readHumidity();
    doorState = digitalRead(DOOR_PIN);   // update global door state

    if (isnan(temp) || isnan(humidity)) {
      Serial.println("DHT22 read FAILED - check wiring!");
    } else {
      tempAlarm = (temp >= TEMP_THRESHOLD);
      humAlarm  = (humidity > HUM_THRESHOLD);

      // fan control (based on temperature only)
      digitalWrite(FAN_PIN, tempAlarm ? HIGH : LOW);

      // Serial output (no LED status)
      Serial.printf("Temp: %.1f°C | Humidity: %.0f%% | Door: %d | Fan: %s\n",
                    temp, humidity, doorState,
                    tempAlarm ? "ON" : "OFF");
    }
  }

  // --- non-blocking LED control (runs every loop iteration) ---

  // Red LED (ALERT) blinking when temp alarm active
  if (tempAlarm) {
    if (millis() - lastRedBlink >= BLINK_INTERVAL) {
      lastRedBlink = millis();
      redState = !redState;
      digitalWrite(LED_ALERT, redState);
    }
  } else {
    digitalWrite(LED_ALERT, LOW);
    redState = false;
  }

  // Blue LED (HUM) blinking when humidity alarm active
  if (humAlarm) {
    if (millis() - lastBlueBlink >= BLINK_INTERVAL) {
      lastBlueBlink = millis();
      blueState = !blueState;
      digitalWrite(LED_HUM, blueState);
    }
  } else {
    digitalWrite(LED_HUM, LOW);
    blueState = false;
  }

  // Yellow LED COOL – on only when NO alarm (normal state)
  digitalWrite(LED_COOL, (!tempAlarm && !humAlarm) ? HIGH : LOW);

  // Yellow LED DOOR – indicates door open (HIGH = open, LOW = closed)
  digitalWrite(LED_DOOR, doorState);
}