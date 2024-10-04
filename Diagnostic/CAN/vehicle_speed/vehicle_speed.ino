#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Vehicle Speed Reader");

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
  // Send the request for vehicle speed data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Show current data
  CAN.write(0x0D); // Vehicle speed PID
  CAN.endPacket();
                         
  // Wait for response
  while (CAN.parsePacket() == 0 ||
         CAN.read() == 0x03 ||     // Correct length
         CAN.read() != 0x41 ||     // Correct mode
         CAN.read() != 0x0D);      // Correct PID for vehicle speed

  // Read and parse vehicle speed
  int speedRaw = CAN.read(); // Read the vehicle speed value

  // Convert raw speed value to actual speed in km/h
  float speedKmH = speedRaw * 0.01;

  // Print vehicle speed
  Serial.print("Vehicle Speed (km/h): ");
  Serial.print(speedKmH); // Print the vehicle speed value in km/h
  Serial.println(" km/h");

  // Indicate end of processing
  // Serial.println("That's all folks!");

  // Add a delay before starting the next iteration
  delay(5000); // 5-second delay
}
