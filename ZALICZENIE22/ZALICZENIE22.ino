#include "DHTesp.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <fstream>

const char* ssid = "Redmi";
const char* password = "rozowy245";
DHTesp dht;
ESP8266WebServer server(80);



const int diodePin = 2; // Diode pin
const float alarm = 60.0; // Temperature threshold (25°C)

void handleRoot(); // Deklaracja funkcji handleRoot przed jej użyciem

std::ofstream outfile("dane.txt");

//void saveDataToFile() {
//    while (true) {
//        float humidity = dht.getHumidity();
//        float temperature = dht.getTemperature();
//        Serial.println("Saving data to file...");
//        outfile << humidity << "," << temperature << std::endl;
//        outfile.flush();
//        delay(2000); // Wait for 2 seconds
//    }
//}
//
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
  pinMode(diodePin, OUTPUT); // Initialize diode pin as output
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

  // Turn on diode if temperature exceeds threshold
  if (humidity < alarm) {
    digitalWrite(diodePin, HIGH); // Turn on the diode
  } else {
    digitalWrite(diodePin, LOW); // Turn off the diode
  }
   outfile.flush();
}

void handleRoot() {
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

const char MAIN_page[] PROGEM = R"=====( 
  <html><head><title>Odczyty DHT11</title><meta http-equiv=\"refresh\" content=\"5\">
   </head>
   <body>
       <script>
       window.onload = function() {
           const apiKey = '07f70f5c584e848bef42cb94acea7641';
           const city = 'Warsaw';  // Zmień na nazwę swojego miasta
           const apiUrl = `https://api.openweathermap.org/data/2.5/forecast?q=$${city}&appid=$${apiKey}&units=metric&lang=pl`;
           
           async function getWeatherForecast() {
               try {
                   const response = await fetch(apiUrl);
                   const data = await response.json();
                   displayWeatherForecast(data);
               } catch (error) {
                   console.error('Błąd podczas pobierania danych:', error);
               }
           }
           
           function displayWeatherForecast(data) {
               const forecastContainer = document.getElementById('weather-forecast');
               forecastContainer.innerHTML = '';  // Wyczyść poprzednie prognozy
           
               const forecastList = data.list.filter((_, index) => index % 8 === 0);  // Pobierz dane co 24 godziny (co 8 razy po 3 godziny)
           
               forecastList.forEach(forecast => {
                   const date = new Date(forecast.dt * 1000);
                   const day = date.toLocaleDateString('pl-PL', { weekday: 'long', day: 'numeric', month: 'long' });
                   const temperature = forecast.main.temp.toFixed(1);
                   const description = forecast.weather[0].description;
                   const iconUrl = `http://openweathermap.org/img/wn/${forecast.weather[0].icon}.png`;
           
                   const forecastElement = document.createElement('div');
                   forecastElement.classList.add('day-forecast');
                   forecastElement.innerHTML = `
                       <h3>${day}</h3>
                       <p>${temperature}°C</p>
                       <p>${description}</p>
                   `;
                   forecastContainer.appendChild(forecastElement);
               });
           }
           
           getWeatherForecast();
       </script>
   
       <h1>Stacja pogodowa</h1>
       <h2> Tomasz Kotyla</h2> 
       <p>Temperatura: " + String(temperature, 1) + " C</p>
       <p>Wilgotnosc: " + String(humidity, 1) + " %</p>
       <div id="weather-forecast"></div>
       
   
   
   
   </body>
   </html>
)=====";

  server.send(200, "text/html", MAIN_page);
}

