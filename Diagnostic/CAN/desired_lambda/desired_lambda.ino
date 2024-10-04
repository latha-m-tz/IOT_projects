//desire lamda means o2 sensor 2 air fuel equivlent ration
#include <CAN.h>

const bool useStandardAddressing = true;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Desired Lambda Reader");

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
  // Send the request for desired lambda data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x25); // PID for desired lambda
  CAN.endPacket();
//the correct command for this 22 01 39 and the formula is (1*A+128)/(256)  

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||     // Correct length
      CAN.read() != 0x41 ||     // Correct mode
      CAN.read() != 0x25) {     // Correct PID for desired lambda
    // Read and parse the desired lambda value
    int desiredLambdaRaw = CAN.read(); // Desired lambda value

    // Convert raw value to actual desired lambda
    float desiredLambda = desiredLambdaRaw*1/32768; // Convert from 0.1 units to actual value

    // Print desired lambda
    Serial.print("Desired Lambda: ");
    Serial.println(desiredLambda);
    delay(5000);
  }
}
