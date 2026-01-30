#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

#define TRIG_PIN 5  // D1 on NodeMCU (GPIO5)
#define ECHO_PIN 4  // D2 on NodeMCU (GPIO4)

const char* ssid = "Harsh";
const char* password = "harsh@123";
unsigned long channelID = 3156530;
const char* writeAPIKey = "D7BQ6OAFIT04R36S";

WiFiClient client;

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(15000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  ThingSpeak.begin(client);
}

void loop() {
  long duration;
  float distance;

  // Trigger the ultrasonic sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read the echo
  duration = pulseIn(ECHO_PIN, HIGH);            // fixed function name
  distance = (duration * 0.034) / 2.0;           // fixed multiplication and float

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Send data to ThingSpeak
  int httpCode = ThingSpeak.writeField(channelID, 1, distance, writeAPIKey);
  if (httpCode == 200) {
    Serial.println("Data uploaded successfully");
  } else {
    Serial.print("Failed to upload data. HTTP code: ");
    Serial.println(httpCode);
  }

  delay(15000); // Update ThingSpeak every 15 seconds
}