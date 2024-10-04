#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Gear Sensor Voltage Reader");

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
  // Send the request for Gear Sensor Voltage data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x21); // PID for Gear Sensor Voltage
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||     // Correct length
      CAN.read() != 0x41 ||     // Correct mode
      CAN.read() != 0x21) {     // Correct PID for Gear Sensor Voltage
    // Read and parse the Gear Sensor Voltage value
    int sensorVoltage_LSB = CAN.read(); // Least significant byte
    int sensorVoltage_MSB = CAN.read(); // Most significant byte
    int sensorVoltage_raw = (sensorVoltage_MSB << 8) | sensorVoltage_LSB;
    float gearSensorVoltage = (5.0 / 1024) * sensorVoltage_raw;

    // Print Gear Sensor Voltage
    Serial.print("Gear Sensor Voltage: ");
    Serial.println(gearSensorVoltage);
  }

  delay(5000);
}
