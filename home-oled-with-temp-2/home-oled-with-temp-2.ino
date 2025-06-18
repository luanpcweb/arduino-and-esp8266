#include "ESP8266WiFi.h"
#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "env.h"

#include "DHT.h"

#define DHTPIN 2

#define DHTTYPE DHT11

extern const char* SSID;
extern const char* PASSWORD;
extern const char* URL_SERVICE;

WiFiServer server(80); 

DHT dht(DHTPIN, DHTTYPE);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  // Connect to WiFi
  WiFi.begin(SSID, PASSWORD);
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

String data;
String dataTemp;

void loop() {

  readTemp();
  delay(10000); // mileseconds
  
  getpr24h();
  delay(10000);

}

void getpr24h() {
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();
  HTTPClient https;

  if (https.begin(*client, "https://blockchain.info/q/24hrprice")) {  // HTTPS
    Serial.println("[HTTPS] GET...");
    int httpCode = https.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
      // file found at server?
      if (httpCode == HTTP_CODE_OK) {
        String payload = https.getString();
        data = payload;
        Serial.println(String("[HTTPS] Received payload: ") + payload);
        Serial.println(String("1BTC = ") + payload + "USD");
        
        display.clearDisplay();
        display.setTextSize(1.9);
        display.setTextColor(WHITE);
        display.setCursor(0, 5);
        display.println("BTC -> USD ");
      
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0, 18);
        display.println(data);
      
        display.display(); 

      }
    } else {
      Serial.printf("[HTTPS] GET... failed, error: %s\n\r", https.errorToString(httpCode).c_str());
    }

    https.end();
  } else {
    Serial.printf("[HTTPS] Unable to connect\n\r");
  }
}


void readTemp() {
   // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    dataTemp = " --- Error in temp --- ";
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  dataTemp = "H: " + String(h) + " | Temp: " + String(t) + " °C" + " | HIC: " +  String(hic) + " °C";  

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

  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 5);
  display.println("T: " + String(hic) + " C");

  display.setTextSize(1.9);
  display.setCursor(0, 25);
  display.println("H: " + String(h));

  display.display(); 

  sendToServ(hif, hic, h, t);
  
}

void fillDisplay(void) {

  display.clearDisplay();
  for (int j = 0; j < 63; j++) {
    for (int i = 0; i < 127; i++) {
      display.drawPixel(i, j, WHITE);
    }
    display.display();
    delay(2);
  }
} 

void printStartData(String data)
{
  //delay(2000);
  display.clearDisplay();

  display.setTextSize(1.9);
  display.setTextColor(WHITE);
  display.setCursor(0, 5);
  display.println(data);

  display.display(); 
}

void sendToServ(float hif, float hic, float h, float t)
{
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();
  HTTPClient https;

    if (https.begin(*client, URL_SERVICE + "?hif=" + hif + "&hic=" + hic + "&h=" + h + "&t=" + t)) {  // HTTPS
    Serial.println("[HTTP] GET...");
    int httpCode = https.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      // file found at server?
      if (httpCode == HTTP_CODE_OK) {
        String payload = https.getString();
        Serial.println(String("[HTTPS] Received payload: ") + payload);

      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n\r", https.errorToString(httpCode).c_str());
    }

    https.end();
  } else {
    Serial.printf("[HTTP] Unable to connect\n\r");
  }
}
