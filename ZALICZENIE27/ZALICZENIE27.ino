#include "DHTesp.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <fstream>
#include "tesuto.h"

const char* ssid = "Redmi";
const char* password = "rozowy245";
DHTesp dht;
ESP8266WebServer server(80);



const int diodePin = 2; // Diode pin
const float alarm = 60.0; // Temperature threshold (25°C)

void handleRoot(); // Deklaracja funkcji handleRoot przed jej użyciem

std::ofstream outfile("dane.txt");

//Tutaj były nieudane próby zapisu
//
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


   String webpage = " <html><head><title>Odczyty DHT11</title><meta http-equiv=\"refresh\" content=\"2\"> ";
   webpage += "<style>.weather-forecast{display: flex;align-items: center;justify-content: center;} ";
   webpage += ".day-forecast {margin: 10px;padding: 10px;border: 1px solid #ccc;border-radius: 8px;width: 200px;text-align: center;}</style></head> ";
   webpage += "<body> ";
   webpage += "    <script> ";
   webpage += "    window.onload = function() { ";
   webpage += "        const apiKey = '07f70f5c584e848bef42cb94acea7641'; ";
   webpage += "        const city = 'Warsaw';  // Zmień na nazwę swojego miasta ";
   webpage += "        const apiUrl = `https://api.openweathermap.org/data/2.5/forecast?q=$${city}&appid=$${apiKey}&units=metric&lang=pl`; ";
   webpage += "         ";
   webpage += "        async function getWeatherForecast() { ";
   webpage += "            try { ";
   webpage += "                const response = await fetch(apiUrl); ";
   webpage += "                const data = await response.json(); ";
   webpage += "                displayWeatherForecast(data); ";
   webpage += "            } catch (error) { ";
   webpage += "                console.error('Błąd podczas pobierania danych:', error); ";
   webpage += "            } ";
   webpage += "        } ";
   webpage += "         ";
   webpage += "        function displayWeatherForecast(data) { ";
   webpage += "            const forecastContainer = document.getElementsByTagName(\"content\"); ";
   webpage += "            forecastContainer.innerHTML = '';  // Wyczyść poprzednie prognozy ";
   webpage += "         ";
   webpage += "            const forecastList = data.list.filter((_, index) => index % 8 === 0);  // Pobierz dane co 24 godziny (co 8 razy po 3 godziny) ";
   webpage += "         ";
   webpage += "            forecastList.forEach(forecast => { ";
   webpage += "                const date = new Date(forecast.dt * 1000); ";
   webpage += "                const day = date.toLocaleDateString('pl-PL', { weekday: 'long', day: 'numeric', month: 'long' }); ";
   webpage += "                const temperature = forecast.main.temp.toFixed(1); ";
   webpage += "                const description = forecast.weather[0].description; ";
   webpage += "                const iconUrl = `http://openweathermap.org/img/wn/${forecast.weather[0].icon}.png`; ";
   webpage += "         ";
   webpage += "                const forecastElement = document.createElement('div'); ";
   webpage += "                forecastElement.classList.add('day-forecast'); ";
   webpage += "                forecastElement.innerHTML = ` ";
   webpage += "                    <h3>${day}</h3> ";
   webpage += "                    <p>${temperature}°C</p> ";
   webpage += "                    <p>${description}</p> ";
   webpage += "                `; ";
   webpage += "                forecastContainer.appendChild(forecastElement); ";
   webpage += "            }); ";
   webpage += "        } ";
   webpage += "         ";
   webpage += "        getWeatherForecast(); ";
   webpage += "    </script> ";
   webpage += " ";
   webpage += "    <h1>Stacja pogodowa</h1> ";
   webpage += "    <h2> Tomasz Kotyla</h2>  ";
   webpage += "    <p>Temperatura: " + String(temperature, 1) + " C</p> ";
   webpage += "    <p>Wilgotnosc: " + String(humidity, 1) + " %</p> ";
   webpage += "    <content></content> ";
   webpage += "    <footer></footer>";
   webpage += "</body> ";
   webpage += "</html> ";



  server.send(200, "text/html", webpage);

  
char buffer[strlen_P(MAIN_page) + 1];
    // Skopiuj zawartość z PROGMEM do bufora
    strcpy_P(buffer, MAIN_page);
    // Wyślij zawartość jako odpowiedź HTML
    server.send(200, "text/html", buffer);
}

