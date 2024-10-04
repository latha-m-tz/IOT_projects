// #include <SPI.h>
// #include <MFRC522.h>

// #define RST_PIN         5          // Configurable, see typical pin layout above
// #define SS_PIN          10         // Configurable, see typical pin layout above

// MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

// void setup() {
// 	Serial.begin(9600);		// Initialize serial communications with the PC
// 	while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
// 	SPI.begin();			// Init SPI bus
// 	mfrc522.PCD_Init();		// Init MFRC522
// 	delay(4);				// Optional delay. Some board do need more time after init to be ready, see Readme
// 	mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
// 	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
// }

// void loop() {
// 	// Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
// 	if ( ! mfrc522.PICC_IsNewCardPresent()) {
// 		return;
// 	}

// 	// Select one of the cards
// 	if ( ! mfrc522.PICC_ReadCardSerial()) {
// 		return;
// 	}

// 	// Dump debug info about the card; PICC_HaltA() is automatically called
// 	mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
// }


// E8 75 3B 4A









#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 5
#define SS_PIN 10
#define led 7

byte readCard[4];
String MasterTag = "E8753B4A";  // E8753B4B	(Suppose this is lock's key)
String tagID = "";

// Create instances
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() 
{
  // Initiating
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  SPI.begin(); // SPI bus
  mfrc522.PCD_Init(); // MFRC522
}

void loop() 
{ 
  //Wait until new tag is available
  while (getID()) 
  { 
    if (tagID == MasterTag) 
    {
      
      Serial.println(" Access Granted! ");
      digitalWrite(led, HIGH);
      delay(500);
      digitalWrite(led, LOW);
      Serial.println();
      // You can write any code here like opening doors, switching on a relay, lighting up an LED, or anything else you can think of.
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

//Read new tag if available
boolean getID() 
{
  // Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
  return false;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue
  return false;
  }
  tagID = "";
  for ( uint8_t i = 0; i < 4; i++) { // The MIFARE PICCs that we use have 4 byte UID
  //readCard[i] = mfrc522.uid.uidByte[i];
  tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String variable
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  return true;
}