#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Transmission Actual Gear Reader");

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
  // Send the request for transmission actual gear data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0xA4); // Replace XX with the appropriate PID for transmission actual gear
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||     // Correct length
      CAN.read() != 0x41 ||     // Correct mode
      CAN.read() != 0xA4) {     // Replace XX with the appropriate PID for transmission actual gear
    // Read and parse the transmission actual gear value
    int gear = CAN.read()*0.001; // Transmission gear value

    // Print transmission actual gear
    Serial.print("Transmission Actual Gear: ");
    Serial.println(gear);
    delay(5000);
  }
}
