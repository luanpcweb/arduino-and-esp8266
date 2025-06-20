#include "ESP8266WiFi.h"
#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_AHTX0.h>
#include "env.h"

extern const char* ssid;
extern const char* password;

WiFiServer server(80); 

#define BUZZER_PIN 14
#define MQ9_ANALOG A0

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_AHTX0 aht;

int limiarGas = 500; 

unsigned long lastSensorTime = 0;
const unsigned long sensorInterval = 2000; // Atualiza a cada 2s

float temperature = 0.0;
float humidity = 0.0;
int gasValue = 0;

void setup() {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Falha ao iniciar o display SSD1306"));
    while(true);
  }

  fillDisplay();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("*");
  }

  server.begin();
  Serial.println("Servidor inicializado");
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());

  if (!aht.begin()) {
    Serial.println("Erro ao iniciar o sensor AHT10");
    while (1) {
      delay(1000);
    }
  }

  Serial.println("AHT10 e OLED iniciados com sucesso!");

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  pinMode(MQ9_ANALOG, INPUT);

  ESP.wdtEnable(10000); // watchdog de 10s
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastSensorTime >= sensorInterval) {
    lastSensorTime = currentMillis;
    updateSensors();
    updateDisplay();
  }

  ESP.wdtFeed();
}

void updateSensors() {
  // Temperatura e umidade
  sensors_event_t hum, tem;
  aht.getEvent(&hum, &tem);
  temperature = tem.temperature;
  humidity = hum.relative_humidity;

  // Gás
  gasValue = analogRead(MQ9_ANALOG);

  // Buzzer
  if (gasValue > limiarGas) {
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  Serial.print("Temp: "); Serial.print(temperature);
  Serial.print(" | Hum: "); Serial.print(humidity);
  Serial.print(" | Gas: "); Serial.println(gasValue);

  // sendDataToServer();
}

void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  // Linha 1: Temperatura e Umidade
  display.setCursor(0, 0);
  display.print("T:");
  display.print(temperature, 1);
  display.print("C");

  display.setCursor(64, 0);
  display.print("H:");
  display.print(humidity, 0);
  display.print("%");

  // Linha 2: Gás
  display.setCursor(0, 12);
  display.print("Gas: ");
  display.print(gasValue);
  display.print(" / ");
  display.print(limiarGas);

  // Gráfico de barra horizontal
  int barWidth = map(gasValue, 220, limiarGas, 0, 90);
  barWidth = constrain(barWidth, 0, 90);
  display.drawRect(30, 24, 90, 6, WHITE); // moldura
  display.fillRect(31, 25, barWidth, 4, WHITE);

  // Alerta visual
  if (gasValue > limiarGas) {
    display.clearDisplay();
    display.setTextSize(1.9);
    display.setCursor(0, 5);
    display.print("! ALERTA DE GAS !");
  }

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

void sendDataToServerPOST() {
  if (WiFi.status() == WL_CONNECTED) {
    std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
    client->setInsecure();
    HTTPClient http;

    String url = "https://home.byteslivres.com.br/api/data";
    http.begin(*client, url);

    // Define cabeçalho para JSON
    http.addHeader("Content-Type", "application/json");

    // Cria JSON com os dados
    String jsonPayload = "{";
    jsonPayload += "\"temperature\":" + String(temperature, 1) + ",";
    jsonPayload += "\"humidity\":" + String(humidity, 1) + ",";
    jsonPayload += "\"gas\":" + String(gasValue);
    jsonPayload += "}";

    Serial.println("Enviando JSON:");
    Serial.println(jsonPayload);

    // Envia POST com o JSON
    int httpCode = http.POST(jsonPayload);

    // Verifica resposta
    if (httpCode > 0) {
      Serial.printf("Código de resposta: %d\n", httpCode);
      String response = http.getString();
      Serial.println("Resposta: " + response);
    } else {
      Serial.printf("Erro na requisição: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end(); // Libera recursos
  } else {
    Serial.println("WiFi não conectado");
  }
}

