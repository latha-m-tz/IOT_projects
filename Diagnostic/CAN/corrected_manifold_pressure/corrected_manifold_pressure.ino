#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Corrected Manifold Pressure Reader");

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
  // Send the request for corrected MAP data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x36); // Corrected MAP PID
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||     // Correct length
      CAN.read() == 0x41 ||     // Correct mode
      CAN.read() == 0x36) {     // Correct PID for corrected MAP
    // Read and parse the corrected MAP value
    int pressureRaw_LSB = CAN.read(); // Least significant byte
    int pressureRaw_MSB = CAN.read(); // Most significant byte
    unsigned int pressureRaw = (pressureRaw_MSB << 8) | pressureRaw_LSB;

    // Convert raw pressure value to actual pressure (kPa)
    float pressure = (10* pressureRaw)/256; // Convert from hectopascals (hPa) to kilopascals (kPa)

    // Print corrected MAP
    Serial.print("Corrected Manifold Pressure (kPa): ");
    Serial.println(pressure);
    delay(5000);
  }
}
