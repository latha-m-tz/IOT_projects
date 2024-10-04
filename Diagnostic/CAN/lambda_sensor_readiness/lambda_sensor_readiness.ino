#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Lambda Sensor Readiness Reader");

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
  // Send the request for readiness status
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x01); // PID for readiness status
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||     // Correct length
      CAN.read() == 0x41 ||     // Correct mode
      CAN.read() == 0x01) {     // Correct PID for readiness status
    // Read and parse the readiness status
    int readinessStatus = CAN.read(); // Readiness status value

    // Check the status of lambda sensor readiness
    bool lambdaReady = (readinessStatus & (1 << 3)) != 0;

    // Print lambda sensor readiness
    Serial.print("Lambda Sensor Readiness: ");
    Serial.println(lambdaReady ? "Ready" : "Not Ready");
    delay(5000);
  }
}
