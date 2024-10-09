#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10

byte readCard[4];
//

String MasterTag = "E8753B4A";
String tagID = "";

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup()
{
  Serial.begin(9600);
  SPI.begin();           // SPI bus
  mfrc522.PCD_Init();    // MFRC522
}

void loop()
{
  while (getID())
  {
    if (tagID == MasterTag)
    {
      Serial.println("True"); 
    }
    else
    {
      Serial.println("False"); 
    }
  }
}

boolean getID()
{
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return false;
  }
  if (!mfrc522.PICC_ReadCardSerial())
  { 
    return false;
  }
  tagID = "";
  for (uint8_t i = 0; i < 4; i++)
  { 
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA();
  return true;
}
