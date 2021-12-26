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
 * 
 * IMPORTENT NOTE: Remove the RX wire during upload the code to the ESP-12E module. Then, link it back after finished uploading process
 */
#include <ESP8266WiFi.h>
#include "FirebaseESP8266.h"  // Install Firebase ESP8266 library
                              // Link: https://github.com/mobizt/Firebase-ESP8266/tree/v3.7.0
#include <SoftwareSerial.h>

// Insert your network credentials
#define WIFI_SSID "*************"
#define WIFI_PASSWORD "*************"

// Insert Firebase project API Key
#define FIREBASE_AUTH "*************"

// Insert RTDB URLefine the RTDB URL */
#define FIREBASE_HOST "*************" 

//Define FirebaseESP8266 data object
FirebaseData firebaseData;
FirebaseData ledData;

FirebaseJson json;

String value_arduino;
float Humidity, Temperature;

// Function for start wifi connection
void wifi_config()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  //Firebase configureation
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

// Help function to read data from Arduino and push it to the firebase
void get_data()
{
  int i1 = value_arduino.indexOf(',');
  String firstValue = value_arduino.substring(0, i1);
  String secondValue = value_arduino.substring(i1 + 1);

  Temperature = firstValue.toFloat(); 
  Humidity = secondValue.toFloat();
  Serial.print("Temperature = ");
  Serial.print(firstValue);
  Serial.print(" , ");
  Serial.print("Humidity = ");
  Serial.print(secondValue);
  Serial.println();
  Firebase.setFloat(firebaseData, "/arduino_mega/Temperature", Temperature);
  Firebase.setFloat(firebaseData, "/arduino_mega/Humidity", Humidity);
  delay(5);
  value_arduino="";
}

void setup() {
  Serial.begin(115200);
  wifi_config();
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() {

  if(Serial.available()){
    while(Serial.available())
    {
      char ch = Serial.read();
      value_arduino += ch;
    }
    int index = value_arduino.indexOf(';');
    value_arduino = value_arduino.substring(0, index);
    get_data();     
  }
  else
  {
    Serial.print("s1 is not available..");
    Serial.println();
  }
  delay(1000);
}
