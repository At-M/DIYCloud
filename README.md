![IoTCloud image](https://i.imgur.com/IzPhMG8.png)

# IoTCloud
A network connected cloud, that visualises the current weather and shows the temperature, based on an esp8266 (WeMos D1 Mini)


Currently working:

- Animation of random rain/snowfall

ToDo:
- connect to weather service and get weather data from current location
- change leds in regards to received weatherdata
- animation should only last a few seconds to save power
- add battery instead of being constantly powered

(Additional planned features)
- include BH1750 to set brightness of leds regarding to ambient light
- include BME280 to get ambient temperature (because why not)
- include SSD1306 OLED to show temperature in °C
- include sensor to only wake up if tapped or something?
- DEEPSLEEP