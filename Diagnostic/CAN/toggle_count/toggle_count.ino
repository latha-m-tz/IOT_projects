#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II KL15 Toggle Count Reader");

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
  // Send the request for KL15 Toggle Count data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x20); // PID for KL15 Toggle Count
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||     // Correct length
      CAN.read() == 0x41 ||     // Correct mode
      CAN.read() == 0x20) {     // Correct PID for KL15 Toggle Count
    // Read and parse the KL15 Toggle Count value
    int kl15ToggleCount = CAN.read(); // KL15 Toggle Count value

    // Print KL15 Toggle Count
    Serial.print("KL15 Toggle Count: ");
    Serial.println(kl15ToggleCount);
  }

  delay(5000);
}
