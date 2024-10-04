#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Warm-up Cycles Since DTCs Cleared Reader");

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
  // Send the request for warm-up cycles since DTCs cleared data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x30); // Warm-up Cycles Since DTCs Cleared PID
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||     // Correct length
      CAN.read() == 0x41 ||     // Correct mode
      CAN.read() == 0x30) {     // Correct PID for Warm-up Cycles Since DTCs Cleared
    // Read and parse the number of warm-up cycles since DTCs cleared value
    int warmUpCycles = CAN.read(); // Number of warm-up cycles since DTCs cleared

    // Print number of warm-up cycles since DTCs cleared
    Serial.print("Warm-up Cycles Since DTCs Cleared: ");
    Serial.println(warmUpCycles);
    delay(5000);
  }
}
