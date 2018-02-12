/*
 Name:		arduino_client.ino
 Created:	2/4/2018 2:57:08 AM
 Author:	lione
*/

// the setup function runs once when you press reset or power the board
#include <SoftwareSerial.h>
#include "DHT.h"   // Librairie des capteurs DHT
#include "WifiPass.h"

#define DHTTYPE DHT11   // DHT 11
#define DHTPIN 2

SoftwareSerial ESP8266(10, 11);

const String HOST = "weather-station.azurewebsites.net"; // 192.168.1.91 || http://weather-station.azurewebsites.net/
const String HOST_PORT = "80";

const int DELAY = 20; // in seconds

void setup()
{
  Serial.begin(9600);
  ESP8266.begin(115200);
  initESP8266();
  receive(2000);
}

void loop()
{
  initESP8266();
  receive(2000);

  int temperature = random(0, 50);
  int humidity = random(0, 100);
  int pressure = random(0, 50);
  int airQuality = random(0, 255);
  int rain = random(0, 255);
  int brightness = random(0, 255);

  String content = "{";
  content += "\"stationId\": 0,";
  content += "\"data\": ";
  content += "{";
  content += "\"Temperature\": " + String(temperature) + ",";
  content += "\"Temperature\": " + String(temperature) + ",";
  content += "\"Humidity\": " + String(humidity) + ",";
  content += "\"Pressure\": " + String(pressure) + ",";
  content += "\"AirQuality\": " + String(airQuality) + ",";
  content += "\"Rain\": " + String(rain) + ",";
  content += "\"Brightness\": " + String(brightness);
  content += "}";
  content += "}";

  post("/api/weatherdata", "application/json", content); // application/x-www-form-urlencoded || application/json

  delay(DELAY * 1000);
}

void post(const String uri, const String contentType, const String content)
{
  send("AT+CIPSTART=4,\"TCP\",\"" + HOST + "\"," + HOST_PORT);
  receive(1000);

  String request = "POST " + uri + " HTTP/1.1\r\n";
  request += "Host: " + HOST + ":" + HOST_PORT + "\r\n";
  request += "Content-Type: " + contentType + "\r\n";
  request += "Content-Length: " + String(content.length()) + "\r\n";
  request += "\r\n";
  request += content;

  send("AT+CIPSEND=4," + String(request.length()));
  receive(2000);

  send(request);
  receive(2000);

  send("AT+CIPCLOSE=4");
  receive(1000);
}

void get(const String uri) {
  send("AT+CIPSTART=4,\"TCP\",\"" + HOST + "\"," + HOST_PORT);
  receive(1000);

  String request = "GET " + uri + " HTTP/1.1\r\n";
  request += "Host: " + HOST + ":" + HOST_PORT + "\r\n\r\n";

  send("AT+CIPSEND=4," + String(request.length()));
  receive(2000);

  send(request);
  receive(2000);

  send("AT+CIPCLOSE=4");
  receive(1000);
}

void initESP8266()
{
  // https://www.itead.cc/wiki/ESP8266_Serial_WIFI_Module#AT_Commands
  // https://www.fais-le-toi-meme.fr/fr/electronique/materiel/esp8266-arduino-wifi-2-euros
  send("AT+RST");
  receive(2000);
  send("AT+CIOBAUD=9600");
  receive(4000);
  ESP8266.begin(9600);
  send("AT+CWMODE=1");
  receive(5000);
  send("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"");
  receive(10000);
  send("AT+CIFSR");
  receive(1000);
  send("AT+CIPMUX=1");
  receive(1000);
}

void send(String command)
{
  ESP8266.println(command);
}

void receive(const int timeout)
{
  String response = "";
  long int time = millis();
  while ((time + timeout) > millis())
  {
    while (ESP8266.available())
    {
      char c = ESP8266.read();
      response += c;
    }
  }
  Serial.print(response);
}
