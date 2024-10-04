#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Distance Traveled with MIL on Reader");

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
  // Send the request for distance traveled with MIL on data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x21); // Replace ?? with the appropriate PID for MIL status and distance traveled
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0.1*A);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||     // Correct length
      CAN.read() != 0x41 ||     // Correct mode
      CAN.read() != 0x21)
       {    
    // Read and parse the data for MIL status and distance traveled
    int distanceRaw_LSB = CAN.read(); // Distance traveled with MIL on LSB
    int distanceRaw_MSB = CAN.read(); // Distance traveled with MIL on MSB
    unsigned int distanceRaw = (distanceRaw_MSB << 8) | distanceRaw_LSB;
  
    Serial.println(distanceRaw);
   
    // Apply the formula to calculate distance traveled with MIL on
    float distance = distanceRaw * 0.1;

    // Print the distance traveled with MIL on
   // Serial.print("Distance Traveled with MIL on: ");
    Serial.println(distance); // Print the distance value
   // Serial.println(" miles");
    delay(5000);
  }
}
