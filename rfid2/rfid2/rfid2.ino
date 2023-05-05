#include <SPI.h>
#include <MFRC522.h>

// INPUT
#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);
// Init array that will store new NUID
byte nuidPICC[4];
void setup() {
 Serial.begin(9600);
 SPI.begin(); // Init SPI bus
 rfid.PCD_Init(); // Init MFRC522
 Serial.println(F("Scan RFID NUID..."));
}
void loop() {
           readRFID();
          delay(200);
}
//
void readRFID()
{
  // Look for new card
  if ( ! rfid.PICC_IsNewCardPresent())
  return;
      // Verify if the NUID has been readed
  if (  !rfid.PICC_ReadCardSerial())
  return;
  
  if (rfid.uid.uidByte[0] != nuidPICC[0] ||
      rfid.uid.uidByte[1] != nuidPICC[1] ||
      rfid.uid.uidByte[2] != nuidPICC[2] ||
      rfid.uid.uidByte[3] != nuidPICC[3] ) {
      Serial.println(F("A new card has been detected."));
      // Store NUID into nuidPICC array
      for (byte i = 0; i < 4; i++) {
          nuidPICC[i] = rfid.uid.uidByte[i];
      }
    
      Serial.print(F("RFID tag in dec: "));
      printDec(rfid.uid.uidByte, rfid.uid.size);
      Serial.println();
  }
    
  // Halt PICC
  rfid.PICC_HaltA();
  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}
/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
      Serial.print(buffer[i] < 0x10 ? " 0" : " ");
      Serial.print(buffer[i], DEC);
  }
}
