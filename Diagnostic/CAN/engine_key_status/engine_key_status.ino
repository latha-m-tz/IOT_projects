#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Vehicle Speed Reader");

  // Start the CAN bus at 500 kbps
  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");   
     while (1);
  }

  // Add filter to only receive the CAN bus IDs we care about
  if (useStandardAddressing) {
    CAN.filter(0x7e8);
  } else {
    CAN.filterExtended(0x18daf110);
  }
}

void loop() {
  // Send the request for vehicle speed data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x01); // PID for engine key-on status
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||   // Correct length
      CAN.read() == 0x41 ||   // Correct mode
      CAN.read() == 0x01) {   // Correct PID for engine key-on status
    // Read the engine key-on status
    int keyOnStatus = CAN.read();

    // Interpret the engine key-on status
    String engineStatus;
    if (keyOnStatus == 0x01) {
      engineStatus = "Engine is ON";
    } else {
      engineStatus = "Engine is OFF";
    }

    // Print the engine key-on status
    Serial.println(engineStatus);
  }

  // Add a delay before sending the next request
  delay(5000);
}
