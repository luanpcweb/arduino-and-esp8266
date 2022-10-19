#include "ESP8266WiFi.h"

const char* ssid = "##"; //Enter SSID
const char* password = "##"; //Enter Password

int LED_A_R = 5;
int LED_A_G = 4;
int LED_A_B = 0;

int LED_B_R = 14;
int LED_B_G = 12;
int LED_B_B = 13;

WiFiServer server(80); 

int val = 0; 


void setup() {
  Serial.begin(115200);
   
  pinMode (LED_A_R, OUTPUT);
  pinMode (LED_A_G, OUTPUT);
  pinMode (LED_A_B, OUTPUT);


  pinMode (LED_B_R, OUTPUT);
  pinMode (LED_B_G, OUTPUT);
  pinMode (LED_B_B, OUTPUT);

  digitalWrite(LED_A_R, LOW);
  digitalWrite(LED_A_G, LOW);
  digitalWrite(LED_A_B, LOW);
  
  digitalWrite(LED_B_R, LOW);
  digitalWrite(LED_B_G, LOW);
  digitalWrite(LED_B_B, LOW);

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

  grpA(req, client);
  grpB(req, client);

  if (req.indexOf("OFFALL") != -1) {
    offAll(client);
  }

  if (req.indexOf("ONALL") != -1) {
    onAll(client);
  }
 

  Serial.println("Cliente desconectado");

}

void grpA(String req, WiFiClient client){

  // Verifica se existe a substring LED_A RED
  if (req.indexOf("LED_A_R_ON") != -1) {
    digitalWrite(LED_A_R, 1);

    val = digitalRead(LED_A_R);
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Access-Control-Allow-Origin: *");
    client.print("\r\n\r\n");
    client.print("ON:");
    client.print(val);
    
    Serial.println(req.indexOf("LED_A_R_ON"));
    
  } else if (req.indexOf("LED_A_R_OFF") != -1) {
    digitalWrite(LED_A_R, 0);

    val = digitalRead(LED_A_R);
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Access-Control-Allow-Origin: *");
    client.print("\r\n\r\n");
    client.print("OFF:");
    client.print(val);
    
    Serial.println(req.indexOf("LED_A_R_OFF"));
  
  } else if (req.indexOf("LED_A_R_STATUS") != -1) {
    
    val = digitalRead(LED_A_R);
    
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Access-Control-Allow-Origin: *");
    client.print("\r\n\r\n");
    client.print("STATUS:");
    client.print(val);
    
    Serial.println(val);
  } 


  // Verifica se existe a substring LED_A GREEN
  if (req.indexOf("LED_A_G_ON") != -1) {
    digitalWrite(LED_A_G, 1);

    val = digitalRead(LED_A_G);
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Access-Control-Allow-Origin: *");
    client.print("\r\n\r\n");
    client.print("ON:");
    client.print(val);
    
    Serial.println(req.indexOf("LED_A_G_ON"));
    
  } else if (req.indexOf("LED_A_G_OFF") != -1) {
    digitalWrite(LED_A_G, 0);

    val = digitalRead(LED_A_G);
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Access-Control-Allow-Origin: *");
    client.print("\r\n\r\n");
    client.print("OFF:");
    client.print(val);
    
    Serial.println(req.indexOf("LED_A_G_OFF"));
  
  } else if (req.indexOf("LED_A_G_STATUS") != -1) {
    
    val = digitalRead(LED_A_G);
    
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Access-Control-Allow-Origin: *");
    client.print("\r\n\r\n");
    client.print("STATUS:");
    client.print(val);
    
    Serial.println(val);
  } 


  // Verifica se existe a substring LED_A BLUE
  if (req.indexOf("LED_A_B_ON") != -1) {
    digitalWrite(LED_A_B, 1);

    val = digitalRead(LED_A_B);
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Access-Control-Allow-Origin: *");
    client.print("\r\n\r\n");
    client.print("ON:");
    client.print(val);
    
    Serial.println(req.indexOf("LED_A_B_ON"));
    
  } else if (req.indexOf("LED_A_B_OFF") != -1) {
    digitalWrite(LED_A_B, 0);

    val = digitalRead(LED_A_B);
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Access-Control-Allow-Origin: *");
    client.print("\r\n\r\n");
    client.print("OFF:");
    client.print(val);
    
    Serial.println(req.indexOf("LED_A_B_OFF"));
  
  } else if (req.indexOf("LED_A_B_STATUS") != -1) {
    
    val = digitalRead(LED_A_B);
    
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Access-Control-Allow-Origin: *");
    client.print("\r\n\r\n");
    client.print("STATUS:");
    client.print(val);
    
    Serial.println(val);
  } 
}

