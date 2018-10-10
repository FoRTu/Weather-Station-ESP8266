#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <InfluxDb.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define INFLUXDB_HOST "........"
#define INFLUXDB_PORT "8086"
#define INFLUXDB_DATABASE "........."
#define SSID "......"
#define PASSWORD "......."
#define DHTPIN 2
#define DHTTYPE DHT11

Influxdb influx(INFLUXDB_HOST);
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  // WiFi Connection
  Serial.begin(115200);
  Serial.println();

  WiFi.begin(SSID, PASSWORD);

  Serial.print("Connecting to ");
  Serial.print(ssid);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  // InfluxDB
  influx.setDb(INFLUXDB_DATABASE);

  // Init DHT sensor
  dht.begin();

  Serial.println("Setup done.");
}

void loop() {

  // Leemos la humedad relativa
  float h = dht.readHumidity();
  // Leemos la temperatura en grados centígrados (por defecto)
  float t = dht.readTemperature();
  // Leemos la temperatura en grados Fahrenheit
  float f = dht.readTemperature(true);

  // Comprobamos si ha habido algún error en la lectura
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Error obteniendo los datos del sensor DHT11");
    return;
  }

  // Calcular el índice de calor en Fahrenheit
  float hif = dht.computeHeatIndex(f, h);
  // Calcular el índice de calor en grados centígrados
  float hic = dht.computeHeatIndex(t, h, false);


  InfluxData row("Datos");
  row.addTag("device", "DHT");
  row.addTag("sensor", "one");
  row.addValue("Centigrados", t);
  row.addValue("Fahrenheit", f);
  row.addValue("Humedad", h);
  row.addValue("Indice_de_calor_c", hic);
  row.addValue("Indice_de_calor_f", hif);


  influx.write(row);

  delay(5000);
}
