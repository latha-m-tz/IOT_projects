#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Battery Voltage During Last Cranking Reader");

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
  // Send the request for battery voltage during last cranking data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x42); // Battery Voltage During Last Cranking PID
  CAN.endPacket();
//the correct command for this 22 10 27 and the formula is 0.1*A for  cranking type 
  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||     // Correct length
      CAN.read() == 0x41 ||     // Correct mode
      CAN.read() == 0x42) {     // Correct PID for Battery Voltage During Last Cranking
    // Read and parse the battery voltage during last cranking value
    float batteryVoltage = CAN.read() * 0.05; // Convert to voltage (assuming each LSB is 0.05 volts)

    // Print battery voltage during last cranking
    Serial.print("Battery Voltage During Last Cranking: ");
    Serial.print(batteryVoltage);
    Serial.println(" V");
    delay(5000);
  }
}
