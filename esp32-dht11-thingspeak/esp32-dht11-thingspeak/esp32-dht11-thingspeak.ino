#include <WiFi.h>
#include "ThingSpeak.h"
#include "DHT.h"

// WiFi Credentials
const char* ssid = "milad";
const char* password = "123456789";

// ThingSpeak info
const char* apiKey = "";   // From your ThingSpeak channel
unsigned long channelId = ;   

// DHT sensor setup
#define DHTPIN 4
#define DHTTYPE DHT11 // or DHT22
DHT dht(DHTPIN, DHTTYPE);

// WiFi client
WiFiClient client;

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nConnected!");
  Serial.println(WiFi.localIP());

  ThingSpeak.begin(client);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(5000);
    return;
  }

  Serial.print("🌡️ Temp: "); Serial.print(t);
  Serial.print(" °C  |  💧 Humidity: "); Serial.print(h); Serial.println(" %");

  ThingSpeak.setField(1, t); // Temperature
  ThingSpeak.setField(2, h); // Humidity

  int status = ThingSpeak.writeFields(channelId, apiKey);

  if (status == 200) {
    Serial.println(" Data sent to ThingSpeak!");
  } else {
    Serial.print(" Error: "); Serial.println(status);
  }

  delay(15000); // ThingSpeak only allows updates every 15 sec
}
