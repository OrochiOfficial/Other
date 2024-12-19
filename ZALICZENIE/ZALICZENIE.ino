

#include "DHTesp.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
const char* ssid = "Redmi";
const char* password = "rozowy245";
DHTesp dht;
ESP8266WebServer server(80);

void handleRoot(); // Deklaracja funkcji handleRoot przed jej użyciem

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
  server.on("/", handleRoot);
  server.begin();

  dht.setup(D2, DHTesp::DHT11);
}

void loop() {
  server.handleClient();

  delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print(" %");
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print(" C");
  Serial.print("\t\t");
  Serial.println();
}

void handleRoot() {
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  String webpage = "<html><head><title>Odczyty DHT11</title><meta http-equiv=\"refresh\" content=\"2\"></head><body>";
  webpage += "<h1>Stacja pogodowa</h1>";
  webpage += "<h2> Tomasz Kotyla</h2> ";
  webpage += "<p>Temperatura: " + String(temperature, 1) + " C</p>";
  webpage += "<p>Wilgotnosc: " + String(humidity, 1) + " %</p>";
  webpage += "</body></html>";

  server.send(200, "text/html", webpage);
}

