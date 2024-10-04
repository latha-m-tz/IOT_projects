#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Warm-Up Fuel Correction Reader");

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
  // Send the request for warm-up fuel correction data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x2F); // PID for warm-up fuel correction
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||     // Correct length
      CAN.read() != 0x41 ||     // Correct mode
      CAN.read() != 0x2F) {     // Correct PID for warm-up fuel correction
    // Read and parse the warm-up fuel correction value
    int warmUpFuelCorrection = CAN.read()/2.55; // Warm-up fuel correction value

    // Print warm-up fuel correction
    Serial.print("Warm-Up Fuel Correction: ");
    Serial.println(warmUpFuelCorrection);
    delay(5000);
  }
}
