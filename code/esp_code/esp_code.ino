#include <ESP8266NetBIOS.h>

#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <string>
#include <algorithm>
//#include <frame_types.hpp>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include <vector>

WiFiClient client;
SoftwareSerial due(12, 13, false, 256);
const char *SSID = "tuskAct2";
const char *pwd = "LRRPEdE5";
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 4, /* clock=*/ 14, /* data=*/ 2);

void setup() {
    pinMode(12, INPUT);
    pinMode(13, OUTPUT);
    Serial.begin(9600);

    u8g2.begin();
    u8g2.setFont(u8g2_font_5x7_mf );
    u8g2.clearBuffer();
    u8g2.drawStr(0,20,"Connecting to network");
    u8g2.sendBuffer();

    due.begin(115200);
    delay(1000);
    WiFi.begin(SSID, pwd);
  
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        u8g2.clearBuffer();
        u8g2.drawStr(0, 20,"Connecting...");
        u8g2.sendBuffer();
    }

    while (client.connect("10.42.0.1", 8080) != true) {
        delay(500);
        u8g2.clearBuffer();
        u8g2.drawStr(0, 20, "again...");
        u8g2.sendBuffer();
    }
    u8g2.clearBuffer();
    u8g2.drawStr(0, 20,"heyo...");
    u8g2.sendBuffer();
    
}

std::vector<uint8_t> incoming_due;
std::vector<uint8_t> incoming_wifi;
char * wifi_out;
char * due_out;

void loop() {
  // put your main code here, to run repeatedly:
    while(WiFi.status() == WL_CONNECTED) {
        while (client.connected()) {
            bool due_message_received = false;
            bool wifi_message_received = false;
            if (due.available()){
                due_message_received = true;
                u8g2.clearBuffer();
                u8g2.drawStr(0, 20, "till here");
                u8g2.sendBuffer();
                while (due.available()){
                    incoming_due.push_back(due.read());
                }
                std::for_each(incoming_due.begin(), incoming_due.end(), [](uint8_t n){*wifi_out = n; wifi_out++;});

            }
            
       /*     if (client.available()){
                wifi_message_received = true;
                while(client.available()){
                    incoming_wifi.push_back(client.read());
                }
                std::for_each(incoming_wifi.begin(), incoming_wifi.end(), [](uint8_t n){*due_out = n; due_out++;});
            }  

            if (due_message_received && client.availableForWrite()){
                client.write(wifi_out);
                client.write("hello");
                u8g2.clearBuffer();
                u8g2.drawStr(0, 20, "till here2");
                u8g2.sendBuffer();
            }
            if (wifi_message_received){
                due.write(due_out);
            }*/
                client.connect(IPAddress(192, 168, 137, 1), 8080);
        }
            WiFi.begin(SSID, pwd);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
}
    }
  
}
