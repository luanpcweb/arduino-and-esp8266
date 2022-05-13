#include "SoftwareSerial.h"

SoftwareSerial ESP(2, 3); // RX, TX

const int RST = 4;

#define DEBUG true

void setup(){
  Serial.begin(9600);
  ESP.begin(9600);
  
  //Pulso em Reset para aceitar comunicacao
  pinMode(RST, OUTPUT);
  digitalWrite(RST, LOW);
  delay(300);
  digitalWrite(RST, HIGH);

  sendData("AT+CWMODE=1rn", 1000, DEBUG);

  sendData("AT+CWJAP=\"BTL\",\"#Brasil2020\"rn", 300, DEBUG);
  
  //sendData("AT+CIFSR", 30000, DEBUG);

  // Inicia o web server na porta 80
  //sendData("AT+CIPSERVER=1,80rn", 1000, DEBUG);
  

//  sendData("AT+RSTrn", 2000, DEBUG); // rst
//  // Conecta a rede wireless
//  sendData("AT+CWJAP=\"BTL\",\"#Brasil2020rn", 2000, DEBUG);
//  delay(3000);
//  sendData("AT+CWMODE=1rn", 1000, DEBUG);
//  // Mostra o endereco IP
//  sendData("AT+CIFSRrn", 1000, DEBUG);
//  // Configura para multiplas conexoes
//  sendData("AT+CIPMUX=1rn", 1000, DEBUG);
//  // Inicia o web server na porta 80
//  sendData("AT+CIPSERVER=1,80rn", 1000, DEBUG);
}

void loop(){
  if (ESP.available()){
    Serial.write(ESP.read());
  }
  if (Serial.available()){
    ESP.write(Serial.read());
  }

  // Verifica se o ESP8266 esta enviando dados
//  if (ESP.available())
//  {
//    if (ESP.find("+IPD,"))
//    {
//      delay(300);
//      int connectionId = ESP.read() - 48;
//
//      String webpage = "<head><meta http-equiv=""refresh"" content=""3"">";
//      webpage += "</head><h1><u>ESP8266 - Web Server</u></h1><h2>Porta";
//      webpage += "Digital 8: ";
//      int a = digitalRead(8);
//      webpage += a;
//      webpage += "<h2>Porta Digital 9: ";
//      int b = digitalRead(9);
//      webpage += b;
//      webpage += "</h2>";
//
//      String cipSend = "AT+CIPSEND=";
//      cipSend += connectionId;
//      cipSend += ",";
//      cipSend += webpage.length();
//      cipSend += "rn";
//
//      sendData(cipSend, 1000, DEBUG);
//      sendData(webpage, 1000, DEBUG);
//
//      String closeCommand = "AT+CIPCLOSE=";
//      closeCommand += connectionId; // append connection id
//      closeCommand += "rn";
//
//      sendData(closeCommand, 3000, DEBUG);
//    }
//  }
  
}

String sendData(String command, const int timeout, boolean debug)
{
  // Envio dos comandos AT para o modulo
  String response = "";
  ESP.print(command);
  
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (ESP.available())
    {
      // The esp has data so display its output to the serial window
      char c = ESP.read(); // read the next character.
      response += c;
    }
  }
  
  if (debug)
  {
    Serial.print("[]");
    Serial.print(response);
  }
  
  return response;
}
