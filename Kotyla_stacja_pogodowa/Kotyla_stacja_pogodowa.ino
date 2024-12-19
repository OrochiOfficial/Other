#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "DHT.h"
#include "DHTesp.h"
#define DHTTYPE DHT11
#include "Indexiak.h"
#define LEDonBoard 2

const char *ssid = "Redmi";
const char *password = "rozowy245";

ESP8266WebServer server(80);

const int DHTPin = 5;
DHTesp dht;

const int diodePin = 2; // Diode pin
const float alarm = 60.0; // Temperature threshold (25°C)

void handleRoot(){
    String s = MAIN_page;
    server.send(200, "text/html", s);
}

void handleDHT11Temperature(){
    float t = dht.readTemperature();
    String Temperature_Value = String(t);
    server.send(200, "text/plane", Temperature_Value); //ewentalnie plain XD
    if (isnan(t)) {
        Serial.println("Failed to read temperature from DHT sensor!");
    }
    else {
        Serial.println("DHT11 || Temperature : ");
        Serial.println(t);
        Serial.println(" || ");
    }
}

void handleDHT11Humidity(){
    float h = dht.readHumidity();
    String Humidity_Value = String(h);
    server.send(200, "text/plane", Humidity_Value);
    if (isnan(h)) {
        Serial.println("Failed to read humidity from DHT sensor!");
    }
    else {
        Serial.println("Humidity : ");
        Serial.println(h);
    }
}

void setup(void){
    Serial.begin(115200);
    delay(500);
    dht.begin();
    delay(500);
    WiFi.begin(ssid, password);
    Serial.println("");
    Serial.print("Connecting");
    Serial.println("");
    Serial.print("Succesfully connected to : ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    server.on("/", handleRoot);
    server.on("/readTemperature", handleDHT11Temperature);
    server.on("/readHumidity", handleDHT11Humidity);
    server.begin();
    Serial.println("HTTP server started");

    server.on("/", handleRoot);
  server.begin();

  dht.setup(D2, DHTesp::DHT11);
  pinMode(diodePin, OUTPUT); // Initialize diode pin as output
    
}

void loop(void){
  server.handleClient();

  delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print(" %");
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print(" C");
  Serial.print("\t\t");
  Serial.println();

  // Turn on diode if temperature exceeds threshold
  if (humidity < alarm) {
    digitalWrite(diodePin, HIGH); // Turn on the diode
  } else {
    digitalWrite(diodePin, LOW); // Turn off the diode
  }
}
