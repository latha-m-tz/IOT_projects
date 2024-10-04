#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Mass Air Flow (MAF) Reader");

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
  // Send the request for MAF data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x10); // MAF PID
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||     // Correct length
      CAN.read() == 0x41 ||     // Correct mode
      CAN.read() == 0x10) {     // Correct PID for MAF
    // Read and parse the MAF value
    int mafRaw_LSB = CAN.read(); // Least significant byte
    int mafRaw_MSB = CAN.read(); // Most significant byte
    int mafRaw = (mafRaw_MSB << 8) | mafRaw_LSB;

    // Convert raw value to actual MAF rate
    float mafRate = mafRaw *0.1; // Formula for converting raw value to MAF rate in grams/second

    // Print MAF rate
    Serial.print("Mass Air Flow (MAF) Rate (g/s): ");
    Serial.println(mafRate);
    delay(5000);
  }
}
