#include <SPISlave.h>
#include <hspi_slave.h>

#include <ESP8266WiFi.h>

 
char* ssid = "tuskAct2";
char* password =  "LRRPEdE5";
 
uint16_t port = 8080;
const char * host = "10.42.0.1";
WiFiClient client;

void setup() {
 
    Serial.begin(115200);
    Serial.print("hello");

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.println("...");
        delay(100);
    }
 
    Serial.print("WiFi connected with IP: ");
    Serial.println(WiFi.localIP());

    while (!client.connect(host, port)) {
        Serial.println("...");
        delay(100);
    }

    SPISlave.onData([](uint8_t * data, size_t len) {
        String message = String((char *)data);
        (void) len;
        if (message.equals("HS")) {
            SPISlave.setData("Hello Master!");
        } else if (message.equals("Are you alive?")) {
            char answer[33];
            sprintf(answer, "Alive for %lu seconds!", millis() / 1000);
            SPISlave.setData(answer);
        } else if (message.equals("Are you connected?"){
            if (
        } else {
            SPISlave.setData("Say what?");
        }
        Serial.printf("Question: %s\n", (char *)data);
    });

    SPISlave.onDataSent([]() {
        Serial.println("Answer Sent");
    });

    SPISlave.onStatus([](uint32_t data) {
        Serial.printf("Status: %u\n", data);
        SPISlave.setStatus(millis()); //set next status
    });

    SPISlave.onStatusSent([]() {
        Serial.println("Status Sent");
    });

    SPISlave.begin();

    SPISlave.setStatus(millis());

    SPISlave.setData("Ask me a question!");
 
}
 
void loop()
{
 
  /*  if (!client.connect(host, port)) {
 
        Serial.println("Connection to host failed");
 
        delay(1000);
        return;
    }
 
    Serial.println("Connected to server successful!");
 
    client.print("Hello from ESP!");
 
    Serial.println("Disconnecting...");
    client.stop();
 
    delay(10000);*/
}