void grpB(String req, WiFiClient client){

  // Verifica se existe a substring LED_A RED
  if (req.indexOf("LED_B_R_ON") != -1) {
    digitalWrite(LED_B_R, 1);

    val = digitalRead(LED_B_R);
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Access-Control-Allow-Origin: *");
    client.print("\r\n\r\n");
    client.print("ON:");
    client.print(val);
    
    Serial.println(req.indexOf("LED_B_R_ON"));
    
  } else if (req.indexOf("LED_B_R_OFF") != -1) {
    digitalWrite(LED_B_R, 0);

    val = digitalRead(LED_B_R);
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Access-Control-Allow-Origin: *");
    client.print("\r\n\r\n");
    client.print("OFF:");
    client.print(val);
    
    Serial.println(req.indexOf("LED_B_R_OFF"));
  
  } else if (req.indexOf("LED_B_R_STATUS") != -1) {
    
    val = digitalRead(LED_B_R);
    
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Access-Control-Allow-Origin: *");
    client.print("\r\n\r\n");
    client.print("STATUS:");
    client.print(val);
    
    Serial.println(val);
  } 


  // Verifica se existe a substring LED_A GREEN
  if (req.indexOf("LED_B_G_ON") != -1) {
    digitalWrite(LED_B_G, 1);

    val = digitalRead(LED_B_G);
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Access-Control-Allow-Origin: *");
    client.print("\r\n\r\n");
    client.print("ON:");
    client.print(val);
    
    Serial.println(req.indexOf("LED_B_G_ON"));
    
  } else if (req.indexOf("LED_B_G_OFF") != -1) {
    digitalWrite(LED_B_G, 0);

    val = digitalRead(LED_B_G);
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Access-Control-Allow-Origin: *");
    client.print("\r\n\r\n");
    client.print("OFF:");
    client.print(val);
    
    Serial.println(req.indexOf("LED_B_G_OFF"));
  
  } else if (req.indexOf("LED_B_G_STATUS") != -1) {
    
    val = digitalRead(LED_B_G);
    
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Access-Control-Allow-Origin: *");
    client.print("\r\n\r\n");
    client.print("STATUS:");
    client.print(val);
    
    Serial.println(val);
  } 


  // Verifica se existe a substring LED_A BLUE
  if (req.indexOf("LED_B_B_ON") != -1) {
    digitalWrite(LED_B_B, 1);

    val = digitalRead(LED_B_B);
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Access-Control-Allow-Origin: *");
    client.print("\r\n\r\n");
    client.print("ON:");
    client.print(val);
    
    Serial.println(req.indexOf("LED_B_B_ON"));
    
  } else if (req.indexOf("LED_B_B_OFF") != -1) {
    digitalWrite(LED_B_B, 0);

    val = digitalRead(LED_B_B);
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Access-Control-Allow-Origin: *");
    client.print("\r\n\r\n");
    client.print("OFF:");
    client.print(val);
    
    Serial.println(req.indexOf("LED_B_B_OFF"));
  
  } else if (req.indexOf("LED_B_B_STATUS") != -1) {
    
    val = digitalRead(LED_B_B);
    
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Access-Control-Allow-Origin: *");
    client.print("\r\n\r\n");
    client.print("STATUS:");
    client.print(val);
    
    Serial.println(val);
  } 
}

void offAll(WiFiClient client) {

  digitalWrite(LED_A_R, LOW);
  digitalWrite(LED_A_G, LOW);
  digitalWrite(LED_A_B, LOW);
  
  digitalWrite(LED_B_R, LOW);
  digitalWrite(LED_B_G, LOW);
  digitalWrite(LED_B_B, LOW);

  client.print("HTTP/1.1 200 OK\r\n");
  client.print("Access-Control-Allow-Origin: *");
  client.print("\r\n\r\n");
  client.print("OFFALL");
}


void onAll(WiFiClient client) {

  digitalWrite(LED_A_R, 1);
  digitalWrite(LED_A_G, 1);
  digitalWrite(LED_A_B, 1);
  
  digitalWrite(LED_B_R, 1);
  digitalWrite(LED_B_G, 1);
  digitalWrite(LED_B_B, 1);

  client.print("HTTP/1.1 200 OK\r\n");
  client.print("Access-Control-Allow-Origin: *");
  client.print("\r\n\r\n");
  client.print("ONALL");
}
