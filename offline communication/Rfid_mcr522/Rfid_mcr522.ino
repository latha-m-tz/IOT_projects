#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 5
#define SS_PIN 10
#define led 7

byte readCard[4];
String MasterTag = "E8753B4A";  
String tagID = "";

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() 
{
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  SPI.begin(); 
  mfrc522.PCD_Init(); 
}

void loop() 
{ 
  while (getID()) 
  { 
    if (tagID == MasterTag) 
    {
      
      Serial.println(" Access Granted! ");
      digitalWrite(led, HIGH);
      delay(500);
      digitalWrite(led, LOW);
      Serial.println();
    }
    else
    {
      Serial.println(" Access Denied!");
          Serial.print("Sorry Your ID ");
    Serial.print(tagID);
    Serial.print(" is not mathched. ");
    Serial.println("\n\n");
    }


  Serial.print("Place Your Card >> ");
  }
}

boolean getID() 
{
  if ( ! mfrc522.PICC_IsNewCardPresent()) { 
  return false;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
  return false;
  }
  tagID = "";
  for ( uint8_t i = 0; i < 4; i++) { 
  tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); 
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA();
  return true;
}