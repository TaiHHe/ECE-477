#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#ifndef STASSID
#define STASSID "TP-LINK_946"
#define STAPSK  "hao970920"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "192.168.0.100";
const uint16_t port = 9090;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);  // Turn the LED on by making the voltage LOW
  Serial.print("Connecting to ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if(client.connect(host, port)){
    Serial.println("Connected to server!");
    Serial.println("Receiving from remote server ...");
    String line = client.readStringUntil('\n');
    if (line != ""){
      Serial.println();
      Serial.print(line);
      Serial.println();
      Serial.println("Done!");
    }
    else{
      Serial.println("No message Received.");
    }    
    // Close the connection
    Serial.println("Closing connection.");
    client.stop();
    delay(5000); // execute once every 5s, don't flood remote service
  }
  else{
    Serial.println("Connection failed.");
    delay(5000);
    return;
  }


}
