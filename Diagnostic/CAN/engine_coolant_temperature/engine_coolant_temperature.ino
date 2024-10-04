#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Engine Coolant Temperature Reader");

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
  // Send the request for engine coolant temperature data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x05); // Engine coolant temperature PID
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||     // Correct length
      CAN.read() == 0x41 ||     // Correct mode
      CAN.read() == 0x05) {     // Correct PID for engine coolant temperature
    // Read and parse the engine coolant temperature value
    int tempRaw = CAN.read(); // Temperature value

    // Convert raw value to actual temperature
    float coolantTemperature = tempRaw - 40; // Formula for converting raw value to temperature in Celsius

    // Print engine coolant temperature
    Serial.print("Engine Coolant Temperature (°C): ");
    Serial.println(coolantTemperature);
    delay(5000);
  }
}
