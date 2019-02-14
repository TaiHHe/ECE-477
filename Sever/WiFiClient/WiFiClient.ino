/*
    This sketch establishes a TCP connection to a "quote of the day" service.
    It sends a "hello" message, and then prints received data.
*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#ifndef STASSID
#define STASSID "XMIXERMAX"
#define STAPSK  "Team19@ECE477"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "192.168.1.100";
const uint16_t port = 9090;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  // We start by connecting to a WiFi network

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
  Serial.print("connecting to ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);

  //HTTPClient http;    //Declare object of class HTTPClient
  //http.begin(host, port);      //Specify request destination
  //http.PUT("Initialize connection");
  //delay(1000);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  int status = client.connect(host, port);

  if(!status){
    Serial.println("connection failed");
    delay(5000);
    return;
  }
  else{
    Serial.println("connected to server!");
  }

//  This will send a string to the server
//  Serial.println("sending data to server");
//  if (client.connected()) {
//    client.println("hello from ESP8266");
//  }
  

  // wait for data to be available
//  unsigned long timeout = millis();
//  while (client.available() == 0) {
//    if (millis() - timeout > 5000) {
//      Serial.println(">>> Client Timeout !");
//      client.stop();
//      delay(60000);
//      return;
//    }
//  }

  // Read all the lines of the reply from server and print them to Serial
  Serial.println("receiving from remote server");
  // not testing 'client.connected()' since we do not need to send data here
  //String line = client.readStringUntil('\n');
  // Wait for server to send back data. Keep refreshing the connection to sever due to the sever keeps track on new connection.
  //while(line == ""){
    //client.connect(host,port);
  while(client.available() > 0){
    line = client.readStringUntil('\n');
  }
  //Serial.write(line);
  Serial.println(line);
  Serial.println("Done!");
  // Close the connection
  Serial.println();
  Serial.println("closing connection");
  client.stop();

  delay(1000); // execute once every 1s, don't flood remote service
}
