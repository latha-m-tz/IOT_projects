#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Supported PIDs Reader");

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
  // Iterate through PIDs from 01 to 20
  for (int pid = 0x01; pid <= 0x20; pid++) {
    // Send the request for each PID
    if (useStandardAddressing) {
      CAN.beginPacket(0x7df, 8);
    } else {
      CAN.beginExtendedPacket(0x18db33f1, 8);
    }
    CAN.write(0x02); // Number of additional bytes
    CAN.write(0x01); // Request current data
    CAN.write(0x00);  // Send current PID
    CAN.endPacket();

    // Wait for response
    while (CAN.parsePacket() == 0);

    // Check if the response is for the requested PID
    if (CAN.read() == 0x04 ||   // Correct length
        CAN.read() == 0x41 ||   // Correct mode
        CAN.read() == 0x00); {    // Response matches requested PID
      // PID is supported, print it
      Serial.print("PID 0x");
      if (pid < 0x10) {
        Serial.print("0"); // Print leading zero for single digit PIDs
      }
      Serial.println(pid, HEX);
    }
  }

  // Wait before querying again (adjust as needed)
  delay(5000);
}

