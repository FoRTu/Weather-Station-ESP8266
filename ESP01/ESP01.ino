

#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>

#include "config.h"  // Sustituir con datos de vuestra red
#include "ESP8266_Utils.hpp"
#include "ESP8266_Utils_OTA.hpp"

WiFiClient espClient;
PubSubClient client(espClient);

#include "MQTT_Utils.hpp"

void setup(){
	Serial.begin(115200);

	ConnectWiFi_STA();

	InitOTA();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop(){
	ArduinoOTA.handle();
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
