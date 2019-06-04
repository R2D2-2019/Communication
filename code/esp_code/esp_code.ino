#include <ESP8266NetBIOS.h>

#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <string>
#include <frame_types.hpp>

WiFiClient client;
SoftwareSerial due(12, 13, false, 256);
const char *SSID = "hotspot44";
const char *pwd = "hotspot44";

void setup() {
  pinMode(12, INPUT);
  pinMode(13, OUTPUT);

  due.begin(115200);
  delay(1000);
  WiFi.begin(SSID, pwd);

  
}

void loop() {
  // put your main code here, to run repeatedly:

}
