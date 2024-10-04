#include <CAN.h>

const bool useStandardAddressing = true;

// Enumerate the methods
enum Method {
  Method_A,
  Method_B,
  Method_C,
  Method_D,
  Method_E
};

// Choose the method to call
Method selectedMethod = Method_E;

void setup() {
  Serial.begin(9600);
  Serial.println("CAN OBD-II Parameter Reader");

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
  // Call the selected method based on the condition
  switch(selectedMethod) {
    case Method_A:
      readBatteryVoltage();
      break;
    case Method_B:
      readUnsuccessfulCrankings();
      break;
    case Method_C:
      readDistanceMIL();
      break;
    case Method_D:
      readKilometerRunCounter();
      break;
    case Method_E:
      readO2SensorState();
      break;
  }

  delay(5000); // Adjust delay as needed
}


void readBatteryVoltage() {
  // Send the request for battery voltage data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Show current data
  CAN.write(0x42); // Battery voltage PID
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0 ||
         CAN.read() == 0x02  ||         // Correct length
         CAN.read() != 0x41  ||     // Correct mode0
         CAN.read() != 0x42);     // Correct PID

  // Read and parse battery voltage
  int voltage_MSB = CAN.read();
  int voltage_LSB = CAN.read();
  float batteryVoltage = ((voltage_MSB * 256) + voltage_LSB ) * 0.001; // Convert to volts

  // Print battery voltage
  Serial.print("Battery Voltage: ");
  Serial.print(batteryVoltage, 2); // Print with 2 decimal places
  Serial.println(" V");
}

void readUnsuccessfulCrankings() {
  // Send the request for number of unsuccessful crankings
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x1C); // Number of unsuccessful crankings PID
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x02 ||     // Correct length
      CAN.read() == 0x41 ||     // Correct mode
      CAN.read() == 0x1C) {     // Correct PID for number of unsuccessful crankings
    // Read and parse the number of unsuccessful crankings
    int unsuccessfulCrankings = CAN.read(); // Number of unsuccessful crankings

    // Print the number of unsuccessful crankings
    Serial.print("Number of Unsuccessful Crankings: ");
    Serial.println(unsuccessfulCrankings);
  }
}

void readDistanceMIL() {
  // Send the request for distance traveled with MIL on data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x21); // Replace  with the appropriate PID for MIL status and distance traveled
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||     // Correct length
      CAN.read() != 0x41 ||     // Correct mode
      CAN.read() != 0x21)
       {    
    // Read and parse the data for MIL status and distance traveled
    int distanceRaw_LSB = CAN.read(); // Distance traveled with MIL on LSB
    int distanceRaw_MSB = CAN.read(); // Distance traveled with MIL on MSB
    unsigned int distanceRaw = (distanceRaw_MSB << 8) | distanceRaw_LSB;

    // Apply the formula to calculate distance traveled with MIL on
    float distance = distanceRaw * 0.1;

    // Print the distance traveled with MIL on
    Serial.print("Distance Traveled with MIL on: ");
    Serial.print(distance); // Print the distance value
    Serial.println(" miles");
  }
}

void readKilometerRunCounter() {
  // Send the request for Kilometer Run Counter data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x21); // PID for Kilometer Run Counter
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||     // Correct length
      CAN.read() == 0x41 ||     // Correct mode
      CAN.read() == 0x21) {     // Correct PID for Kilometer Run Counter
    // Read and parse the Kilometer Run Counter value
    int kilometerRunCounter_LSB = CAN.read(); // Least significant byte
    int kilometerRunCounter_MSB = CAN.read(); // Most significant byte
    int kilometerRunCounter = (kilometerRunCounter_MSB << 8) | kilometerRunCounter_LSB;

    // Print Kilometer Run Counter
    Serial.print("Kilometer Run Counter: ");
    Serial.print(kilometerRunCounter);
    Serial.println("km");
  }
}

void readO2SensorState() {
  // Send the request for O2 Sensor State data
  if (useStandardAddressing) {
    CAN.beginPacket(0x7df, 8);
  } else {
    CAN.beginExtendedPacket(0x18db33f1, 8);
  }
  CAN.write(0x02); // Number of additional bytes
  CAN.write(0x01); // Request current data
  CAN.write(0x13); // PID for O2 Sensor State
  CAN.endPacket();

  // Wait for response
  while (CAN.parsePacket() == 0);

  // Check if the response is for the requested PID
  if (CAN.read() == 0x04 ||     // Correct length
      CAN.read() == 0x41 ||     // Correct mode
      CAN.read() == 0x13) {     // Correct PID for O2 Sensor State
    // Read and parse the O2 Sensor State value
    int o2SensorState = CAN.read(); // O2 Sensor State value

    // Print O2 Sensor State
    Serial.print("O2 Sensor State: ");
    Serial.println(o2SensorState);
  }

  delay(5000);
}

