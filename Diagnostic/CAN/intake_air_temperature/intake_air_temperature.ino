#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Intake Air Temperature Reader");

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
  // Send the request for intake air temperature data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x0F); // Intake air temperature PID
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x03 ||     // Correct length
      CAN.read() != 0x41 ||     // Correct mode
      CAN.read() != 0x0F) {     // Correct PID for intake air temperature
    // Read and parse the intake air temperature value
    int tempRaw = CAN.read(); // Temperature value

    // Convert raw value to actual temperature
    float intakeAirTemperature = (tempRaw - 40); // Formula for converting raw value to temperature in Celsius

    // Print intake air temperature
    Serial.print("Intake Air Temperature (°C): ");
    Serial.println(intakeAirTemperature);
    delay(5000);
  }
}
