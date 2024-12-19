const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" rel="stylesheet">
    <style>
      html {
        font-family: Arial;
        display: inline-block;
        margin: 0px auto;
        text-align: center;
      }
      h1 { font-size: 2.0rem; }
      p { font-size: 2.0rem; }
      .units { font-size: 1.2rem; }
      .dht-labels{
        font-size: 1.5rem;
        vertical-align:middle;
        padding-bottom: 15px;
      }
      .weather-forecast{
                display: flex;
                align-items: center;
                justify-content: center;
            }
            .day-forecast {
            margin: 10px;
            padding: 10px;
            border: 1px solid #ccc;
            border-radius: 8px;
            width: 200px;
            text-align: center;
            }
    </style>
  </head>
  <body>
  <script>
      setInterval(function() {
        // Call a function repetatively with 2 Second interval
        getTemperatureData();
        getHumidityData();
      }, 2000); 
      
      setInterval(function() {
        // Call a function repetatively with 1 Second interval
        Time_Date();
      }, 1000); 

      function getTemperatureData() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("TemperatureValue").innerHTML =
            this.responseText;
          }
        };
        xhttp.open("GET", "readTemperature", true);
        xhttp.send();
      }

      function getHumidityData() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
          document.getElementById("HumidityValue").innerHTML =
          this.responseText;
          }
        };
        xhttp.open("GET", "readHumidity", true);
        xhttp.send();
      }
      function Time_Date() {
        var t = new Date();
        document.getElementById("time").innerHTML = t.toLocaleTimeString();
        var d = new Date();
        const dayNames = ["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday","Saturday"];
        const monthNames = ["January", "February", "March", "April", "May", "June","July", "August", "September", "October", "November", "December"];
        document.getElementById("date").innerHTML = dayNames[d.getDay()] + ", " + d.getDate() + "-" + monthNames[d.getMonth()] + "-" + d.getFullYear();
      }

      const apiKey = '07f70f5c584e848bef42cb94acea7641';
         const city = 'Warsaw';  // Zmień na nazwę swojego miasta
         const apiUrl = `https://api.openweathermap.org/data/2.5/forecast?q=${city}&appid=${apiKey}&units=metric&lang=pl`;
         
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
            <img src="${iconUrl}" alt="${description}">
        `;
        forecastContainer.appendChild(forecastElement);
    });
}
    </script>
    <h1>NodeMCU ESP8266 Monitoring Sensor DHT11</h1>
    <p>
      <i class="fa fa-thermometer-half" style="font-size:3.0rem;color:#62a1d3;"></i> 
      <span class="dht-labels">Temperaturka : </span> 
      <span id="TemperatureValue">0</span>
      <sup class="units">&deg;C</sup>
    </p>
    <p>
      <i class="fa fa-tint" style="font-size:3.0rem;color:#75e095;"></i>
      <span class="dht-labels">Wilgotnosc : </span>
      <span id="HumidityValue">0</span>
      <sup class="units">%</sup>
    </p>
    <p>
      <i class="far fa-clock" style="font-size:1.0rem;color:#e3a8c7;"></i>
      <span style="font-size:1.0rem;">Time </span>
      <span id="time" style="font-size:1.0rem;"></span>
      
      <i class="far fa-calendar-alt" style="font-size:1.0rem;color:#f7dc68";></i>
      <span style="font-size:1.0rem;">Date </span>
      <span id="date" style="font-size:1.0rem;"></span>
    </P>
  <p>
    <i class="fab fa-youtube" style="font-size:1.0rem;color:red;"></i>
    <span style="font-size:1.0rem;">Don't Forget to Subscribe </span>
    <a href="https://www.youtube.com/channel/UCk8rZ8lhAH4H-75tQ7Ljc1A" target="_blank" style="font-size:1.0rem;">Uteh Str YouTube Channel</a>
  </P>

     <div class="weather-forecast" id="weather-forecast"></div>
    
  </body>
</html>
)=====";
