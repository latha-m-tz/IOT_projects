#include <CAN.h>

const bool useStandardAddressing = true;
const unsigned long durationInSeconds = 30; // Duration for which to read RPM (in seconds)

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Engine Speed Reader");

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
  unsigned long startTime = millis(); // Record start time
  unsigned long elapsedTime = 0; // Initialize elapsed time

  while (elapsedTime < durationInSeconds * 1000) { // Convert duration to milliseconds
    // Send the request for engine speed
    if (useStandardAddressing) {
      CAN.beginPacket(0x7df, 8);
    } else {
      CAN.beginExtendedPacket(0x18db33f1, 8);
    }
    CAN.write(0x02); // Number of additional bytes
    CAN.write(0x01); // Request current data
    CAN.write(0x0C); // Engine speed PID
    CAN.endPacket();

    // Wait for response
    while (CAN.parsePacket() == 0 ||
           CAN.read() != 0x10 || CAN.read()  != 0x14 ||
           CAN.read() != 0x41 || // correct length and mode 
           CAN.read() != 0x0C);                        // correct PID

    // Read and parse the engine speed (RPM) response
    unsigned int rpmRaw_MSB = CAN.read();
    unsigned int rpmRaw_LSB = CAN.read();
    unsigned int rpmRaw = (rpmRaw_MSB << 8) | rpmRaw_LSB;
    unsigned int rpm = rpmRaw * 0.5; // Calculate RPM

    // Display engine speed
    Serial.print("Engine Speed (RPM): ");
    Serial.println(rpm);

    elapsedTime = millis() - startTime; // Update elapsed time
  }

  Serial.println("Duration expired. Stopping RPM readings.");

  while (1); // Stop further processing after the duration
}
