#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II VIN reader");

  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }

  if (useStandardAddressing) {
    CAN.filter(0x7e8);
  } else {
    CAN.filterExtended(0x18daf110);
  }
}

void loop() {
  Serial.println("CAN OBD-II VIN reader");

  // Send the request for the first chunk
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x09); // Request vehicle information
  CAN.write(0x02); // Vehicle Identification Number (VIN)
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0 ||
         CAN.read() != 0x10 || CAN.read() != 0x14 || // Correct length
         CAN.read() != 0x49 ||                       // Correct mode
         CAN.read() != 0x02 ||                       // Correct PID
         CAN.read() != 0x01);

  // Print out raw data
  Serial.println("Raw data received:");
  while (CAN.available()) {
    byte data = CAN.read();
    Serial.print(data, HEX);
    Serial.print(" ");
  }   
 // Read in remaining chunks
  for (int i = 0; i < 2; i++) {
    // Send the request for the next chunk
    if (useStandardAddressing) {
      CAN.beginPacket(0x7e0, 8);
    } else {
      CAN.beginExtendedPacket(0x18db33f1, 8);
    }
    CAN.write(0x30);
    CAN.endPacket();

    // Wait for response
    while (CAN.parsePacket() == 0 ||
           CAN.read() != (0x21 + i)); // Correct sequence number

    // Print out raw data
    Serial.println("Additional data chunk received:");
    while (CAN.available()) {
      byte data = CAN.read();
      Serial.print(data, HEX);
      Serial.print(" ");
    }
    Serial.println(); // Print newline after raw data
  }

  Serial.println("That's all folks!");
  while (1); // All done*/
}
