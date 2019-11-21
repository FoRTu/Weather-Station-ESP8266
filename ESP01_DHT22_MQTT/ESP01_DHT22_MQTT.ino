#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#include <Adafruit_Sensor.h>

#include "config.h"  // Configure file

char Temp[6];
char Hum[6];
float h, t;

WiFiClient espClient;
PubSubClient client(espClient);

DHT dht(DHTPIN, DHTTYPE);

void setup(){
	WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED) 
  { 
    delay(100);
  }
	client.setServer(mqtt_server, 1883);
  dht.begin();
}

void loop(){
	delay(500);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    while (reading_attempts >= 0){
      reading_attempts--;
      return;
    }
    ESP.deepSleep(sleeptime);
  }

  
  dtostrf(t, 5, 2, Temp);
  dtostrf(h, 5, 2, Hum);
  client.connect(mqtt_clientid, mqtt_username, mqtt_password);
  client.publish("MQTT/DEFINE/TOPIC/TEMP", Temp);
  client.publish("MQTT/DEFINE/TOPIC/HUM", Hum);
  delay(100);
  client.disconnect();
  ESP.deepSleep(sleeptime);
}
