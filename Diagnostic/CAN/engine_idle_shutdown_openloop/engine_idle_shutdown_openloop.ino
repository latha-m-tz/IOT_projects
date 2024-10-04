#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Number of Engine Switch Offs at Idle (Open Loop) Reader");

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
  // Send the request for Number of Engine Switch Offs at Idle (Open Loop) data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x10); // PID for Number of Engine Switch Offs at Idle (Open Loop)
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||     // Correct length
      CAN.read() == 0x41 ||     // Correct mode
      CAN.read() == 0x10) {     // Correct PID for Number of Engine Switch Offs at Idle (Open Loop)
    // Read and parse the Number of Engine Switch Offs at Idle (Open Loop) value
    int engineSwitchOffsIdleOpenLoop = CAN.read(); // Number of Engine Switch Offs at Idle (Open Loop) value

    // Print Number of Engine Switch Offs at Idle (Open Loop)
    Serial.print("Number of Engine Switch Offs at Idle (Open Loop): ");
    Serial.println(engineSwitchOffsIdleOpenLoop);
  }

  delay(5000);
}
