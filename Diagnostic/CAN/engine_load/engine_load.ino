#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Engine Load Reader");

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
  // Send the request for engine load data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x04); // Engine load PID
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||     // Correct length
      CAN.read() != 0x41 ||     // Correct mode
      CAN.read() == 0x04) {     // Correct PID for engine load
    // Read and parse the engine load value
    int loadRaw = CAN.read(); // Engine load value

    // Calculate engine load (percentage)
    float engineLoad = (loadRaw / 2.55); // Convert raw value to percentage (0-100)

    // Print engine load
    Serial.print("Engine Load (%): ");
    Serial.print(engineLoad); // Print the engine load in percentage
    Serial.println("%");
    delay(5000);
  }
}
