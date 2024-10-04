#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Distance Traveled since DTCs Cleared Reader");

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
  // Send the request for distance traveled since DTCs cleared data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x31); // Distance traveled since DTCs cleared PID
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||     // Correct length
      CAN.read() != 0x41 ||     // Correct mode
      CAN.read() != 0x31) {     // Correct PID for distance traveled since DTCs cleared
    // Read and parse the distance traveled since DTCs cleared value
    int distanceRaw_LSB = CAN.read(); // Least significant byte
    int distanceRaw_MSB = CAN.read(); // Most significant byte
    int distanceRaw = (distanceRaw_MSB << 8) | distanceRaw_LSB;

    // Convert raw value to actual distance traveled since DTCs cleared
    float distanceTraveled = (1.0*distanceRaw)/100;

    // Print distance traveled since DTCs cleared
    Serial.print("Distance Traveled since DTCs Cleared: ");
    Serial.print(distanceTraveled);
    Serial.println(" km");
    delay(5000);
  }
}
