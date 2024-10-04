#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Distance to Empty Reader");

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
  // Send the request for Distance to Empty data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x21); // PID for Distance to Empty
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||     // Correct length
      CAN.read() == 0x41 ||     // Correct mode
      CAN.read() == 0x21) {     // Correct PID for Distance to Empty
    // Read and parse the Distance to Empty value
    int distanceToEmpty_LSB = CAN.read(); // Least significant byte
    int distanceToEmpty_MSB = CAN.read(); // Most significant byte
    int distanceToEmpty = (distanceToEmpty_MSB << 8) | distanceToEmpty_LSB;

    // Print Distance to Empty
    Serial.print("Distance to Empty: ");
    Serial.print(distanceToEmpty);
    Serial.println(" km");
  }

  delay(5000);
}
