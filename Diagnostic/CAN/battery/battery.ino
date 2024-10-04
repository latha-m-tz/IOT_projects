#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Battery Voltage Reader");

  // Start the CAN bus at 500 kbps
  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }


  // Add filter to only receive the CAN bus IDs we care about
  if (useStandardAddressing) {
    CAN.filter(0x7e8);
  } else {
    CAN.filterExtended(0x18daf110);
  }
}
   
void loop() {
  // Send the request for battery voltage data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x22); // Number of additional bytes
  CAN.write(0x01); // Show current data
  CAN.write(0x06); // Battery voltage PID
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0 ||
         CAN.read() == 0x01  ||         // Correct length
         CAN.read() != 0x41  ||     // Correct mode
         CAN.read() != 0x06);
              // Correct PID

  // Read and parse battery voltage
  int voltage_MSB = CAN.read();
  int voltage_LSB = CAN.read();
 // Serial.print(voltage_MSB);
  //Serial.print("  ");
  //Serial.print(voltage_LSB);
  float batteryVoltage = ((voltage_MSB * 256) + voltage_LSB ) * 0.1; // Convert to volts
  Serial.print("Battery Voltage: ");
  Serial.print(batteryVoltage, 2); // Print with 2 decimal places
  Serial.println(" V");
}