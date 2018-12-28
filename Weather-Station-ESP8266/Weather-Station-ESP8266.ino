#include <ESP8266WiFi.h>
#include <InfluxDb.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <WiFiClientSecure.h>

#define INFLUXDB_HOST "192.168.X.X"
#define INFLUXDB_PORT "8086"
#define INFLUXDB_DATABASE "TEMP_DATABASE"
#define SSID "My-WiFi"
#define PASSWORD "XXXXX"
#define HOSTNAME "ESP-01-DHT11"
#define DHTPIN 2
#define DHTTYPE DHT11

// Tiempo de DeepSleep del ESP8266 en microsegundos
uint32_t SLEEP_TIME = 6e7;

Influxdb influx(INFLUXDB_HOST);
DHT dht(DHTPIN, DHTTYPE);

WiFiClientSecure client;

void setup()
{
  // WiFi Connection
  Serial.begin(115200);
  Serial.println();

  WiFi.begin(SSID, PASSWORD);
  WiFi.hostname(HOSTNAME);

  Serial.print("Conectando ");
  Serial.print(SSID);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Conectado, IP: ");
  Serial.println(WiFi.localIP());

  // InfluxDB
  influx.setDb(INFLUXDB_DATABASE);

  // Sensor DHT
  dht.begin();

  Serial.println("Setup finalizado.");
}

void loop() {

  // Leemos la humedad relativa
  float h = dht.readHumidity();
  Serial.println("Humeda %: " + String(h));
  // Leemos la temperatura en grados centígrados (por defecto)
  float t = dht.readTemperature();
  Serial.println("Temperatura ºC: " + String(t));
  // Leemos la temperatura en grados Fahrenheit
  float f = dht.readTemperature(true);
  Serial.println("Temperatura F: " + String(f));
  // Comprobamos si ha habido algún error en la lectura
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Error obteniendo los datos del sensor DHT11");
    return;
  }

  // Calcular el índice de calor en Fahrenheit
  float hif = dht.computeHeatIndex(f, h);
  // Calcular el índice de calor en grados centígrados
  float hic = dht.computeHeatIndex(t, h, false);

  // Preparar los datos para InfluxDB
  InfluxData row("Datos");
  row.addTag("device", "DHT");
  row.addTag("sensor", "TEST");
  row.addValue("Centigrados", t);
  row.addValue("Fahrenheit", f);
  row.addValue("Humedad", h);
  row.addValue("Indice_de_calor_c", hic);
  row.addValue("Indice_de_calor_f", hif);

  // Guardar los datos en InfluxDB
  influx.write(row);
  
  //Porner el dispositivo en DeepSleep
  ESP.deepSleep(SLEEP_TIME, WAKE_RF_DEFAULT);
}
