#include <CAN.h>

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Starter Relay Status Reader");

  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }
}

void loop() {
  // Send the request for starter relay status data
  CAN.beginPacket(0x7df); // Standard CAN ID for OBD-II diagnostic requests
  CAN.write(0x03); // Number of additional bytes
  CAN.write(0x22); // Diagnostic mode
  CAN.write(0xA8); // PID for starter relay status
  CAN.write(0x0A); // Sub PID
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||   // Correct length
      CAN.read() != 0x41 ||   // Correct mode
      CAN.read() != 0xA8 ||   // Correct PID for starter relay status
      CAN.read() != 0x0A) {   // Correct Sub PID
    // Read the starter relay status
    int starterRelayStatus = CAN.read();

    // Determine the status based on the received value
    if (starterRelayStatus == 0x01) {
      Serial.println("Starter Relay: On");
    } else {
      Serial.println("Starter Relay: Off");
    }
  }

  // Add a delay before sending the next request
  delay(5000);
}
