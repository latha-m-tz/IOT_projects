#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10

byte readCard[4];
//

String MasterTag = "E8753B4A";
//String MasterTag = "12345678"; // Second access ID (change to your desired second access ID)
String tagID = "";

// Create instances
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup()
{
  Serial.begin(9600);
  SPI.begin();           // SPI bus
  mfrc522.PCD_Init();    // MFRC522
}

void loop()
{
  // Wait until new tag is available
  while (getID())
  {
    if (tagID == MasterTag)
    {
      Serial.println("True"); // Access granted
    }
    else
    {
      Serial.println("False"); // Access denied
    }
  }
}

// Read new tag if available  
boolean getID()
{
  // Getting ready for Reading PICCs
  if (!mfrc522.PICC_IsNewCardPresent())
  { // If a new PICC placed to RFID reader continue
    return false;
  }
  if (!mfrc522.PICC_ReadCardSerial())
  { // Since a PICC placed get Serial and continue
    return false;
  }
  tagID = "";
  for (uint8_t i = 0; i < 4; i++)
  { // The MIFARE PICCs that we use have 4 byte UID
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String variable
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  return true;
}
