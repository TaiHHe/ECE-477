#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#ifndef STASSID
#define STASSID "XMIXERMAX"
#define STAPSK "Team19@ECE477"
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

//const char* host = "192.168.0.100";
const char *host = "67.209.189.144";
const uint16_t port = 9999;

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT); // Initialize the LED_BUILTIN pin as an output
//  Serial.println();
//  Serial.println();
//  Serial.print("Connecting to ");
//  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    // Serial.print(".");
  }
  // Serial.println("");
  // Serial.println("WiFi connected");
  // Serial.println("IP address: ");
  // Serial.println(WiFi.localIP());
}

void loop()
{
  digitalWrite(LED_BUILTIN, LOW); // Turn the LED on by making the voltage LOW
  Serial.print("1,50.2,60.3,70.4,80.done!");
  delay(1000);
}
