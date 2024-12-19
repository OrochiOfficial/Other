#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "DHT.h"
#define DHTTYPE DHT11
#include "PageIndex.h"
#define LEDonBoard 2

const char *ssid = "Redmi";
const char *password = "rozowy245";

ESP8266WebServer server(80);

const int DHTPin = 5;
DHT dht(DHTPin, DHTTYPE);

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
    pinMode(LEDonBoard, OUTPUT);
    digitalWrite(LEDonBoard, HIGH);
    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED) 
    {
        Serial.print(".");
        digitalWrite(LEDonBoard, LOW);
        delay(250);
        digitalWrite(LEDonBoard, HIGH);
        delay(250);
    }
    digitalWrite(LEDonBoard, HIGH);
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
}

void loop(void){
    server.handleClient();
}
