/*
 * Mohammed A. Shehab
 * Concordia University
 * Hardware used in this project are:
 * 1- Name : NodeMCU ESP8266
 *   1.1- link: https://components101.com/development-boards/nodemcu-esp8266-pinout-features-and-datasheet
 *   1.2- Alternative name: ESP-12E module
 *   1.3- CH340 chip drivers: https://sparks.gogo.co.nz/ch340.html
 * 2- Name : DHT11–Temperature and Humidity Sensor
 *    2.1- link: https://components101.com/sensors/dht11-temperature-sensor
 *    2.2- Connection map: https://www.makerguides.com/wp-content/uploads/2019/08/DHT11-3-pin-with-Arduino-UNO-wiring-diagram-schematic-1024x492.jpg
 * 3- Name : Arduino mega 2560
 *    3.1- link: https://store-usa.arduino.cc/products/arduino-mega-2560-rev3?selectedStore=us
 */
#include <DHT.h>
#include <SoftwareSerial.h> //used to communitcate with ESP Wifi

#define DHTPIN 52     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

SoftwareSerial s1(5,6); // Set serial port with pins on the Arduino mega borad.

String buffer;          // Buffer used to send the data of multiple sensors

void setup() {
  Serial.begin(115200);
  s1.begin(115200);     // Start temperature and humidity sensors
  dht.begin();
  delay(2000);
  buffer = "";
}

void loop() {
  
  temp_huma();
  delay(1000);
}

void temp_huma()
{
  
  float h = dht.readHumidity();        // read humidity
  float f = dht.readTemperature();    // read temperature
  if (isnan(h) || isnan(f)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
  buffer = String(f) +"," + String(h)+";";  //Formate the data to be sent to the wifi borad.
  Serial.print(buffer);                     // This print just to check the formate of buffer.
  Serial.println();
  s1.print(buffer);                         // Send the buffer to the wifi borad using serial port
                                            // Be sure that pin# 6 connected to the RX, pin# 5 to TX
                                            // Where pins 6 and 5 on the arduino borad and RX, TX on the (NodeCu ESP8266)
  buffer = "";                              // Finally clean the buffer for next incoming data.
}
