/*
Name:		arduino_client.ino
Created:	2/4/2018 2:57:08 AM
Author:	lione
*/

// the setup function runs once when you press reset or power the board
//#include <SoftwareSerial.h>
//#include <AltSoftSerial.h>
#include <NeoSWSerial.h>
#include <avr/wdt.h>
//#include <Wire.h>
#include <SPI.h>
#include <EEPROM.h>
#include "DHT.h"
//#include "Adafruit_BMP085.h"
#include "WifiPass.h"

#define DHTTYPE DHT11   // DHT 11
#define DHTPIN 2
DHT dht(DHTPIN, DHTTYPE);
//Adafruit_BMP085 bmp;

//SoftwareSerial ESP8266(10, 11);
NeoSWSerial ESP8266(10, 11);

const String HOST = "192.168.1.200";
const String HOST_PORT = "3000";

const int DELAY = 20; // in seconds
const int RETRY = 4;

const int PIN_BRIGHTNESS = 0;
const int PIN_RAIN = 1;
const int PIN_AIRQUALITY = 2;

int step = 0;

void setup()
{
  //Wire.write(0);
  //Wire.endTransmission();
  //digitalWrite(SDA, 0);
  //digitalWrite(SCL, 0);

  Serial.begin(9600);

  Serial.println("====== START ======");

  step = EEPROM.read(0);

  //I2C_ClearBus();
  //digitalWrite(A4, LOW);
  //digitalWrite(A5, LOW);

  if (step == 0) {
    ESP8266.begin(115200);
  }
  else {
    init_dht();
    init_bmp();
  }

  read_sensor();
}

void loop()
{
  if (step == 0) {
    init_ESP8266();

    int temperature = EEPROM.read(1);
    int humidity = EEPROM.read(2);
    int pressure = EEPROM.read(3);
    int airQuality = get_air_quality();
    int rain = get_rain();
    int brightness = get_brightness();

    //Serial.print("humidity : ");
    //Serial.println(humidity);

    String content = "{";
    content += "\"stationId\": 1,";
    content += "\"data\": ";
    content += "{";
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
    EEPROM.write(0, 1);
    software_Reboot();
  }
}

void read_sensor() {
  if (step != 0) {
    Serial.println("Get humidity, temperature and pressure");

    int temperature = get_temperature();
    EEPROM.write(1, temperature);

    int humidity = get_humidity();
    EEPROM.write(2, humidity);

    int pressure = get_pressure();
    EEPROM.write(3, pressure);

    EEPROM.write(0, 0);
    software_Reboot();
  }
}

void post(const String uri, const String contentType, const String content)
{
  send("AT+CIFSR", 1000);
  send("AT+CIPSTART=4,\"TCP\",\"" + HOST + "\"," + HOST_PORT, 3000);

  String request = "POST " + uri + " HTTP/1.1\r\n";
  request += "Host: " + HOST + ":" + HOST_PORT + "\r\n";
  request += "Content-Type: " + contentType + "\r\n";
  request += "Content-Length: " + String(content.length()) + "\r\n";
  request += "\r\n";
  request += content;

  send("AT+CIPSEND=4," + String(request.length()), 5000);
  send(request, 4000);
  send("AT+CIPCLOSE=5", 1000);
}

void get(const String uri) {
  send("AT+CIPSTART=4,\"TCP\",\"" + HOST + "\"," + HOST_PORT, 3000);
  String request = "GET " + uri + " HTTP/1.1\r\n";
  request += "Host: " + HOST + ":" + HOST_PORT + "\r\n\r\n";
  send("AT+CIPSEND=4," + String(request.length()), 4000);
  send(request, 4000);
  send("AT+CIPCLOSE=4", 4000);
}

void init_ESP8266()
{
  // https://www.itead.cc/wiki/ESP8266_Serial_WIFI_Module#AT_Commands
  // https://www.fais-le-toi-meme.fr/fr/electronique/materiel/esp8266-arduino-wifi-2-euros
  Serial.println("======= START INIT =======");
  send("AT+RST", 2000);
  send("AT+CIOBAUD=9600", 2000);
  ESP8266.begin(9600);
  send("AT+CWMODE=1", 2000);
  send("ATE0", 2000);
  send("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"", 5000);
  send("AT+CIFSR", 3000);
  send("AT+CIPMUX=1", 2000);
  Serial.println("======= END INIT =======");
}

void send(String command, const int timeout)
{
  String response = "";
  int i = 0;
  do {
    if (i == 0) {
      delay(1000);
    }
    //Serial.println("SEND " + String(i) + " l " + command);
    ESP8266.println(command);
    response = receive(timeout);
    delay(2000);
    i++;
    if (i >= RETRY) {
      software_Reboot();
    }
  } while (response_failed(response));
}

String receive(const int timeout)
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
  return response;
}

bool response_failed(String response) {
  response.toLowerCase();
  return contains(response, "busy") ||
    contains(response, "error") ||
    contains(response, "fail") ||
    contains(response, "bad request") ||
    contains(response, "unlink");
}

bool contains(const String str1, const String str2) {
  return str1.indexOf(str2) > 0;
}

void software_Reboot() {
  wdt_enable(WDTO_15MS);
  while (1) {}
}

void init_dht() {
  dht.begin();
}

void init_bmp() {
  //bmp.begin();
}

int get_temperature() {
  //return (float)bmp.readTemperature();
  return 0;
}

int get_humidity() {
  return dht.readHumidity();
}

int get_pressure() {
  //return (int)bmp.readPressure();
  return 0;
}

int get_air_quality() {
  return to_percent(analogRead(PIN_AIRQUALITY));
}

int get_rain() {
  return to_percent(analogRead(PIN_RAIN));
}

int get_brightness() {
  return to_percent(analogRead(PIN_BRIGHTNESS));
}

int to_percent(int value) {
  return map(value, 0, 1023, 0, 100);
}
