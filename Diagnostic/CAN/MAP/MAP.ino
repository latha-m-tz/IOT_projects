#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II MAP Reader");

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
  // Send the request for MAP data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);s
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x0B); // MAP PID
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||     // Correct length
      CAN.read() == 0x41 ||     // Correct mode
      CAN.read() == 0x0B) {     // Correct PID for MAP
    // Read and parse the MAP value
    int mapRaw = CAN.read()*0.25; // MAP value in kPa

    // Print MAP value
    Serial.print("MAP (kPa): ");
    Serial.print(mapRaw); // Print the MAP value in kPa
    Serial.println(" kPa");
    delay(5000);
  }
}
