//created by www.RoboCore.net 

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "MinhaRede";
const char* password = "12345678";

String site = "";

ESP8266WebServer server(80); // server: http://192.168.4.1

void handleRoot() {
  site =  "<html>\n";
  site += "<head><title>Hello World</title></head>\n";
  site += "<body style=\"color: blue\">\n";
  site += "<center><h1>Hello World do meu ESP8266</h1></center>\n";
  site += "</body>\n";
  site += "</html>";

  server.send(200, "text/html", site);

  site = "";
}

void setup() {
  WiFi.mode(WIFI_AP); //aceita WIFI_AP / WIFI_AP_STA / WIFI_STA
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}
