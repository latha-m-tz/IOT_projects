#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Idle Speed Control Status Reader");

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
  // Send the request for idle speed control status data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x1A); // PID for idle speed control status
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x03 ||     // Correct length
      CAN.read() != 0x41 ||     // Correct mode
      CAN.read() != 0x1A) {     // Correct PID for idle speed control status
    // Read and parse the idle speed control status value
    int idleSpeedControlStatus = CAN.read()*1/2.55; // Idle speed control status value

    // Print idle speed control status
    Serial.print("Idle Speed Control Status: ");
    Serial.println(idleSpeedControlStatus ? "Active" : "Inactive");
    delay(5000);
  }
}
