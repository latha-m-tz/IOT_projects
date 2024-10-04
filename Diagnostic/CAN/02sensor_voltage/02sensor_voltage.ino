#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II O2 Sensor Voltage Reader");

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
  // Send the request for O2 sensor voltage data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x09); // PID for O2 sensor voltage
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||     // Correct length
      CAN.read() != 0x41 ||     // Correct mode
      CAN.read() != 0x09) {     // Correct PID for O2 sensor voltage
    // Read and parse the O2 sensor voltage
    int voltage_MSB = CAN.read();
    int voltage_LSB = CAN.read();
    float o2SensorVoltage = ((voltage_MSB * 256) + voltage_LSB ) * 0.005; // Convert to volts

    // Print O2 sensor voltage
    Serial.print("O2 Sensor Voltage: ");
    Serial.print(o2SensorVoltage, 2); // Print with 2 decimal places
    Serial.println(" V");
  }

  // Add a delay before sending the next request
  delay(5000);
}
