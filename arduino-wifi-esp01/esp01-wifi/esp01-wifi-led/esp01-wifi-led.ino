#include <SoftwareSerial.h>
 
//RX pino 2, TX pino 3
SoftwareSerial esp8266(2, 3);
 
#define DEBUG true
#define LED 5
 
void setup()
{
  Serial.begin(9600);
  esp8266.begin(19200);
  pinMode(LED, OUTPUT);
 
  sendData("AT+RST\r\n", 2000, DEBUG); // rst
  // Conecta a rede wireless
  sendData("AT+CWJAP=\"BTL\",\"#Brasil2020\"\r\n", 2000, DEBUG);
  delay(5000);
  sendData("AT+CWMODE=1\r\n", 1000, DEBUG);
  // Mostra o endereco IP
  sendData("AT+CIFSR\r\n", 1000, DEBUG);
  // Configura para multiplas conexoes
  sendData("AT+CIPMUX=1\r\n", 1000, DEBUG);
  // Inicia o web server na porta 80
  sendData("AT+CIPSERVER=1,80\r\n", 1000, DEBUG);
}
 
void loop()
{
  // Verifica se o ESP8266 esta enviando dados
  if (esp8266.available())
  {
    if (esp8266.find("+IPD,"))
    {
      delay(300);
      int connectionId = esp8266.read() - 48;

      String message = readWifiSerialMessage();
 
      String webpage = message;
     
      if (message.indexOf("GET /LED/ON") >= 0) {
              digitalWrite(LED,HIGH);
              Serial.print("LED ON");
      }

      if (message.indexOf("GET /LED/OFF") >= 0) {
              digitalWrite(LED,LOW);
              Serial.print("LED DOWN");
      }
 
      String cipSend = "AT+CIPSEND=";
      cipSend += connectionId;
      cipSend += ",";
      cipSend += webpage.length();
      cipSend += "\r\n";
 
      sendData(cipSend, 1000, DEBUG);
      sendData(webpage, 1000, DEBUG);
 
      String closeCommand = "AT+CIPCLOSE=";
      closeCommand += connectionId; // append connection id
      closeCommand += "\r\n";
 
      sendData(closeCommand, 3000, DEBUG);
    }
  }
}
 
String sendData(String command, const int timeout, boolean debug)
{
  // Envio dos comandos AT para o modulo
  String response = "";
  esp8266.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (esp8266.available())
    {
      // The esp has data so display its output to the serial window
      char c = esp8266.read(); // read the next character.
      response += c;
    }
  }
  if (debug)
  {
    Serial.print(response);
  }
  return response;
}

/*
* Name: readWifiSerialMessage
* Description: Function used to read data from ESP8266 Serial.
* Params: 
* Returns: The response from the esp8266 (if there is a reponse)
*/
String  readWifiSerialMessage(){
  char value[100]; 
  int index_count =0;
  while(esp8266.available()>0){
    value[index_count]=esp8266.read();
    index_count++;
    value[index_count] = '\0'; // Null terminate the string
  }
  String str(value);
  str.trim();
  return str;
}
