#include "ESP8266WiFi.h"

#include "DHT.h"

#define DHTPIN 2

#define DHTTYPE DHT11

const char* ssid = "V##"; //Enter SSID
const char* password = "###"; //Enter Password

WiFiServer server(80); 

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
     delay(500);
     Serial.print("*");
  }

  server.begin();
  Serial.println("Servidor inicializado");
  
  Serial.println("");
  Serial.println("WiFi connection Successful");
  Serial.print("The IP Address of ESP8266 Module is: ");
  Serial.print(WiFi.localIP());// Print the IP address

  dht.begin();

}

void loop() {

  WiFiClient client = server.available(); 
  if ( ! client) {
    return;
  }

  // Quando estiver alguem acessando 
  Serial.println("New Client"); 
  
  // Enquanto nao tiver cliente
  while ( ! client.available()) { 
    delay(1);
  }

  // Lê caracteres do buffer serial
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();


  if (req.indexOf("WATHER-AAX") != -1) {
    
        // Reading temperature or humidity takes about 250 milliseconds!
        // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
        float h = dht.readHumidity();
        // Read temperature as Celsius (the default)
        float t = dht.readTemperature();
        // Read temperature as Fahrenheit (isFahrenheit = true)
        float f = dht.readTemperature(true);
      
        // Check if any reads failed and exit early (to try again).
        if (isnan(h) || isnan(t) || isnan(f)) {
          Serial.println(F("Failed to read from DHT sensor!"));
          
          client.print("HTTP/1.1 500 Internal Server Error\r\n");
          client.print("Access-Control-Allow-Origin: *");
          client.print("\r\n\r\n");
          client.print("INTERNAL-SERVER-ERROR");
          
          return;
        }
      
        // Compute heat index in Fahrenheit (the default)
        float hif = dht.computeHeatIndex(f, h);
        // Compute heat index in Celsius (isFahreheit = false)
        float hic = dht.computeHeatIndex(t, h, false);
      
        Serial.print(F("Humidity: "));
        Serial.print(h);
        Serial.print(F("%  Temperature: "));
        Serial.print(t);
        Serial.print(F("°C "));
        Serial.print(f);
        Serial.print(F("°F  Heat index: "));
        Serial.print(hic);
        Serial.print(F("°C "));
        Serial.print(hif);
        Serial.println(F("°F"));

        client.print("HTTP/1.1 200 OK\r\n");
        client.print("Access-Control-Allow-Origin: *");
        client.print("\r\n\r\n");
        client.print("H:");
        client.print(h);
        client.print("|");
        client.print("TEMPC:");
        client.print(t);
        client.print("|");
        client.print("HIC:");
        client.print(hic);

        return;
  } 

  Serial.println("Cliente desconectado");

  client.print("HTTP/1.1 404 Not Found\r\n");
  client.print("Access-Control-Allow-Origin: *");
  client.print("\r\n\r\n");
  client.print("HTTP-NOTFOUND");

}
