#include "ESP8266WiFi.h"
#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_AHTX0.h>



const char* ssid = "##"; //Enter SSID
const char* password = "##"; //Enter Password

WiFiServer server(80); 


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


Adafruit_AHTX0 aht;

#define BUZZER_PIN D4

#define MQ9_ANALOG A0

int limiarGas = 600; 

void setup() {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

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

  if (!aht.begin()) {
    while (1) {
      Serial.println("Erro ao iniciar o sensor AHT10");
      delay(1000);
    }
  }

  Serial.println("AHT10 e OLED iniciados com sucesso!");

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(MQ9_ANALOG, INPUT);
}

String data;
String dataTemp;

void loop() {

  readTemp();
  delay(10000);


}

void readGas(){

    int gasValue = analogRead(MQ9_ANALOG);
    Serial.print("Gas Value: ");
    Serial.println(gasValue);

    display.clearDisplay();
    
    if (gasValue > limiarGas) {
      Serial.println("Detected gas!");
      digitalWrite(BUZZER_PIN, HIGH);

      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 5);
      display.println("GAS DETECTED");
    } else {

      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 5);
      display.println("GAS LEVEL: " + String(gasValue) + " - Limiar: " + String(limiarGas));
      
      digitalWrite(BUZZER_PIN, LOW);
    }

}


void readTemp() {

  sensors_event_t hum, tem;
  aht.getEvent(&hum, &tem);

  Serial.print(F("Humidity: "));
  Serial.print(hum.relative_humidity);
  Serial.print(F("%  Temperature: "));
  Serial.print(tem.temperature);
  Serial.print(F("°C "));


  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 5);
  display.println("T: " + String(tem.temperature) + " C");

  display.setTextSize(1.9);
  display.setCursor(0, 25);
  display.println("H: " + String(hum.relative_humidity));

  display.display(); 

  
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
