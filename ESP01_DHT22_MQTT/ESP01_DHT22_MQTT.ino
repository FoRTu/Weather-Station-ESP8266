#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <DHT.h>

#include <Adafruit_Sensor.h>

#include "config.h"  // Variables de configuracion
#include "ESP8266_Utils.hpp"
#include "ESP8266_Utils_OTA.hpp"

WiFiClient espClient;
PubSubClient client(espClient);

#include "MQTT_Utils.hpp"

DHT dht(DHTPIN, DHTTYPE);
float h, t, f;

void setup(){
	Serial.begin(115200);

	ConnectWiFi_STA();

	InitOTA();

  client.setServer(mqtt_server, 1883);

  dht.begin();
}

void loop(){
	ArduinoOTA.handle();
  delay(2500);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
  }

  Serial.println("Humeda %: " + String(h));
  
  Serial.println("Temperatura ºC: " + String(t));
  
  char Temp[6];
  char Hum[6];
  dtostrf(t, 5, 2, Temp);
  dtostrf(h, 5, 2, Hum);
  
  sendMessage("MQTT/TOPIC", Temp);
  sendMessage("MQTT/TOPIC", Hum);
  delay(100);
  ESP.deepSleep(sleeptime);
}
