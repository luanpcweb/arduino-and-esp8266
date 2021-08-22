#include <ESP8266WiFi.h>

// Nome do seu wifi
const char* ssid = "BTL-700"; 

// Senha do seu wifi
const char* password = "@@##sapao200#"; 

// Porta de comunicacao (normalmente se utiliza a 80 ou 8080)
WiFiServer server(80); 

int val = 0; 

void setup() {
  // Para o node mcu esp8266 a velocidade do monitor serial e 115200
  Serial.begin(115200); 
  delay(10);
  
  pinMode(5, OUTPUT); 
  digitalWrite(5, 0); 

  // Mostra no monitor serial informacoes de conexao da rede
  Serial.println();
  Serial.println();
  Serial.print("conectando em ");
  Serial.println(ssid);
  
  // Inicializando a conexao
  WiFi.begin(ssid, password); 
  
  /* Enquanto nao conseguir conectar
    imprime um ponto na tela (dá a ideia de que esta carregando) */
  
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print("."); 
  }

  Serial.println("");
  Serial.println("WiFi connectado");

  // Inicializa o servidor (nesse caso o proprio esp8266)
  server.begin();
  Serial.println("Servidor inicializado");
  
  // Mostra o IP do servidor
  Serial.println(WiFi.localIP()); 
}

void loop() {
  
  // Guarda o status do servidor
  WiFiClient client = server.available(); 
  if ( ! client) {
    return;
  }
  
  // Quando estiver alguem acessando 
  Serial.println("novo cliente"); 
  
  // Enquanto nao tiver cliente
  while ( ! client.available()) { 
    delay(1);
  }
  
  // Lê caracteres do buffer serial
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Verifica se existe a substring led5_on
  if (req.indexOf("led5_on") != -1) {
    digitalWrite(5, 1);

    val = digitalRead(5);
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Access-Control-Allow-Origin: *");
    client.print("\r\n\r\n");
    client.print("ON:");
    client.print(val);
    
    Serial.println(req.indexOf("led5_on"));
    
  } else if (req.indexOf("led5_off") != -1) {
    digitalWrite(5, 0);

    val = digitalRead(5);
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Access-Control-Allow-Origin: *");
    client.print("\r\n\r\n");
    client.print("OFF:");
    client.print(val);
    
    Serial.println(req.indexOf("led5_off"));
  
  } else if (req.indexOf("led5_status") != -1) {
    
    val = digitalRead(5);
    
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Access-Control-Allow-Origin: *");
    client.print("\r\n\r\n");
    client.print("STATUS:");
    client.print(val);
    
    Serial.println(val);
  } 

  Serial.println("Cliente desconectado");
}
