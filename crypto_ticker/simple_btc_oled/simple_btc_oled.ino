#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const char* ssid     = "BTL";
const char* password = "#Brasil2020";

#define CD_API "/v1/bpi/currentprice.json"
#define CD_URL "api.coindesk.com"

static char respBuffer[4096];

WiFiClientSecure client;

void setup()
{
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  
}

String data;

void loop()
{
  getData();
  printData(data);
  delay(10000);
}

void getData()
{

  
  const char request[] = 
    "GET " CD_API " HTTP/1.1\r\n"
    "User-Agent: ESP8266/0.1\r\n"
    "Accept: */*\r\n"
    "Host: " CD_URL "\r\n"
    "Connection: close\r\n"
    "\r\n";    
  Serial.print("Requesting URL: ");
  Serial.println(CD_URL);
  
  delay(100);
  
  if (!client.connect(CD_URL, 443))
  {
    Serial.println("Connection failed");
    return;
  }

  client.print(request);
  client.flush();
  delay(1000);

  uint16_t index = 0;
  while(client.connected())
  {
    if(client.available())
    {
      respBuffer[index++] = client.read();
      delay(1);
    }
  }
  client.stop();
  char * json = strchr(respBuffer,'{');
  String json_str = String(json);
  
  ///////////////////////////////////////////
  // The Response Buffer currently (03.11.18)
  // contains a stray 'd' character that
  // corrupts the data. This removes it, but
  // shouldn't be necessary when the issue is 
  // fixed!
  uint16_t idx_d = json_str.lastIndexOf('d');
  json_str.remove(idx_d,3);
  ///////////////////////////////////////////

  Serial.println("String json:");
  Serial.println(json_str);
  
  DynamicJsonDocument jBuffer(1024);
  deserializeJson(jBuffer, json_str);
  //JsonObject& root = jBuffer.parseObject(json_str);
  JsonObject root = jBuffer.as<JsonObject>();

  
  //Serial.println("JsonObject: ");
  //Serial.println(root);
  //root.prettyPrintTo(Serial);
  //Serial.println();

  JsonObject bpi = root["bpi"];
  JsonObject usd = bpi["USD"];
  String tmp = usd["rate_float"];
  data = tmp;
  Serial.print("BTC (USD): $");
  Serial.println(data);
}

void printData(String data)
{
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  display.setCursor(0, 5);
  display.println(data);

  display.display(); 
}
