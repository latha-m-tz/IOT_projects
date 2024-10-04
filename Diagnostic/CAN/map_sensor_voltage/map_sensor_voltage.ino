#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II MAP Sensor Voltage Reader");

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
  // Send the request for MAP sensor voltage data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x0B); // MAP Sensor Voltage PID
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x02 ||     // Correct length
      CAN.read() == 0x41 ||     // Correct mode
      CAN.read() == 0x0B) {     // Correct PID for MAP Sensor Voltage
    // Read and parse the MAP sensor voltage value
    float mapSensorVoltage = CAN.read() * 0.00488281; // Convert to voltage (assuming each LSB is 0.005 volts)

    // Print MAP sensor voltage value
    Serial.print("MAP Sensor Voltage: ");
    Serial.print(mapSensorVoltage);
    Serial.println(" V");
    delay(5000);
  }
}
