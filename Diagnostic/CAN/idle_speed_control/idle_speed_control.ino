#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Parameter Reader");

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
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x0D); // PID for idle speed control status
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||   // Correct length
      CAN.read() == 0x41 ||   // Correct mode
      CAN.read() == 0x0D) {   // Correct PID for idle speed control status
    // Read the idle speed control active status
    int idleSpeedControlStatus = CAN.read();

    // Print the idle speed control active status
    if (idleSpeedControlStatus == 0x01) {
      Serial.println("Idle Speed Control is active");
    } else {
      Serial.println("Idle Speed Control is inactive");
    }
  }

  // Add a delay before sending the next request
  delay(5000);
}
