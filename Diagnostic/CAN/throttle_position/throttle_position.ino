#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Throttle Position Reader");

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
  // Send the request for throttle position data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x11); // Throttle position PID
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x03 ||     // Correct length
      CAN.read() != 0x41 ||     // Correct mode
      CAN.read() != 0x11) {     // Correct PID for throttle position
    // Read and parse the throttle position value
    int throttleRaw = CAN.read(); // Throttle position value

    // Convert raw throttle position value to percentage
    float throttlePosition = throttleRaw * 0.3921568627; // Scale factor: 100 / 255

    // Print throttle position
    Serial.print("Throttle Position (%): ");
    Serial.print(throttlePosition); // Print the throttle position percentage
    Serial.println("%");
    delay(5000);
  }
}
