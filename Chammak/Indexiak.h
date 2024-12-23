const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
  <html>
    <head>
      <title>Odczyty DHT11</title>
      <meta http-equiv=\"refresh\" content=\"5\">
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
      <div id=\"weather-forecast\"></div>
      }
  
  
  
    </body>
  </html>
)=====";
