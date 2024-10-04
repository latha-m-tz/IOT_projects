#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Odometer Reader");

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
  // Send the request for odometer data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x00); // Odometer PID
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||     // Correct length
      CAN.read() == 0x41 ||     // Correct mode
      CAN.read() == 0x00) {     // Correct PID for odometer
    // Read and parse the odometer value
    int odometer_msb = CAN.read(); // Most significant byte
    int odometer_lsb = CAN.read(); // Least significant byte

    // Combine the two bytes to form the odometer value 
    unsigned int odometer_raw = (odometer_msb << 8) | odometer_lsb;

    // Convert raw odometer value to actual distance traveled in kilometers
    float odometer_km = odometer_raw * 0.1;

    // Print the odometer value
    Serial.print("Odometer: ");
    Serial.print(odometer_km);
    Serial.println(" km");
    delay(5000);
  }
}
