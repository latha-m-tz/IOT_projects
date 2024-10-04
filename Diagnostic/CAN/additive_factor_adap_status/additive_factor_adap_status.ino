#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Additive Factor Adaptation Status Reader");

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
  // Send the request for additive factor adaptation status data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x60); // PID for additive factor adaptation status
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||     // Correct length
      CAN.read() == 0x41 ||     // Correct mode
      CAN.read() == 0x60) {     // Correct PID for additive factor adaptation status
    // Read and parse the additive factor adaptation status
    int adaptationStatus = CAN.read(); // Additive factor adaptation status value

    // Print additive factor adaptation status
    Serial.print("Additive Factor Adaptation Status: ");
    Serial.println(adaptationStatus ? "Active" : "Inactive");
    delay(5000);
  }
}
