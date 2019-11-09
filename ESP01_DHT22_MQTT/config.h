const char* ssid     = "XXXXXX";
const char* password = "XXXXXX";
const char* hostname = "ESP01";
const char* ota_password = "XXXXXX";
const double sleeptime = 1800e6; // en microsegundos
const char* mqtt_server = "XXXXXX";
const char* mqtt_username = "XXXXXX";
const char* mqtt_password = "XXXXXX";
const char* mqtt_clientid = "ESP01-ID";

#define DHTPIN 0
#define DHTTYPE DHT22

IPAddress ip(192, 168, 1, 17);
IPAddress gateway(192, 168, 1, 254);
IPAddress subnet(255, 255, 255, 0);
