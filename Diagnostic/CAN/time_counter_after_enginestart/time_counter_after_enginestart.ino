#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Time Counter After Engine Start Reader");

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
  // Send the request for time counter after engine start data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x4D); // PID for time counter after engine start
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||     // Correct length
      CAN.read() == 0x41 ||     // Correct mode
      CAN.read() == 0x4D) {     // Correct PID for time counter after engine start
    // Read and parse the time counter after engine start value
    int timeCounter_LSB = CAN.read(); // Least significant byte
    int timeCounter_MSB = CAN.read(); // Most significant byte
    int timeCounter = (timeCounter_MSB << 8) | timeCounter_LSB;

    // Apply the formula (if applicable)
    // Replace the formula with the appropriate one based on your vehicle's specification
    float timeAfterStart = timeCounter * 0.1; // Example formula: Multiply by 0.1 to convert to seconds

    // Print time counter after engine start
    Serial.print("Time Counter After Engine Start: ");
    Serial.print(timeAfterStart);
    Serial.println(" seconds");
    delay(5000);
  }
}
