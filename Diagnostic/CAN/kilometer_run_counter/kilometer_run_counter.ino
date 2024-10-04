#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Kilometer Run Counter Reader");

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
  // Send the request for Kilometer Run Counter data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x00); // PID for Kilometer Run Counter
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||     // Correct length
      CAN.read() == 0x41 ||     // Correct mode
      CAN.read() == 0x00) {     // Correct PID for Kilometer Run Counter
    // Read and parse the Kilometer Run Counter value
    int kilometerRunCounter_LSB = CAN.read(); // Least significant byte    int kilometerRunCounter_MSB = CAN.read(); // Most significant byte
    int kilometerRunCounter = (kilometerRunCounter_MSB << 8) | kilometerRunCounter_LSB;

    // Print Kilometer Run Counter
    Serial.print("Kilometer Run Counter: ");
    Serial.print(kilometerRunCounter);
    Serial.println("km");
  }

  delay(5000);
}
