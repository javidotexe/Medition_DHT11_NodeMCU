#include <ESP8266WiFi.h>
#include "DHT.h"

#define DHTPIN D2 // MODIFICAR EL PIN EN CASO DE SER NECESARIO
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "WIFINETWORK"; //REEMPLAZAR CON LA RED A EMPLEAR
const char* password = "PASSWORD"; //REEMPLAZAR CON LA CONTRASEÑA
const char* server = "192.168.1.80"; // REEMPLAZAR CON LA DIRECCIÓN IP DEL SERVIDOR
const int port = 80; // Assuming HTTP port 80

float humidity, temperature;

void setup() {
  Serial.begin(115200); 

  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to the WiFi network");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  dht.begin();
}

void loop() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  WiFiClient client;

  if (client.connect(server, port)) {
    Serial.println("Connected to server");

    String request = "GET /testcode/dht.php?humidity=";
    request += humidity;
    request += "&temperature=";
    request += temperature;
    request += " HTTP/1.1\r\nHost: ";
    request += server;
    request += "\r\nConnection: close\r\n\r\n";

    client.print(request);
    client.flush();

    while (client.available()) {
     String line = client.readStringUntil('\r');
     Serial.println(line);
    }

    client.stop();
    Serial.println("Connection closed");
  } else {
    Serial.println("Connection to server failed");
  }

  delay(600000); // INTERVALO DE LECTURAS
}
