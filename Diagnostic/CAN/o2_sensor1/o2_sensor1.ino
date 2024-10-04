#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Oxygen Sensor 1 Voltage and Short Term Fuel Trim Reader");

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
  // Send the request for Oxygen Sensor 1 Voltage data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x14); // Oxygen Sensor 1 Voltage PID
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||     // Correct length
      CAN.read() == 0x41 ||     // Correct mode
      CAN.read() == 0x14) {     // Correct PID for Oxygen Sensor 1 Voltage
    // Read and parse the Oxygen Sensor 1 Voltage value
    float o2SensorVoltage = CAN.read() * 0.005; // Convert to voltage (assuming each LSB is 0.005 volts)

    // Print Oxygen Sensor 1 Voltage value
    Serial.print("Oxygen Sensor 1 Voltage: ");
    Serial.print(o2SensorVoltage);
    Serial.println(" V");
  }

  // Send the request for Short Term Fuel Trim data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x06); // Short Term Fuel Trim PID
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||     // Correct length
      CAN.read() == 0x41 ||     // Correct mode
      CAN.read() == 0x06) {     // Correct PID for Short Term Fuel Trim
    // Read and parse the Short Term Fuel Trim value
    int shortTermFuelTrim = CAN.read() - 128; // Convert to signed value

    // Print Short Term Fuel Trim value
    Serial.print("Short Term Fuel Trim: ");
    Serial.print(shortTermFuelTrim);
    Serial.println("%");
  }

  delay(5000);
}
