#include <CAN.h>

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Kill Switch Status Reader");

  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }
}

void loop() {
  // Send the request for kill switch status data
  CAN.beginPacket(0x7df); // Standard CAN ID for OBD-II diagnostic requests
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x10); // Diagnostic mode
  CAN.write(0x24); // PID for kill switch status
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||   // Correct length
      CAN.read() != 0x41 ||   // Correct mode
      CAN.read() != 0x24) {   // Correct PID for kill switch status
    // Read the kill switch status
    int killSwitchStatus = CAN.read();

    // Determine the status based on the received value
    if (killSwitchStatus == 0x01) {
      Serial.println("Kill Switch: Active");
    } else {
      Serial.println("Kill Switch: Inactive");
    }
  }

  // Add a delay before sending the next request
  delay(5000);
}
